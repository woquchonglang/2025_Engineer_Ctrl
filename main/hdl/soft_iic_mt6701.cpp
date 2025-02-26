
/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soft_iic_mt6701.h"
#include "esp_check.h"
#include "esp_log.h"
#include "math.h"
#include "../hal/soft_iic.h"

static const char *TAG = "MT6701";

/* Constants for CRC calculation */
static uint8_t tableCRC6[64] = {
    0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09, 0x18, 0x1B, 0x1E,
    0x1D, 0x14, 0x17, 0x12, 0x11, 0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F,
    0x3A, 0x39, 0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21, 0x23,
    0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A, 0x3B, 0x38, 0x3D, 0x3E,
    0x37, 0x34, 0x31, 0x32, 0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19,
    0x1A, 0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02};

/* 32-bit input data, right alignment, Calculation over 18 bits (mult. of 6) */
static uint8_t CRC6_43_18bit(uint32_t w_InputData) {
  uint8_t b_Index = 0;
  uint8_t b_CRC = 0;

  b_Index = (uint8_t)(((uint32_t)w_InputData >> 12u) & 0x0000003Fu);

  b_CRC = (uint8_t)(((uint32_t)w_InputData >> 6u) & 0x0000003Fu);
  b_Index = b_CRC ^ tableCRC6[b_Index];

  b_CRC = (uint8_t)((uint32_t)w_InputData & 0x0000003Fu);
  b_Index = b_CRC ^ tableCRC6[b_Index];

  b_CRC = tableCRC6[b_Index];

  return b_CRC;
}

soft_MT6701::soft_MT6701() { is_installed = false; }

soft_MT6701::~soft_MT6701() {
  if (is_installed) {
    deinit();
  }
}

void soft_MT6701::init() {
  esp_err_t ret = ESP_OK;
  bus = NULL;
  soft_i2c_master_config_t config = {
      .scl_pin = 40, .sda_pin = 39, .freq = SOFT_I2C_100KHZ,};

  ret = soft_i2c_master_new(&config, &bus);
ESP_LOGI(TAG,"%d",ret);
  is_installed = true;

}

void soft_MT6701::deinit() {
   is_installed = false;
}

float soft_MT6701::getSensorAngle() {

  static float angle = 0.0;
  static float previous_angle = 0.0;

  uint8_t raw_angle_addr = 0x03;
  uint8_t raw_angle_buffer[2] = {0};


  soft_i2c_master_write_read(bus, 0x06, &raw_angle_addr, 1, raw_angle_buffer, 2);

  angle = ((int)((raw_angle_buffer[0] << 6) | (raw_angle_buffer[1] >> 2))) *
          0.00038349519f;
  
  return angle;
}
