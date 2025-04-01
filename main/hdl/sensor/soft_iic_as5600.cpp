#include "soft_iic_as5600.h"


#define PI 3.14159265358979f
static const char *TAG = "soft_AS5600";
soft_AS5600::soft_AS5600() {}

float soft_AS5600::getSensorAngle() {

  // softiic_Hw_write_data(device_addr, &raw_angle_addr, 1);
  // softiic_Hw_read_data(device_addr, raw_angle_buf, 2);

      raw_angle = (uint16_t)(raw_angle_buf[0] << 8 | raw_angle_buf[1]);
  angle = (((int)raw_angle & 0b0000111111111111) * 360.0f / 4096.0f) *
          (PI / 180.0f);

  return angle;
}