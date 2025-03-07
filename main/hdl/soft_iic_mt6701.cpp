#include "soft_iic_mt6701.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "math.h"
#include "../hal/soft_iic.h"


static const char *TAG = "MT6701";

soft_MT6701::soft_MT6701(uint8_t _SDL_PIN, uint8_t _SCL_PIN)
{
    this->SDL_PIN = _SDL_PIN;
    this->SCL_PIN = _SCL_PIN;
    is_installed = false;
}

soft_MT6701::~soft_MT6701()
{
    if (is_installed) {
        deinit();
    }
}

void soft_MT6701::init()
{
    esp_err_t ret = ESP_OK;
    bus = NULL;
    soft_i2c_master_config_t config = {
        .scl_pin = SCL_PIN,
        .sda_pin = SDL_PIN,
        .freq = SOFT_I2C_100KHZ,
    };

    ret = soft_i2c_master_new(&config, &bus);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "soft_i2c_master_new failed");
    }
    is_installed = true;
}

void soft_MT6701::deinit()
{
    is_installed = false;
}

float soft_MT6701::getSensorAngle()
{
    uint8_t raw_angle_buffer[2] = { 0 };

    soft_i2c_master_write_read(bus, 0x06, &raw_angle_addr, 1, raw_angle_buffer, 2);

    angle = ((int)((raw_angle_buffer[0] << 6) | (raw_angle_buffer[1] >> 2))) * 0.00038349519f;

    return angle;
}
