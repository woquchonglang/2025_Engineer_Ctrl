#include "./app/Oled.h"
#include "./hdl/mt6701_pwm.h"
#include "./hdl/soft_iic_as5600.h"
#include "./hdl/soft_iic_mt6701.h"
#include "app/Mpu6050.h"
#include "app/Sensor.h"
#include "app/Uart.h"
#include "app/bluetooch.h"
#include "app/nvs.h"
#include "as5600.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hdl/button.h"
#include "hdl/buzzer.h"
#include "sdkconfig.h"
#include "soc/gpio_num.h"
#include <stdint.h>

static const char *TAG = "main";

// i2c_0 : scl:1 sdo:2 , i2c_1 :scl:42 sdo:41

extern "C" {

bool pump = false;
}

Buzzer buzzer(GPIO_NUM_8);
AS5600 joint1 = AS5600(I2C_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
MT6701 joint2 = MT6701(I2C_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
AS5600 joint4 = AS5600(I2C_NUM_1, GPIO_NUM_42, GPIO_NUM_41);
MT6701 joint5 = MT6701(I2C_NUM_1, GPIO_NUM_42, GPIO_NUM_41);
mt6701_pwm joint3(GPIO_NUM_8, 994, 4119, 16, 4111);
soft_AS5600 joint6;
soft_MT6701 joint7;

SemaphoreHandle_t buzzer_xSemaphore = NULL;

QueueHandle_t jointQueue;

extern "C" void app_main(void)
{
    nvs_init();
    buzzer.init();
    button_init(9);       // enter
    button_down_init(11); // down
    button_up_init(12);   // up

    joint3.init();

    //   joint1.init();
    //   joint2.init();
    //   joint4.init();
    //   joint5.init();

    bluetooch_init();

    jointQueue = xQueueCreate(10, 39);

    xTaskCreate((TaskFunction_t)oled_task, "oled_task", 4096, NULL, 6, NULL);
    xTaskCreate((TaskFunction_t)sensor_task, "sensor_task", 4096, (void *)jointQueue, 6, NULL);
    // xTaskCreate((TaskFunction_t)uart_task, "uart_task", 1048, (void *)jointQueue, 5, NULL);
    xTaskCreate((TaskFunction_t)bluetooch_task, "bluetooch_task", 4096, (void *)jointQueue, 6,
                NULL); // 无线调试
    // xTaskCreate((TaskFunction_t)buzzer_task, "buzzer_task", 4096,
    // NULL, 6, NULL);

    ESP_LOGI(TAG, "init");
}
