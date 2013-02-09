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
#include "memory.h"

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
    char *eol = NULL;
    long int temp;
    FILE* file = fopen(sensor->path, "r");

    if (!file) {
        syslog(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        return -1;
    }

    if (!fgets(line, LINE_BUFFER_SIZE, file)) {
        syslog(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        fclose(file);
        return -1;
    }
    fclose(file);
    
    len = strlen(line);
    line[len-1] = 0;
    
    temp = strtol(line, &eol, 10);
    if (eol == line) {
        syslog(LOG_ERR, "%s: invalid temperature", sensor->path);
        return -1;
    }

    return temp * 0.001;
}
