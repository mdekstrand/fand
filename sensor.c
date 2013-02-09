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

static char* readline(FILE* file) {
    int size = 12;
    int to_fetch = size;
    char* full = xmalloc(12);
    char* buf = full;

    memset(full, 0, size);

    do {
        if (!fgets(buf, to_fetch, file)) {
            if (full == buf) {
                free(full);
                return NULL;
            } else {
                return full;
            }
        }
        int len = strlen(full);
        if (full[len - 1] == '\n') {
            full[len - 1] = 0;
            return full;
        } else {
            size = size * 2;
            full = xrealloc(full, size);
            buf = full + len;
            to_fetch = size - len;
        }
    } while (1);
}

/**
 * Read a sensor in decidegrees Celcius.
 */
int sensor_read(fand_sensor_t *sensor)
{
    FILE* file = fopen(sensor->path, "r");
    char* line;
    char* eol = NULL;
    int ret = -1;
    long int temp;
    if (!file) {
        syslog(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        goto done;
    }

    line = readline(file);
    if (line == NULL) {
        syslog(LOG_ERR, "%s: %s", sensor->path, strerror(errno));
        goto closefile;
    }

    temp = strtol(line, &eol, 10);
    if (eol == line) {
        syslog(LOG_ERR, "%s: invalid temperature", sensor->path);
        goto freeline;
    }

    ret = (int) temp / 100;

freeline:
    free(line);
closefile:
    fclose(file);
done:
    return ret;
}
