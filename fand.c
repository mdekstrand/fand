/*********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#include "config.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>
#include <libgen.h>

#include "layout.h"

static const char const *optchars = "c:d";
static const struct option opt_table[] = {
    {"config-file", true, NULL, 'c'},
    {"debug", false, NULL, 'd'},
    { NULL }
};

static const char *config_file = SYSCONFDIR "/fand.conf";

static int log_opts = LOG_NDELAY | LOG_PID;

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
            log_opts = LOG_PERROR;
            break;
        case '?':
            exit(2);
        }
    }
}

void run_loop(fand_layout_t *layout)
{
    for (int i = 0; i < layout->sensor_count; i++) {
        fand_sensor_t *sensor = layout->sensors[i];
        float temp = sensor_read(sensor);
        syslog(LOG_DEBUG, "%s: %.1f C", sensor->name, temp);
    }
}

int main(int argc, char *argv[])
{
    parse_args(argc, argv);
    openlog(basename(argv[0]), log_opts, LOG_DAEMON);
    layout = load_layout(config_file);

    while (true) {
        run_loop(layout);
        sleep(layout->interval);
    }
}
