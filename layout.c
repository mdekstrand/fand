/*********************************************************************
 * fand - control fans using sysfs
 * setup.c: read the configuration file
 ********************************************************************/

#include "config.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <libdaemon/dlog.h>
#include <confuse.h>

#include "util.h"
#include "layout.h"

static cfg_opt_t sensor_opts[] = {
    CFG_STR("path", NULL, CFGF_NONE),
    CFG_END()
};

static cfg_opt_t fan_opts[] = {
    CFG_STR("path", NULL, CFGF_NONE),
    CFG_STR("sensor", NULL, CFGF_NONE),
    CFG_FLOAT("min_temp", 25.0, CFGF_NONE),
    CFG_FLOAT("max_temp", 45.0, CFGF_NONE),
    CFG_INT("min_power", 0, CFGF_NONE),
    CFG_INT("max_power", 255, CFGF_NONE),
    CFG_END()
};

static cfg_opt_t options[] = {
    CFG_SEC("sensor", sensor_opts,
            CFGF_MULTI | CFGF_TITLE | CFGF_NO_TITLE_DUPES),
    CFG_SEC("fan", fan_opts,
            CFGF_MULTI | CFGF_TITLE | CFGF_NO_TITLE_DUPES),
    CFG_INT("interval", 10, CFGF_NONE),
    CFG_END()
};

static fand_layout_t* setup_new(cfg_t *cfg)
{
    fand_layout_t* setup = xmalloc(sizeof(fand_layout_t));
    setup->config = cfg;
    setup->interval = 0;
    setup->sensor_count = 0;
    setup->sensors = NULL;
    setup->fan_count = 0;
    setup->fans = NULL;
    return setup;
}

static bool setup_globals(fand_layout_t *setup, cfg_t *cfg)
{
    setup->interval = cfg_getint(cfg, "interval");
    return true;
}

static bool setup_sensors(fand_layout_t *setup, cfg_t *cfg)
{
    int n, i;
    cfg_t *sec;

    setup->sensor_count = n = cfg_size(cfg, "sensor");
    setup->sensors = xmalloc(n * sizeof(fand_sensor_t*));
    for (i = 0; i < n; i++) {
        setup->sensors[i] = NULL;
    }

    for (i = 0; i < n; i++) {
        sec = cfg_getnsec(cfg, "sensor", i);
        const char *title = cfg_title(sec);
        const char *path = cfg_getstr(sec, "path");
        if (path == NULL) {
            daemon_log(LOG_ERR, "sensor %s: no path provided", title);
            return false;
        }
        daemon_log(LOG_INFO, "sensor %s: %s", title, path);
        setup->sensors[i] = sensor_new(title, path);
    }
    daemon_log(LOG_INFO, "configured %d sensors", setup->sensor_count);
    return true;
}

fand_sensor_t* layout_find_sensor(fand_layout_t *layout,
                                  const char *name)
{
    for (int i = 0; i < layout->sensor_count; i++) {
        fand_sensor_t *sensor = layout->sensors[i];
        if (strcmp(name, sensor->name) == 0) {
            return sensor;
        }
    }
    return NULL;
}


static bool setup_fans(fand_layout_t *setup, cfg_t *cfg)
{
    int n, i;
    cfg_t *sec;

    setup->fan_count = n = cfg_size(cfg, "fan");
    setup->fans = xmalloc(n * sizeof(fand_fan_t*));
    for (i = 0; i < n; i++) {
        setup->fans[i] = NULL;
    }

    for (i = 0; i < n; i++) {
        sec = cfg_getnsec(cfg, "fan", i);
        const char *title = cfg_title(sec);
        const char *path = cfg_getstr(sec, "path");
        if (!path) {
            daemon_log(LOG_ERR, "fan %s: no path provided", title);
            return false;
        }
        const char *sensor_name = cfg_getstr(sec, "sensor");
        fand_sensor_t *sensor;
        if (sensor_name) {
            sensor = layout_find_sensor(setup, sensor_name);
            if (!sensor) {
                daemon_log(LOG_WARNING, "fan %s: unknown sensor %s",
                           title, sensor_name);
            }
        } else {
            daemon_log(LOG_WARNING, "fan %s: no sensor specified", title);
            sensor = NULL;
        }
        daemon_log(LOG_INFO, "fan %s: %s (sensor %s)",
                   title, path, sensor->name);
        fand_fan_t *fan = fan_new(title, path, sensor);
        fan->min_temp = cfg_getfloat(sec, "min_temp");
        fan->max_temp = cfg_getfloat(sec, "max_temp");
        fan->min_power = cfg_getint(sec, "min_power");
        fan->max_power = cfg_getint(sec, "max_power");
        setup->fans[i] = fan;
    }
    daemon_log(LOG_INFO, "configured %d fans", setup->fan_count);
    return true;
}

fand_layout_t* load_layout(const char *cfgfn)
{
    /* load the configuration. everything is carefully ordered to make freeing
       safe. */
    fand_layout_t* setup = NULL;
    cfg_t *cfg;
    daemon_log(LOG_INFO, "loading configuration from %s", cfgfn);
    cfg = cfg_init(options, CFGF_NONE);
    if (!cfg) {
        daemon_log(LOG_ERR, "cannot initialize libconfuse");
        return NULL;
    }
    if (cfg_parse(cfg, cfgfn) == CFG_PARSE_ERROR) {
        daemon_log(LOG_ERR, "%s: error parsing config file", cfgfn);
        cfg_free(cfg);
        return NULL;
    }

    setup = setup_new(cfg);
    if (!setup) {
        daemon_log(LOG_CRIT, "cannot allocate memory");
        cfg_free(cfg);
        return NULL;
    }
    
    if (!setup_globals(setup, cfg)) {
        layout_free(setup);
        return NULL;
    }

    if (!setup_sensors(setup, cfg)) {
        layout_free(setup);
        return NULL;
    }

    if (!setup_fans(setup, cfg)) {
        layout_free(setup);
        return NULL;
    }

    return setup;
}

void layout_free(fand_layout_t *setup)
{
    if (!setup) return;

    if (setup->sensors) {
        for (int i = 0; i < setup->sensor_count; i++) {
            xfree(setup->sensors[i]);
        }
    }
    xfree(setup->sensors);

    if (setup->fans) {
        for (int i = 0; i < setup->fan_count; i++) {
            xfree(setup->fans[i]);
        }
    }
    xfree(setup->fans);

    xfree(setup);
}
