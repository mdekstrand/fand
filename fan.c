/*********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libdaemon/dlog.h>
#include <errno.h>

#include "fan.h"
#include "util.h"

fand_fan_t* fan_new(const char *name, const char *path,
                    fand_sensor_t *sensor)
{
    fand_fan_t *fan = xmalloc(sizeof(fand_fan_t));
    fan->name = name;
    fan->path = path;
    fan->sensor = sensor;
    fan->min_temp = 20;
    fan->max_temp = 45;
    fan->min_power = 0;
    fan->max_power = 0;
    return fan;
}

void fan_free(fand_fan_t *fan)
{
    free(fan);
}

void fan_update(fand_fan_t *fan)
{
    if (!fan->sensor) return;
    float temp = fan->sensor->temp;
    int power = -1;
    if (temp < 0) {
        daemon_log(LOG_INFO, "fan %s: sensor %s has no value",
                   fan->name, fan->sensor->name);
        return;
    } else if (temp <= fan->min_temp) {
        power = fan->min_power;
    } else if (temp >= fan->max_temp) {
        power = fan->max_power;
    } else {
        int prange = fan->max_power - fan->min_power;
        float trange = fan->max_temp - fan->min_temp;
        float delta = (temp - fan->min_temp) / trange * prange;
        power = fan->min_power + (int) delta;
    }
    daemon_log(LOG_DEBUG, "fan %s: setting power to %d",
               fan->name, power);
}
