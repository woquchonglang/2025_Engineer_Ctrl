#include "Oled.h"

#ifdef __cplusplus
extern "C" {
#include "../ui/MiaoUI/core/ui.h"
#include "../ui/MiaoUI/ui_conf.h"
#include "../ui/lib/u8g2/u8g2.h"

#endif
}

#include "esp_check.h"
#include "esp_timer.h"
#include "ssd1306.h"
#include <cstdint>

#define I2C_SDA_IO   42
#define I2C_SCL_IO   41

#define I2C_NUM      I2C_NUM_1

#define ACK_CHECK_EN 0x1              /*!< I2C master will check ack from slave*/
#define WRITE_BIT    I2C_MASTER_WRITE /*!< I2C master write */

static const char *TAG = "oled";
ssd1306_handle_t dev = NULL;
extern u8g2_t u8g2;

/* Task Handle */
TaskHandle_t ui_task_handle;
TaskHandle_t WaveTestTask_Handle;
TaskHandle_t SystemStateUpdateTask_Handle;

/* Queue Handle */
QueueHandle_t WaveTest_Queue;

/* Global Variable */
int Wave_TestData;
char FreeHeap[20];

i2c_config_t i2c_config = {
    .mode = I2C_MODE_MASTER,             // 主机模式
    .sda_io_num = I2C_SDA_IO,            // sda i引脚编号
    .scl_io_num = I2C_SCL_IO,            // scl 引脚编号
    .sda_pullup_en = GPIO_PULLUP_ENABLE, // 上拉使能
    .scl_pullup_en = GPIO_PULLUP_ENABLE, // 上拉使能
    .master{ .clk_speed = 400000 }       // 100k
};
void esp32_i2c_write(uint8_t addr, uint32_t idx, uint8_t *data)
{
    i2c_cmd_handle_t handler = i2c_cmd_link_create();
    i2c_master_start(handler);
    i2c_master_write_byte(handler, addr | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(handler, data, idx, 2);
    i2c_master_stop(handler);
    i2c_master_cmd_begin(I2C_NUM, handler, 100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(handler);
}
// u8g2用到的显示屏控制接口
uint8_t u8x8_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    static uint8_t buffer[32]; /* u8g2/u8x8 will never send more than 32 bytes
                                between START_TRANSFER and END_TRANSFER */
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg) {
    case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;
        while (arg_int > 0) {
            buffer[buf_idx++] = *data;
            data++;
            arg_int--;
        }
        break;
    case U8X8_MSG_BYTE_INIT:
        /* add your custom code to init i2c subsystem */
        break;
    case U8X8_MSG_BYTE_SET_DC:
        /* ignored for i2c */
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        buf_idx = 0;
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        esp32_i2c_write(u8x8_GetI2CAddress(u8x8), buf_idx, buffer);
        break;
    default:
        return 0;
    }
    return 1;
}
// u8g2用到的系统资源
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        oled_init(); // 调用iic初始化
        break;
    case U8X8_MSG_DELAY_MILLI:
        vTaskDelay(arg_int);
        break;
    default:
        return 0;
    }
    return 1;
}

void oled_init()
{
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM, &i2c_config));
    ESP_LOGI(TAG, "i2c %d config done", I2C_NUM);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0));
    ESP_LOGI(TAG, "i2c %d install done", I2C_NUM);

    dev = ssd1306_create(I2C_NUM, SSD1306_I2C_ADDRESS);

    ESP_LOGI(TAG, "oled create done");
}

void oled_task()
{
    ui_t ui;

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_i2c, u8x8_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SendBuffer(&u8g2); // 清屏
    ESP_LOGI(TAG, "u8g2 init done");
    Disp_DrawLine(0, 0, 128, 64);
    u8g2_SendBuffer(&u8g2); // 清屏
    // MiaoUi_Setup(&ui);

    while (1) {
        // ui_loop(&ui);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// 测试代码
//  void oled_task() {
//    dev = ssd1306_create(I2C_NUM, SSD1306_I2C_ADDRESS);
//    while (1) {
//      ssd1306_clear_screen(dev, 0x00);
//      ssd1306_draw_string(dev, 0, 0, (uint8_t *)"Hello World!", 12, 1);
//      ssd1306_refresh_gram(dev);
//      vTaskDelay(10 / portTICK_PERIOD_MS);
//    }
//  }