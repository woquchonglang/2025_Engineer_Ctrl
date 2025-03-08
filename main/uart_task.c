#include "uart_task.h"
#include "crc.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_timer.h" 
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "string.h"

#define TXD_PIN (GPIO_NUM_21)
#define RXD_PIN (GPIO_NUM_20)

#define TX_BUF_SIZE 1024

void uart_init(void) {
  const uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };
  // We won't use a buffer for sending data.
  uart_driver_install(UART_NUM_1, TX_BUF_SIZE * 2, 0, 0, NULL, 0);
  uart_param_config(UART_NUM_1, &uart_config);
  uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
}

float get_freq(void) {
  int64_t current_time = esp_timer_get_time();
  static int64_t last_time = 0;
  float frequency = 1000000.0 / (current_time - last_time);
  last_time = current_time;
  return frequency;
}

void tx_task(void *param) {
  uint8_t tx_data[39];
  tx_data_t *tx_tmp = NULL;
  float joint[7] = {0};
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (1) {
    if (xQueueReceive(param, &tx_data, 0) == pdTRUE) {
      if (Verify_CRC8_Check_Sum((uint8_t *)tx_data, sizeof(frame_header_t)) &&
          Verify_CRC16_Check_Sum((uint8_t *)tx_data, sizeof(tx_data_t))) {

        tx_tmp = (tx_data_t *)&tx_data;
        for(int i = 0; i < 7; i++) {
          joint[i] = (float)(tx_tmp->ctrl_data.joints[i]);
          // ESP_LOGI("TX_TASK", "joint[%d]: %f", i, joint[i]);
        }
 
        // ESP_LOGI("TX_TASK", "Frequency: %f", get_freq());

        uart_write_bytes(UART_NUM_1, tx_data, sizeof(tx_data_t));
      }
    }
    vTaskDelayUntil(&(xLastWakeTime), (40 / portTICK_PERIOD_MS));
  }
}
