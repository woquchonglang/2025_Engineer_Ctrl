#include "Sensor.h"
#include "../hdl/sensor/sensor.h"
#include "as5600.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mt6701.h"
#include <cstdint>
#include <sys/types.h>
#include "string.h"
#include "../algorithm/crc.h"

float joint[7] = { 0 };


extern As5600 joint1;
extern Mt6701 joint2;
extern mt6701_pwm joint3;
extern soft_AS5600 joint4;
extern soft_MT6701 joint5;
extern soft_MT6701 joint7;
// extern Mt6701 joint7;

void sensor_init()
{
    // joint1.init();
    // joint2.init();
    // joint3.init();
    // joint4.init();
    // joint5.init();
    // joint6.init();
    joint7.init();
}

void sensor_task(void *param)
{
    tx_data_t tx_data;
    memset(&tx_data, 0, sizeof(tx_data_t));
    tx_data.frame_header.sof = 0xA5;
    tx_data.frame_header.seq = 0;
    tx_data.frame_header.data_len = 30;
    tx_data.cmd_id = 0x0302;
    tx_data.frame_header.crc8 = Get_CRC8_Check_Sum((uint8_t *)&(tx_data.frame_header),
                                                   sizeof(frame_header_t) - 1, 0xFF);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // joint_1 = joint1.getSensorAngle();
        // joint[1] = joint2.getSensorAngle();
        joint[6] = joint7.getSensorAngle();
        // float joint3_angle = joint3.getSensorAngle();


        // ESP_LOGI("TAG", "",);
        // ESP_LOGI("TAG", "angle: %f, GPIO level: %d\n", joint[2] ,gpio_get_level(GPIO_NUM_8));
        // ESP_LOGI(TAG, "%f,%f,%f\n", joint_1, joint_2, joint_3);
        // ESP_LOGI("TAG", "%f\n", joint[6]);

        for (int i = 0; i < 7; i++) {
            tx_data.ctrl_data.joints[i] = (joint[i]);
        }
        // crc
        uint32_t total_size = sizeof(frame_header_t) + sizeof(uint16_t) + 30 + sizeof(uint16_t);
        Append_CRC16_Check_Sum((uint8_t *)&tx_data, total_size);
        // tx_buf
        uint8_t tx_buffer[39] = { 0 };
        memcpy(tx_buffer, &tx_data, sizeof(tx_data_t));

        xQueueSend((QueueHandle_t)param, tx_buffer, 0);
        vTaskDelayUntil(&(xLastWakeTime), (33 / portTICK_PERIOD_MS));
    }
}

float *get_angle()
{
    return joint;
}