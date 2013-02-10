/*********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#include "config.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <libdaemon/daemon.h>

#include "util.h"
#include "layout.h"

static const char const *optchars = "c:dfki:";
static const struct option opt_table[] = {
    {"config-file", true, NULL, 'c'},
    {"debug", false, NULL, 'd'},
    {"foreground", false, NULL, 'f'},
    {"kill", false, NULL, 'k'},
    {"interval", true, NULL, 'i'},
    { NULL }
};

static const char *config_file = SYSCONFDIR "/fand.conf";

fand_layout_t *layout = NULL;
static bool daemonize = true;
static bool kill_mode = false;
static int interval = -1;

static void parse_args(int argc, char *argv[])
{
    int c;
    while ((c = getopt_long(argc, argv, optchars, opt_table, NULL)) >= 0) {
        switch (c) {
        case 'c':
            config_file = optarg;
            break;
        case 'd':
            daemon_set_verbosity(LOG_DEBUG);
            break;
        case 'f':
            daemonize = false;
            break;
        case 'k':
            kill_mode = true;
            break;
        case 'i':
            interval = atol(optarg);
            break;
        case '?':
            exit(2);
        }
    }
}

static void run_fans(fand_layout_t *layout)
{
    // update the sensors
    for (int i = 0; i < layout->sensor_count; i++) {
        fand_sensor_t *sensor = layout->sensors[i];
        float temp = sensor_read(sensor);
        daemon_log(LOG_DEBUG, "%s: %.1f C", sensor->name, temp);
    }

    // run the fans
    for (int i = 0; i < layout->fan_count; i++) {
        fand_fan_t *fan = layout->fans[i];
        fan_update(fan);
    }
}

static void main_loop()
{
    bool keep_running = true;
    int fd;
    fd_set fds;
    int delay = interval;
    if (delay <= 0) {
        delay = layout->interval;
    }

    run_fans(layout);
    alarm(delay);

    fd = daemon_signal_fd();
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    
    while (keep_running) {
        fd_set fds2 = fds;
        if (select(FD_SETSIZE, &fds2, NULL, NULL, NULL) < 0) {
            if (errno == EINTR) {
                continue;
            }
            daemon_log(LOG_ERR, "select: %s", strerror(errno));
            break;
        }
        if (FD_ISSET(fd, &fds2)) {
            int sig;
            sig = daemon_signal_next();
            if (sig <= 0) {
                daemon_log(LOG_ERR, "daemon_signal_next: %s",
                           strerror(errno));
                break;
            }
            switch (sig) {
            case SIGINT:
            case SIGQUIT:
            case SIGTERM:
                daemon_log(LOG_INFO, "exiting");
                keep_running = false;
                break;
            case SIGHUP:
                daemon_log(LOG_INFO, "received SIGHUP, reloading configuration");
                layout_free(layout);
                layout = load_layout(config_file);
                if (!layout) {
                    daemon_log(LOG_ERR, "error reading config file");
                    keep_running = false;
                }
                delay = interval;
                if (delay <= 0) {
                    delay = layout->interval;
                }
                break;
            case SIGALRM:
                run_fans(layout);
                alarm(delay);
                break;
            default:
                daemon_log(LOG_WARNING,
                           "received unknown signal %d, confused",
                           sig);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    daemon_pid_file_ident = daemon_log_ident =
        daemon_ident_from_argv0(argv[0]);
    parse_args(argc, argv);

    if (daemon_reset_sigs(-1) < 0 || daemon_unblock_sigs(-1)) {
        daemon_log(LOG_ERR, "failed to initialize signals: %s",
                   strerror(errno));
        return 2;
    }

    if (kill_mode) {
        int ret = daemon_pid_file_kill_wait(SIGTERM, 5);
        if (ret < 0) {
            daemon_log(LOG_ERR, "Failed to kill daemon: %s",
                       strerror(errno));
            return 1;
        } else {
            return 0;
        }
    }

    int pid = daemon_pid_file_is_running();
    if (pid >= 0) {
        daemon_log(LOG_ERR, "daemon already running on pid %u", pid);
        return 1;
    }

    if (daemonize) {
        if (daemon_retval_init() < 0) {
            daemon_log(LOG_ERR, "failed to initialize daemon framework");
            return 2;
        }
        pid = daemon_fork();
    } else {
        pid = 0;
    }

    if (pid < 0) {
        daemon_log(LOG_ERR, "failed to fork: %s", strerror(errno));
        daemon_retval_done();
        return 2;
    } else if (pid) {
        /* parent process */
        int ret = daemon_retval_wait(20);
        if (ret < 0) {
            daemon_log(LOG_ERR, "error receiving return value: %s",
                       strerror(errno));
            return 2;
        } else if (ret) {
            daemon_log(LOG_ERR, "daemon returned code %u", ret);
        } else {
            daemon_log(LOG_INFO, "daemon started successfully");
        }
        return ret;
    } else {
        /* child or no fork */
        if (daemonize) {
            if (daemon_close_all(-1) < 0) {
                daemon_log(LOG_ERR, "failed to close file descriptors: %s",
                           strerror(errno));
                daemon_retval_send(2);
                return 2;
            }
            if (daemon_pid_file_create() < 0) {
                daemon_log(LOG_ERR, "error creating PID file: %s",
                           strerror(errno));
                daemon_retval_send(2);
                return 2;
            }
        }
        layout = load_layout(config_file);
        if (!layout) {
            if (daemonize) {
                daemon_retval_send(1);
                daemon_pid_file_remove();
            }
            return 1;
        }

        if (daemon_signal_init(SIGINT, SIGTERM, SIGQUIT, SIGHUP,
                               SIGALRM, 0) < 0) {
            daemon_log(LOG_ERR, "error initializing signals: %s",
                       strerror(errno));
            if (daemonize) {
                daemon_retval_send(2);
                daemon_pid_file_remove();
            }
            return 2;
        }
        daemon_retval_send(0);

        main_loop();

        daemon_signal_done();
        if (daemonize) {
            daemon_pid_file_remove();
        }
    }
    return 0;
}
