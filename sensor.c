/*********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
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
        syslog(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        return -1;
    }

    tbuf = fgets(line, LINE_BUFFER_SIZE, file);
    // done with the file
    fclose(file);

    if (!tbuf) {
        syslog(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        return -1;
    }
    
    len = strlen(line);
    line[len-1] = 0;
    
    tbuf = NULL;
    value = strtol(line, &tbuf, 10);
    if (tbuf == line) {
        syslog(LOG_ERR, "%s: invalid temperature", sensor->path);
        return -1;
    }

    return value * 0.001;
}
