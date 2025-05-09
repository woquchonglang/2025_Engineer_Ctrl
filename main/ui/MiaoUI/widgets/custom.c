/**
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) <2024>  <JianFeng>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "custom.h"
#include "../display/dispDriver.h"
#include "../indev/indevDriver.h"

#include <stdio.h>
#include "../images/image.h"
#include "../version.h"
#include "..//ui_conf.h"


#if ( UI_USE_FREERTOS == 1 )
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#endif

const unsigned char UI_NAME_LOGO[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x78, 0x3c, 0xf0, 0x8c, 0xf0, 0xcc, 0xc8, 0xf3, 0x3c, 0x40, 0x04, 0x10, 0x8c, 0x0c, 
    0xc4, 0x48, 0x10, 0x64, 0x40, 0x04, 0x10, 0x8c, 0x04, 0xc4, 0x48, 0x10, 0x44, 0x40, 0x1e, 0x10, 
    0x94, 0x02, 0x44, 0x49, 0x10, 0x24, 0x20, 0x30, 0xf8, 0x92, 0x62, 0x24, 0xe9, 0xfb, 0x1e, 0x10, 
    0x20, 0x08, 0x52, 0x42, 0x22, 0x25, 0x08, 0x12, 0x08, 0x20, 0x08, 0x62, 0x42, 0x22, 0x26, 0x08, 
    0x12, 0x04, 0x10, 0x08, 0x62, 0x46, 0x22, 0x26, 0x08, 0x32, 0x3e, 0x0e, 0x78, 0x62, 0x3c, 0x27, 
    0xe6, 0x79, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char UI_URL [] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xaf, 0xbf, 0xff, 0xfe, 0xdf, 0xff, 0xf7, 0xfb, 
	0xab, 0xfe, 0xf4, 0xd6, 0xff, 0x69, 0xff, 0xf9, 0xaf, 0xfb, 0xbd, 0xfe, 0xef, 0xd9, 0xfd, 0xfb, 
	0xaf, 0xef, 0xaf, 0xfa, 0xef, 0xf0, 0xbe, 0xf7, 0xff, 0xef, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

//首页
void Draw_Home(ui_t *ui)
{
    int16_t y = 128;
    uint8_t state = 0;
    uint8_t color = 1;
    while (1)
    {
        if(indevScan() == UI_ACTION_ENTER)return;

        Disp_ClearBuffer();
        Disp_SetDrawColor(&color);
        switch (state)
        {
        case 0:
            ui->dialog.nowWide = (uint16_t)easeInOutCirc(ui->dialog.times, 0, 30, 20);
            ui->dialog.times++;
            if(ui->dialog.times == 20)
            {
                state = 1;
            }
            break;
        case 1:
            ui->dialog.times = 0;
            y = (int16_t )UI_Animation(26.0, (float )y, &ui->animation.textPage_ani);
            Disp_DrawXBMP(34, y, 72, 12, UI_NAME_LOGO);
            if(y == 26)
            {
                state = 2;
                y = 0;
            }
            break;
        case 2:
            y = (int16_t )UI_Animation(14.0, (float )y, &ui->animation.textPage_ani);
            Disp_DrawXBMP(34, 26, 72, 12, UI_NAME_LOGO);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(y, 58, VERSION_PROJECT_LINK);
            if(y == 12)
            {
                state = 3;
                y = 128;
            }
            break;
        case 3:
            y = (int16_t )UI_Animation(0.0, (float )y, &ui->animation.textPage_ani);
            Disp_DrawXBMP(34, 26, 72, 12, UI_NAME_LOGO);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(14, 58, VERSION_PROJECT_LINK);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(y, 16, "Version:");
            if(y == 0)
            {
                state = 4;
                y = 128;
            }
            break;
        case 4:
            y = (int16_t )UI_Animation(54.0, (float )y, &ui->animation.textPage_ani);
            Disp_DrawXBMP(34, 26, 72, 12, UI_NAME_LOGO);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(14, 58, VERSION_PROJECT_LINK);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(0, 16, "Version:");
            Disp_DrawStr(y, 16, "0");
            if(y == 54)
            {
                state = 5;
                y = 128;
            }
            break;
        case 5:
            y = (int16_t )UI_Animation(54.0, (float )y, &ui->animation.textPage_ani);
            Disp_DrawXBMP(34, 26, 72, 12, UI_NAME_LOGO);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(14, 58, VERSION_PROJECT_LINK);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(0, 16, "Version:");
            Disp_DrawStr(54, 16, "0");
            Disp_DrawStr(y+6, 16, ".");
            if(y == 54)
            {
                state = 6;
                y = 128;
            }
            break;
        case 6:
            y = (int16_t )UI_Animation(54.0, (float )y, &ui->animation.textPage_ani);
            Disp_DrawXBMP(34, 26, 72, 12, UI_NAME_LOGO);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(14, 58, VERSION_PROJECT_LINK);
            Disp_SetFont(UI_FONT);
            Disp_DrawStr(0, 16, "Version:");
            Disp_DrawStr(54, 16, "0");
            Disp_DrawStr(60, 16, ".");
            Disp_DrawStr(y+12, 16, "1");
            if(y == 54)
            {
                state = 7;
            }
            break;
        case 7:
            return;
        default:
            break;
        }
        Disp_DrawLine(64, 45, 64 - ui->dialog.nowWide, 45);
        Disp_DrawLine(64, 45, 64 + ui->dialog.nowWide, 45);
        Disp_SendBuffer();
        #if ( UI_USE_FREERTOS == 1 )
        vTaskDelay(10 / portTICK_PERIOD_MS);
        #endif
    }
}

const unsigned char AUTHOR [] = {
	0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x90, 
	0x00, 0x00, 0x04, 0x88, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x52, 0x00, 0x00, 0x08, 0x20, 0x80, 0x28, 0x80, 0x20, 0x00, 0x00, 0x44, 0x00, 0x20, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x02, 0x01, 0x20, 0x00, 0x08, 0x02, 0x00, 0x20, 0x00, 0x08, 
	0x02, 0x00, 0x08, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x01, 0x10, 0x20, 0x00, 
	0x08, 0x40, 0x00, 0x00, 0x80, 0x00, 0x00, 0x89, 0x04, 0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x20, 0x10, 0x00, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 
	0x20, 0x00, 0x00, 0x81, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 
	0x00, 0x00, 0x08, 0x02, 0x20, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x80, 0x08, 0x10, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 
	0x40, 0x00, 0x00, 0x00, 0x80, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x08, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 
	0x00, 0x81, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f, 
	0x40, 0x04, 0x22, 0x10, 0x00, 0x75, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xd0, 0x30, 0x00, 0x00, 0x40, 0x00, 0xe4, 0x71, 0x01, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x8b, 0x08, 0x04, 0x00, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 
	0xfe, 0xff, 0x07, 0x00, 0x00, 0x80, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x20, 0xff, 0xff, 0x07, 0x00, 
	0x00, 0x80, 0xff, 0xff, 0x0f, 0x10, 0x00, 0x08, 0xf9, 0xff, 0x1f, 0x00, 0x5a, 0x01, 0xf0, 0xff, 
	0x0f, 0x09, 0xff, 0x01, 0xf1, 0xff, 0x01, 0x00, 0xff, 0x32, 0xf8, 0xff, 0xa1, 0x1e, 0x80, 0x4f, 
	0xf8, 0xff, 0x01, 0x00, 0x81, 0x37, 0xf0, 0xff, 0x01, 0x00, 0x01, 0x7e, 0xf8, 0xff, 0x08, 0x00, 
	0x29, 0x3c, 0xf3, 0xff, 0x02, 0x00, 0x85, 0x80, 0xff, 0xff, 0x06, 0x00, 0x00, 0x80, 0xfb, 0x7f, 
	0x0f, 0x00, 0x00, 0x80, 0xff, 0x7f, 0x47, 0x00
};

void Show_Version(ui_t *ui)
{
    int16_t value = -128;
    uint8_t state = 0;
    while (1)
    {
        if(indevScan() == UI_ACTION_ENTER)return;

        Disp_ClearBuffer();

        switch (state)
        {
        case 0:
            value = (int16_t )UI_Animation(0.0, (float )value, &ui->animation.textPage_ani);
            Disp_DrawXBMP(value, 0, 45, 60, AUTHOR);
            if(value == 0)
            {
                state = 1;
                value = 128;
            }
            break;
        case 1:
            value = (int16_t )UI_Animation(50, (float )value, &ui->animation.textPage_ani);
            Disp_DrawXBMP(0, 0, 45, 60, AUTHOR);
            Disp_SetFont(font_home_h6w4);
            Disp_DrawStr(value, 6, "Author:YJY & CYY & TQS");
            Disp_DrawStr(value, 18, "UI:MiaoUI");
            Disp_DrawStr(value, 30, "Version:0.1");
            Disp_DrawStr(value, 42, "OS:FreeRTOS v10.3");
            Disp_DrawStr(value, 56, "Hardware:ESP32S3");
            break;
        default:
            break;
        }
        Disp_SendBuffer();

        #if ( UI_USE_FREERTOS == 1 )
        vTaskDelay(10 / portTICK_PERIOD_MS);
        #endif
    }
}

void Show_Logo(ui_t *ui)
{
    Disp_ClearBuffer();
    Disp_DrawXBMP(34, 26, 72, 12, UI_NAME_LOGO);
    Disp_SendBuffer();
}

extern float joint[7];   // BUG: waiting to fix
extern bool pump;

void Show_Joints(ui_t *ui) {

  char joint_tmp[7][20]= {0}; // 缓冲区用于存储浮点数的字符串表示

  
  while (1) {
    
    if (indevScan() == UI_ACTION_ENTER)
      return;

    for (int8_t i = 0; i < 8; i++) {
      sprintf(&joint_tmp[i], "%.2f", joint[i]);
    }

    Disp_ClearBuffer();

    Disp_SetFont(UI_FONT);
    Disp_DrawStr(0, 12, "J1: ");
    Disp_DrawStr(0, 24, "J2: ");
    Disp_DrawStr(0, 36, "J3: ");
    Disp_DrawStr(0, 48, "J4: ");
    Disp_DrawStr(0, 60, "J5: ");
    Disp_DrawStr(64, 12, "J6: ");
    Disp_DrawStr(64, 24, "J7: ");
    Disp_DrawStr(64, 36, "Pump: ");

    for (int8_t i = 0; i < 5; i++) {
        Disp_DrawStr(24, 12*(i+1), joint_tmp[i]);
    }
    for (int8_t i = 5; i < 7; i++) {
        Disp_DrawStr(88, 12*(i-4), joint_tmp[i]);
    }
    Disp_DrawStr(100, 36, pump ? "on" : "off");

    Disp_SendBuffer();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void Move_goals(ui_t *ui)
{
  char joint_tmp[7][20] = { 0 }; // 缓冲区用于存储浮点数的字符串表示


  while (1) {
    if (indevScan() == UI_ACTION_ENTER)
        return;

    for (int8_t i = 0; i < 8; i++) {
        sprintf(&joint_tmp[i], "%.2f", joint[i]);
    }

    Disp_ClearBuffer();

    

    Disp_SendBuffer();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// void Teach(ui_t *ui)
// {
//   char joint_tmp[7][20] = { 0 }; // 缓冲区用于存储浮点数的字符串表示


//   while (1) {
//     if (indevScan() == UI_ACTION_ENTER)
//         return;

//     for (int8_t i = 0; i < 8; i++) {
//         sprintf(&joint_tmp[i], "%.2f", joint[i]);
//     }

//     Disp_ClearBuffer();

//     Disp_SetFont(UI_FONT);
//     Disp_DrawStr(0, 12, "J1: ");
//     Disp_DrawStr(0, 24, "J2: ");
//     Disp_DrawStr(0, 36, "J3: ");
//     Disp_DrawStr(0, 48, "J4: ");
//     Disp_DrawStr(0, 60, "J5: ");
//     Disp_DrawStr(64, 12, "J6: ");
//     Disp_DrawStr(64, 24, "J7: ");
//     Disp_DrawStr(64, 36, "Pump: ");

//     for (int8_t i = 0; i < 5; i++) {
//         Disp_DrawStr(24, 12 * (i + 1), joint_tmp[i]);
//     }
//     for (int8_t i = 5; i < 7; i++) {
//         Disp_DrawStr(88, 12 * (i - 4), joint_tmp[i]);
//     }
//     Disp_DrawStr(100, 36, pump ? "on" : "off");

//     Disp_SendBuffer();
//     vTaskDelay(10 / portTICK_PERIOD_MS);
//   }
// }

#if ( UI_USE_FREERTOS == 1 )
void TaskLvel_Setting(ui_t *ui)
{
    vTaskPrioritySet(*ui->nowItem->element->data->dataRootTask, *(UBaseType_t *)ui->nowItem->element->data->ptr);
}
#endif
