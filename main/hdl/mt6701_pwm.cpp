#include "mt6701_pwm.h"
#include "esp_timer.h"
#include "math.h"
#include "soc/gpio_num.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"

const static char *TAG = "mt6701_pwm";

mt6701_pwm::mt6701_pwm(gpio_num_t _pinPWM, int _min_raw_count, int _max_raw_count)
{
    pinPWM = _pinPWM;

    cpr = _max_raw_count - _min_raw_count + 1;
    min_raw_count = _min_raw_count;
    max_raw_count = _max_raw_count;

    // define if the sensor uses interrupts
    is_interrupt_based = false;

    // define as not set
    last_call_us = esp_timer_get_time();
}


mt6701_pwm::mt6701_pwm(gpio_num_t _pinPWM, int freqHz, int _total_pwm_clocks, int _min_pwm_clocks,
                       int _max_pwm_clocks)
{
    pinPWM = _pinPWM;

    min_raw_count = _min_pwm_clocks;
    max_raw_count = _max_pwm_clocks;
    // min_raw_count = lroundf(1000000.0f / freqHz / _total_pwm_clocks * _min_pwm_clocks);
    // max_raw_count = lroundf(1000000.0f / freqHz / _total_pwm_clocks * _max_pwm_clocks);
    // cpr = max_raw_count - min_raw_count + 1;

    ESP_LOGI(TAG, "min_raw_count: %d, max_raw_count: %d, cpr: %d", min_raw_count, max_raw_count,
             cpr);

}


void mt6701_pwm::init()
{
    gpio_set_direction(pinPWM, GPIO_MODE_INPUT);
    pcnt.init(pinPWM, max_raw_count, handlePWM);
    uint32_t tof_ticks;
    // this->Sensor::init(); // call base class init
}

void Pcnt::init(gpio_num_t pinPWM, int _high_limit, pcnt_watch_cb_t cb_func)
{
    ESP_LOGI(TAG, "install pcnt unit");

    pcnt_unit_config_t unit_config = {
        .low_limit = -1,
        .high_limit = _high_limit,
    };
    unit = NULL;
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &unit));

    ESP_LOGI(TAG, "set glitch filter");
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(unit, &filter_config));

    ESP_LOGI(TAG, "install pcnt channels");
    pcnt_chan_config_t chan_config = {
        .edge_gpio_num = pinPWM,
        .level_gpio_num = -1,
    };
    chan = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(unit, &chan_config, &chan));

    ESP_LOGI(TAG, "set edge and actions for pcnt channels");
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(chan, PCNT_CHANNEL_EDGE_ACTION_INCREASE,
                                                 PCNT_CHANNEL_EDGE_ACTION_HOLD));

    ESP_LOGI(TAG, "add watch points");
    int watch_points[] = { 16, 4096 };
    for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(unit, watch_points[i]));
    }

    ESP_LOGI(TAG, "register callbacks");
    pcnt_event_callbacks_t cbs = {
        .on_reach = cb_func,
    };
    queue = xQueueCreate(10, sizeof(int));
    ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(unit, &cbs, queue));

    ESP_LOGI(TAG, "enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(unit));
    ESP_LOGI(TAG, "clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(unit));
    ESP_LOGI(TAG, "start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(unit));
}

// get current angle (rad)
float mt6701_pwm::getSensorAngle()
{
    // raw data from sensor
    raw_count = getRawCount();
    // if (raw_count > max_raw_count)
    //     raw_count = max_raw_count;
    // if (raw_count < min_raw_count)
    //     raw_count = min_raw_count;
    return ((float)(raw_count - min_raw_count) / (float)cpr) * 2 * M_PI;
}

int mt6701_pwm::getRawCount()
{
    int pulse_count = 0;
    int event_count = 0;
    if (xQueueReceive(pcnt.queue, &event_count, pdMS_TO_TICKS(100))) {
        ESP_LOGI(TAG, "Watch point event, count: %d", event_count);
    } else {
        // ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt.unit, &pulse_count));
        // ESP_LOGI(TAG, "Pulse count: %d", pulse_count);
    }
    return event_count;
}

bool mt6701_pwm::handlePWM(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata,
                           void *user_ctx)
{
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // send event data to queue, from this interrupt callback
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    return (high_task_wakeup == pdTRUE);
    ESP_LOGI(TAG, "pwm callback");
}
