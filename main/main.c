#include "BT_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "uart_task.h"
#include "ws2812_control.h"

QueueHandle_t joint_queue;

void app_main() {

  joint_queue = xQueueCreate(30, 39);

  nvs_flash_init();
  uart_init();
  bluetooch_init();

  ws2812_strip_t *WS2812 = ws2812_create();
  led_set_on(WS2812, COLOR_BLUE);

  xTaskCreate(tx_task, "tx_task", 2048, joint_queue, 5, NULL);
}