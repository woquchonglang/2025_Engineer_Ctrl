
#include "esp_err.h"
#include "./buzzer.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LEDC_TIMER     LEDC_TIMER_0
#define LEDC_MODE      LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL   LEDC_CHANNEL_0
#define LEDC_DUTY_RES  LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY      (4096)            // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY (4000)            // Frequency in Hertz. Set frequency at 4 kHz

extern Buzzer buzzer;
extern SemaphoreHandle_t buzzer_xSemaphore;

Buzzer::Buzzer(gpio_num_t pin)
{
    _pin = pin;
}

void Buzzer::init()
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = { .speed_mode = LEDC_MODE,
                                       .duty_resolution = LEDC_DUTY_RES,
                                       .timer_num = LEDC_TIMER,
                                       .freq_hz = LEDC_FREQUENCY, // Set output frequency at 4 kHz
                                       .clk_cfg = LEDC_AUTO_CLK };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = { .gpio_num = _pin,
                                           .speed_mode = LEDC_MODE,
                                           .channel = LEDC_CHANNEL,
                                           .intr_type = LEDC_INTR_DISABLE,
                                           .timer_sel = LEDC_TIMER,

                                           .duty = 0, // Set duty to 0%
                                           .hpoint = 0 };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void Buzzer::proc(uint32_t frequency, uint32_t duration)
{
    ESP_ERROR_CHECK(ledc_set_freq(LEDC_MODE, LEDC_TIMER, frequency));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
    vTaskDelay(duration / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

void buzzer_task()
{
    while (true) {
        if (xSemaphoreTake(buzzer_xSemaphore, portMAX_DELAY) == pdTRUE) {
            buzzer.proc(LEDC_FREQUENCY, 1000); // 例如，频率为4kHz，持续时间为1秒
        }
    vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


