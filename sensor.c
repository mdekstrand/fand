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

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libdaemon/dlog.h>
#include <errno.h>

#include "sensor.h"
#include "util.h"

// 12-character lines are adequate to read a temperature
#define LINE_BUFFER_SIZE 12

fand_sensor_t* sensor_new(const char *name, const char *path)
{
    fand_sensor_t *sensor = xmalloc(sizeof(fand_sensor_t));
    sensor->name = name;
    sensor->path = path;
    return sensor;
}

void sensor_free(fand_sensor_t *sensor)
{
    free(sensor);
}

/**
 * Read a sensor in decidegrees Celcius.
 */
float sensor_read(fand_sensor_t *sensor)
{
    char line[LINE_BUFFER_SIZE];
    int len;
    char *tbuf = NULL;
    long int value;
    FILE* file = fopen(sensor->path, "r");

    if (!file) {
        daemon_log(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        return -1;
    }

    tbuf = fgets(line, LINE_BUFFER_SIZE, file);
    // done with the file
    fclose(file);

    if (!tbuf) {
        daemon_log(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        return -1;
    }
    
    len = strlen(line);
    line[len-1] = 0;
    
    tbuf = NULL;
    value = strtol(line, &tbuf, 10);
    if (tbuf == line) {
        daemon_log(LOG_ERR, "%s: invalid temperature", sensor->path);
        return -1;
    }

    sensor->temp = value * 0.001;
    return sensor->temp;
}
