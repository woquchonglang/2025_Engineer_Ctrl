
/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "common/base_classes/Sensor.h"
#include "driver/gpio.h"
#include "../../hal/soft_iic.h"
#include <cstdint>
#include <sys/types.h>

class soft_MT6701 : public Sensor {
public:
    soft_MT6701(uint8_t SDL_PIN, uint8_t SCL_PIN);
    ~soft_MT6701();

    void init();
    void deinit();
    float getSensorAngle();

private:
    soft_i2c_master_bus_t bus;
    uint8_t SCL_PIN;
    uint8_t SDL_PIN;
    bool is_installed;
    float angle = 0;
    uint8_t raw_angle_addr = 0x03;
    uint8_t mt6701_addr = 0x08;
};
