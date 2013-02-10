/*********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#ifndef FAND_FAN_H
#define FAND_FAN_H

#include "sensor.h"

typedef struct fand_fan {
    const char *name;
    const char *path;
    const fand_sensor_t *sensor;
    float min_temp, max_temp;
    int min_power, max_power;
} fand_fan_t;

fand_fan_t* fan_new(const char *name, const char *path,
                    fand_sensor_t *sensor);
void fan_free(fand_fan_t *fan);

void fan_update(fand_fan_t *fan);

#endif //!defined(FAND_FAN_H)
