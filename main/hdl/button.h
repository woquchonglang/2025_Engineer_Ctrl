#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

#include "../ui/MiaoUI/ui_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

static void button_event_cb(void *arg, void *data);
void button_init(uint32_t button_num);
void button_up_init(uint32_t button_num);
void button_down_init(uint32_t button_num);

#if CONFIG_ENTER_LIGHT_SLEEP_MODE_MANUALLY
void button_enter_power_save(void *usr_data);
#endif

UI_ACTION Button_Scan(void);

#ifdef __cplusplus
}
#endif


#endif