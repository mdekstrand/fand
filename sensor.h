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

#ifndef FAND_SENSOR_H
#define FAND_SENSOR_H

typedef struct fand_sensor {
    const char *name;
    const char *path;
    float temp;
} fand_sensor_t;

fand_sensor_t* sensor_new(const char *name, const char *path);
void sensor_free(fand_sensor_t *sensor);

/**
 * Read a sensor in decidegrees Celcius.
 */
float sensor_read(fand_sensor_t *sensor);

#endif //!defined(FAND_SENSOR_H)
