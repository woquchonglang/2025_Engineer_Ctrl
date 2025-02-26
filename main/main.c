#include "BT_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "uart_task.h"

QueueHandle_t joint_queue;

void app_main() {

  joint_queue = xQueueCreate(20, 39);

  nvs_flash_init();
  uart_init();
  bluetooch_init();

  xTaskCreate(tx_task, "tx_task", 2048, joint_queue, 5, NULL);
}