/*********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#include "config.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <libdaemon/daemon.h>

#include "util.h"
#include "layout.h"

static const char const *optchars = "c:d";
static const struct option opt_table[] = {
    {"config-file", true, NULL, 'c'},
    {"debug", false, NULL, 'd'},
    { NULL }
};

static const char *config_file = SYSCONFDIR "/fand.conf";

fand_layout_t *layout = NULL;

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
        case '?':
            exit(2);
        }
    }
}

static void run_fans(fand_layout_t *layout)
{
    for (int i = 0; i < layout->sensor_count; i++) {
        fand_sensor_t *sensor = layout->sensors[i];
        float temp = sensor_read(sensor);
        daemon_log(LOG_DEBUG, "%s: %.1f C", sensor->name, temp);
    }
}

static void main_loop()
{
    while (true) {
        run_fans(layout);
        sleep(layout->interval);
    }
}

int main(int argc, char *argv[])
{
    daemon_pid_file_ident = daemon_log_ident =
        daemon_ident_from_argv0(argv[0]);
    parse_args(argc, argv);
    layout = load_layout(config_file);
    main_loop();
    return 0;
}
