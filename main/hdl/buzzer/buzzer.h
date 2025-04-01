#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "driver/gpio.h"

class Buzzer {
public:
    Buzzer(gpio_num_t pin);
    void proc(uint32_t frequency, uint32_t duration);
    void init();
   

private:
    gpio_num_t _pin;
};

void buzzer_task();

#endif