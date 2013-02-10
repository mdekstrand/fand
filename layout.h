/********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#ifndef FAND_LAYOUT_H
#define FAND_LAYOUT_H

#include <confuse.h>
#include "sensor.h"
#include "fan.h"

typedef struct fand_layout {
    cfg_t *config;
    int interval;
    int sensor_count;
    fand_sensor_t **sensors;
    int fan_count;
    fand_fan_t **fans;
} fand_layout_t;

fand_layout_t* load_layout(const char *fn);
void layout_free(fand_layout_t *layout);

fand_sensor_t* layout_find_sensor(fand_layout_t *layout,
                                  const char *name);

#endif //!defined(FAND_LAYOUT_H)
