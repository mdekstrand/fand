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
