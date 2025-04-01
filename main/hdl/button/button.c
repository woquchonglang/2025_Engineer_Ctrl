/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "button.h"
#include "../../ui/MiaoUI/ui_conf.h"
#include "esp_idf_version.h"
#include "esp_log.h"
#include "esp_pm.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_button.h"
#include <stdio.h>

#define BOOT_BUTTON_NUM     4
#define BUTTON_ACTIVE_LEVEL 0

extern TaskHandle_t buzzer_Handle;
BaseType_t xHigherPriorityTaskWoken = pdFALSE;


static const char *TAG = "button";
UI_ACTION action = UI_ACTION_NONE;
UI_ACTION last_action = UI_ACTION_NONE;
static void button_event_cb(void *arg, void *data)
{
    button_event_t event;
    iot_button_print_event((button_handle_t)arg);
    event = iot_button_get_event((button_handle_t)arg);
    switch (event) {
    case BUTTON_SINGLE_CLICK:
        vTaskNotifyGiveFromISR(buzzer_Handle, &xHigherPriorityTaskWoken);
        action = UI_ACTION_ENTER;
        break;
    default:
        break;
    }
}

static void button_up_event_cb(void *arg, void *data)
{
    button_event_t event;
    iot_button_print_event((button_handle_t)arg);
    event = iot_button_get_event((button_handle_t)arg);
    switch (event) {
    case BUTTON_SINGLE_CLICK:
        vTaskNotifyGiveFromISR(buzzer_Handle, &xHigherPriorityTaskWoken);
        action = UI_ACTION_UP;
        break;
    default:
        break;
    }
}

static void button_dowm_event_cb(void *arg, void *data)
{
    button_event_t event;
    iot_button_print_event((button_handle_t)arg);
    event = iot_button_get_event((button_handle_t)arg);
    switch (event) {
    case BUTTON_SINGLE_CLICK:
        vTaskNotifyGiveFromISR(buzzer_Handle, &xHigherPriorityTaskWoken);
        action = UI_ACTION_DOWN;
        break;
    default:
        break;
    }
}

void button_click_init(uint32_t button_num)
{
    button_config_t btn_cfg = {
      .type = BUTTON_TYPE_GPIO,
      .gpio_button_config =
          {
              .gpio_num = button_num,
              .active_level = BUTTON_ACTIVE_LEVEL,
          },
  };
    button_handle_t btn = iot_button_create(&btn_cfg);
    iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, button_event_cb, NULL);
}
void button_down_init(uint32_t button_num)
{
    button_config_t btn_cfg = {
      .type = BUTTON_TYPE_GPIO,
      .gpio_button_config =
          {
              .gpio_num = button_num,
              .active_level = BUTTON_ACTIVE_LEVEL,
          },
  };
    button_handle_t btn = iot_button_create(&btn_cfg);
    iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, button_dowm_event_cb, NULL);
}
void button_up_init(uint32_t button_num)
{
    button_config_t btn_cfg = {
      .type = BUTTON_TYPE_GPIO,
      .gpio_button_config =
          {
              .gpio_num = button_num,
              .active_level = BUTTON_ACTIVE_LEVEL,
          },
  };
    button_handle_t btn = iot_button_create(&btn_cfg);
    iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, button_up_event_cb, NULL);
}

// 因为按键回调速率高于主循环，所以需要一个变量来存储上一次的按键状态来刷新按键状态

UI_ACTION Button_Scan(void)
{
    if (last_action != action) {
        last_action = action;
        return action;
    } else {
        action = UI_ACTION_NONE;
        return UI_ACTION_NONE;
    }
}
