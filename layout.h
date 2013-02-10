/********************************************************************
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
