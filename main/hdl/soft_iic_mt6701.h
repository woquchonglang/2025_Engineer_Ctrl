
/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "common/base_classes/Sensor.h"
#include "driver/gpio.h"
#include "../hal/soft_iic.h"

class soft_MT6701 : public Sensor {
public:
  soft_MT6701();
  ~soft_MT6701();

  void init();
  void deinit();
  float getSensorAngle();

private:
  soft_i2c_master_bus_t bus;
  bool is_installed;
};
