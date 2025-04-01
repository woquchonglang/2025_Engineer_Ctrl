#ifndef MT6701_PWM_H
#define MT6701_PWM_H


#include "soc/gpio_num.h"
#include "stdint.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "common/base_classes/Sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Pcnt {
public:
    void init(gpio_num_t pinPWM, int _high_limit,  pcnt_watch_cb_t cb_func);

    pcnt_unit_handle_t unit;
    pcnt_channel_handle_t chan;
    QueueHandle_t queue;
};

class mt6701_pwm : public Sensor {
public:
    /** MagneticSensorPWM(uint8_t _pinPWM, int _min, int _max)
    * @param _pinPWM  the pin that is reading the pwm from magnetic sensor
    * @param _min_raw_count  the smallest expected reading
    * @param _max_raw_count  the largest expected reading
    */
    mt6701_pwm(gpio_num_t _pinPWM, int _min = 0, int _max = 0);
    /** MagneticSensorPWM(uint8_t _pinPWM, int freqHz, int _total_pwm_clocks, int _min_pwm_clocks, int _max_pwm_clocks)
     * 
     * Constructor that computes the min and max raw counts based on the PWM frequency and the number of PWM clocks in one period
     * 
     * @param _pinPWM  the pin that is reading the pwm from magnetic sensor
     * @param freqHz  the frequency of the PWM signal, in Hz, e.g. 115, 230, 460 or 920 for the AS5600, depending on the PWM frequency setting
     * @param _total_pwm_clocks  the total number of PWM clocks in one period, e.g. 4351 for the AS5600
     * @param _min_pwm_clocks  the 0 value returned by the sensor, in PWM clocks, e.g. 128 for the AS5600
     * @param _max_pwm_clocks  the largest value returned by the sensor, in PWM clocks, e.g. 4223 for the AS5600
     */
    mt6701_pwm(gpio_num_t _pinPWM, int freqHz, int _total_pwm_clocks, int _min_pwm_clocks,
               int _max_pwm_clocks);

    // initialize the sensor hardware
    void init();

    gpio_num_t pinPWM;

    // get current angle (rad)
    float getSensorAngle() override;

    // pwm handler
    static bool handlePWM(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata,
                          void *user_ctx);
    void enableInterrupt(void (*doPWM)());
    unsigned long pulse_length_us;

private:
    // raw count (typically in range of 0-1023)
    int raw_count;
    int min_raw_count;
    int max_raw_count;
    int cpr;

    // flag saying if the readings are interrupt based or not
    bool is_interrupt_based;

    int read();

    /**
     * Function getting current angle register value
     * it uses angle_register variable
     */
    int getRawCount();

    // time tracking variables
    unsigned long last_call_us;
    // unsigned long pulse_length_us;

    Pcnt pcnt;
};


#endif
