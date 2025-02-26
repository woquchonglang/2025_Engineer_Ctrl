#include "uart_task.h"
#include "crc.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "string.h"
#include <stdint.h>

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

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

void tx_task(void *param) {
  // TickType_t xLastWakeTime;
  // xLastWakeTime = xTaskGetTickCount();

  uint8_t tx_data[39];
  tx_data_t *tx_da = NULL;
  float joint[7] = {0};

  uint16_t cnt = 0;

  while (1) {
    if (xQueueReceive(param, &tx_data, 0) == pdTRUE) {
      if (Verify_CRC8_Check_Sum((uint8_t *)tx_data, sizeof(frame_header_t)) &&
          Verify_CRC16_Check_Sum((uint8_t *)tx_data, sizeof(tx_data_t))) {  

        tx_da = (tx_data_t *)&tx_data;
        joint[2] = (float)(tx_da->ctrl_data.joints[2]);

        ESP_LOGI("TX_TASK", "joint[%d]: %f", 2, joint[2]);

        vTaskDelay(100);

      } else {
      }
    }
  }
}