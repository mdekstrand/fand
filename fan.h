/*********************************************************************
 * fand - control fans using sysfs
 * Copyright 2013 Michael Ekstrand <michael@elehack.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
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
