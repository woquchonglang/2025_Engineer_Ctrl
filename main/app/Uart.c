#include "Uart.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TXD_PIN        (17)
#define RXD_PIN        (16)

#define RX_BUFFER_SIZE (30U)

extern float joint[7];

void uart_init()
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUFFER_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
void uart_task(void *param)
{
    uint8_t tx_buffer[39];
    while (1) {
        xQueueReceive(param, &tx_buffer, portMAX_DELAY);
        uart_write_bytes(UART_NUM_1, tx_buffer, sizeof(tx_buffer));
    }
}
