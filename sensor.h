/*********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#ifndef FAND_SENSOR_H
#define FAND_SENSOR_H

typedef struct fand_sensor {
    const char *name;
    const char *path;
} fand_sensor_t;

fand_sensor_t* sensor_new(const char *name, const char *path);
void sensor_free(fand_sensor_t *sensor);

/**
 * Read a sensor in decidegrees Celcius.
 */
float sensor_read(fand_sensor_t *sensor);

#endif //!defined(FAND_SENSOR_H)
