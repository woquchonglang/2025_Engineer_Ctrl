#include "Mpu6050.h"
#include "../algorithm/crc.h"
#include "../algorithm/kalman.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "math.h"
#include "mpu6050.h"
#include "string.h"
#include <stdio.h>

#define I2C_MASTER_SCL_IO 1       /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 2       /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

static const char *TAG = "mpu6050";
static mpu6050_handle_t mpu6050;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)
static const int RX_BUF_SIZE = 1024;

void i2c_bus_init(void) {
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
  conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

  i2c_param_config(I2C_MASTER_NUM, &conf);

  i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void uart_init(void) {
  const uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };
  // We won't use a buffer for sending data.
  uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
  uart_param_config(UART_NUM_1, &uart_config);
  uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
}

int sendData(const char *data) {
  const int len = strlen(data);
  const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
  // ESP_LOGI(logName, "Wrote %d bytes", txBytes);
  return txBytes;
}

/**
 * @brief i2c master initialization
 */
void i2c_sensor_mpu6050_init(void) {

  i2c_bus_init();

  mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);

  mpu6050_config(mpu6050, ACCE_FS_2G, GYRO_FS_1000DPS);

  mpu6050_wake_up(mpu6050);
}

void mpu6050_task(void) {

  uint8_t mpu6050_deviceid;

  static mpu6050_acce_value_t acce;
  static mpu6050_gyro_value_t gyro;

  static complimentary_angle_t angle;

  static float kf_roll;
  static float kf_pitch;

  static KalmanFilter roll_kf;
  static KalmanFilter pitch_kf;

  i2c_sensor_mpu6050_init();

  mpu6050_get_deviceid(mpu6050, &mpu6050_deviceid);

  KalmanFilter_Init(&roll_kf, 0, 0.01, 0.1);
  KalmanFilter_Init(&pitch_kf, 0, 0.01, 0.1);

  while (1) {

    mpu6050_get_acce(mpu6050, &acce);
    mpu6050_get_gyro(mpu6050, &gyro);

    mpu6050_complimentory_filter(mpu6050, &acce, &gyro, &angle);

    kf_roll = KalmanFilter_Update(&roll_kf, angle.roll);
    kf_pitch = KalmanFilter_Update(&roll_kf, angle.pitch);

    // ESP_LOGI(TAG, "%.2f,%.2f\n", angle.roll, angle.pitch);
    ESP_LOGI(TAG, "%.2f,%.2f\n", kf_roll, kf_pitch);

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }

  mpu6050_delete(mpu6050);
  i2c_driver_delete(I2C_MASTER_NUM);
}
