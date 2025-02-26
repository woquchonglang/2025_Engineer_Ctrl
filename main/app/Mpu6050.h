#ifndef MPU6050_TASK_H
#define MPU6050_TASK_H

#ifdef __cplusplus
extern "C" {
#endif


void i2c_bus_init(void);
void i2c_sensor_mpu6050_init(void);
void mpu6050_task(void);

#ifdef __cplusplus
}
#endif


#endif

