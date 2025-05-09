#include "./app/Oled.h"
#include "./hdl/sensor/sensor.h"
#include "app/Mpu6050.h"
#include "app/Uart.h"
#include "app/Sensor.h"
#include "app/bluetooch.h"
#include "app/nvs.h"
#include "as5600.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hdl/button/button.h"
#include "hdl/buzzer/buzzer.h"
#include "sdkconfig.h"
#include "soc/gpio_num.h"
#include <stdint.h>

TaskHandle_t buzzer_Handle;

static const char *TAG = "main";

extern "C" {

bool pump = false;
}

Buzzer buzzer(GPIO_NUM_10);
// As5600 joint1 = As5600(I2C_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
// Mt6701 joint2 = Mt6701(I2C_NUM_1, GPIO_NUM_2, GPIO_NUM_1);
// As5600 joint4 = As5600(I2C_NUM_1, GPIO_NUM_42, GPIO_NUM_41);
// Mt6701 joint5 = Mt6701(I2C_NUM_1, GPIO_NUM_42, GPIO_NUM_41);

// mt6701_pwm joint3(GPIO_NUM_8, 994, 4119, 16, 4111);
// soft_AS5600 joint6;
soft_MT6701 joint7(GPIO_NUM_1, GPIO_NUM_2);

QueueHandle_t jointQueue;

extern "C" void app_main(void)
{
    nvs_init();
    buzzer.init();
    button_up_init(8);     // up
    button_click_init(18); // enter
    button_down_init(3);  // down
    // sensor_init();
    // uart_init();

    // bluetooch_init();

    jointQueue = xQueueCreate(10, 39);

    xTaskCreate((TaskFunction_t)oled_task, "oled_task", 4096, NULL, 6, NULL);
    // xTaskCreate((TaskFunction_t)sensor_task, "sensor_task", 4096, (void *)jointQueue, 6, NULL);
    // xTaskCreate((TaskFunction_t)uart_task, "uart_task", 4096, (void *)jointQueue, 6, NULL);
    // xTaskCreate((TaskFunction_t)bluetooch_task, "bluetooch_task", 4096, (void *)jointQueue, 6,
    //             NULL); // 无线调试
    xTaskCreate((TaskFunction_t)buzzer_task, "buzzer_task", 4096, NULL, 6, &buzzer_Handle);

    ESP_LOGI(TAG, "init");
}
