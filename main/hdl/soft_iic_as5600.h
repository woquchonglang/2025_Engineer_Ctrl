
#pragma once

#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#include "../hal/soft_iic.h"
}
#endif

class soft_AS5600 {
public:
  soft_AS5600();
  ~soft_AS5600();
  void init();
  void deinit();
  float getSensorAngle();

private:
  gpio_num_t _scl_io;
  gpio_num_t _sda_io;
  bool is_installed;
  uint8_t device_addr = 0x36;
  uint8_t raw_angle_addr = 0x0C;
  uint8_t raw_angle_buf[2] = {0};
  uint16_t raw_angle;
  float angle;
};
