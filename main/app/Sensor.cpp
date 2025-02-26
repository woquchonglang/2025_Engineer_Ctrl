#include "Sensor.h"
#include "../hdl/mt6701_pwm.h"
#include "../hdl/soft_iic_as5600.h"
#include "../hdl/soft_iic_mt6701.h"
#include "as5600.h"
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


extern AS5600 joint1;
extern AS5600 joint2;
extern mt6701_pwm joint3;
extern soft_AS5600 joint4;
extern soft_MT6701 joint5;

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
        // joint_2 = joint2.getSensorAngle();
        // joint[2] = joint2.getSensorAngle();
        float joint3_angle = joint3.getSensorAngle();

        joint[2] = 1.0;

        // ESP_LOGI(TAG, "%f\n", joint[2]);
        // ESP_LOGI(TAG, "%f,%f,%f\n", joint_1, joint_2, joint_3);
        // ESP_LOGI(TAG, "%f\n", joint3_angle);

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