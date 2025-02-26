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
#include "ui_conf.h"
#include "core/ui.h"
#include "./display/dispDriver.h"
#include "images/image.h"
#include "widgets/custom.h"
#include "version.h"

/*Page*/
ui_page_t Home_Page, System_Page,Money_Page;
/*item */
ui_item_t HomeHead_Item, SystemHead_Item, System_Item, Image_Item, Github_Item, Bilibili_Item, Version_Item, Wave_Item, Money_Item;
ui_item_t SystemHeap_Item;
ui_item_t Contrast_Item, Power_Item, MenuColor_Item, CursorAni_Item, Test_Item, J1_Item;

/**
 * 在此建立所需显示或更改的数据
 * 无参数
 * 无返回值
 */
void Create_Parameter(ui_t *ui)
{
    static int Contrast = 255;
    static ui_data_t Contrast_data;
    Contrast_data.name = "Contrast";
    Contrast_data.ptr = &Contrast;
    Contrast_data.function = Disp_SetContrast;
    Contrast_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Contrast_data.dataType = UI_DATA_INT;
    Contrast_data.actionType = UI_DATA_ACTION_RW;
    Contrast_data.max = 255;
    Contrast_data.min = 0;
    Contrast_data.step = 5;
    static ui_element_t Contrast_element;
    Contrast_element.data = &Contrast_data;
    Create_element(&Contrast_Item, &Contrast_element);

    static uint8_t power_off = false;
    static ui_data_t Power_switch_data;
    Power_switch_data.ptr = &power_off;
    Power_switch_data.function = Disp_SetPowerSave;
    Power_switch_data.dataType = UI_DATA_SWITCH;
    Power_switch_data.actionType = UI_DATA_ACTION_RW;
    static ui_element_t Power_element;
    Power_element.data = &Power_switch_data;
    Create_element(&Power_Item, &Power_element);

    #if ( UI_USE_FREERTOS == 1 )
    extern QueueHandle_t WaveTest_Queue;
    extern TaskHandle_t WaveTestTask_Handle;
    #endif
    extern int Wave_TestData;
    static ui_data_t Wave_data;
    Wave_data.name = "Wave";
    Wave_data.ptr = &Wave_TestData;
    Wave_data.dataType = UI_DATA_INT;
    Wave_data.max = 600;
    Wave_data.min = 0;
    #if ( UI_USE_FREERTOS == 1 )
    Wave_data.dataRootTask = &WaveTestTask_Handle;
    Wave_data.dataRootQueue = &WaveTest_Queue;
    #endif
    static ui_element_t Wave_element;
    Wave_element.data = &Wave_data;
    Create_element(&Wave_Item, &Wave_element);

    #if ( UI_USE_FREERTOS == 1 )
    extern char FreeHeap[20];
    static ui_data_t SystemHeap_data;
    SystemHeap_data.name = "FreeHeap";
    SystemHeap_data.ptr = FreeHeap;
    SystemHeap_data.dataType = UI_DATA_STRING;
    SystemHeap_data.actionType = UI_DATA_ACTION_RO;
    static ui_element_t SystemHeap_element;
    SystemHeap_element.data = &SystemHeap_data;
    Create_element(&SystemHeap_Item, &SystemHeap_element);
    #endif

    static ui_data_t MenuColor_data;
    MenuColor_data.name = "MenuColor";
    MenuColor_data.ptr = &ui->bgColor;
    MenuColor_data.dataType = UI_DATA_SWITCH;
    MenuColor_data.actionType = UI_DATA_ACTION_RW;
    static ui_element_t MenuColor_element;
    MenuColor_element.data = &MenuColor_data;
    Create_element(&MenuColor_Item, &MenuColor_element);

    static ui_data_t CursorAni_data;
    CursorAni_data.name = "CursorAni";
    CursorAni_data.ptr = &ui->animation.cursor_ani.kp;
    CursorAni_data.dataType = UI_DATA_FLOAT;
    CursorAni_data.actionType = UI_DATA_ACTION_RW;
    CursorAni_data.min = 0;
    CursorAni_data.max = 10;
    CursorAni_data.step = 0.1;
    static ui_element_t CursorAni_element;
    CursorAni_element.data = &CursorAni_data;
    Create_element(&CursorAni_Item, &CursorAni_element);
}

/**
 * 在此建立所需显示或更改的文本
 * 无参数
 * 无返回值
 */
void Create_Text(ui_t *ui)
{
    static ui_text_t github_text;
    github_text.font = UI_FONT;
    github_text.fontHight = UI_FONT_HIGHT;
    github_text.fontWidth = UI_FONT_WIDTH;
    github_text.ptr = VERSION_PROJECT_LINK;
    static ui_element_t github_element;
    github_element.text = &github_text;
    Create_element(&Github_Item, &github_element);

    static ui_text_t bilibili_text;
    bilibili_text.font = UI_FONT;
    bilibili_text.fontHight = UI_FONT_HIGHT;
    bilibili_text.fontWidth = UI_FONT_WIDTH;
    bilibili_text.ptr = "Could you trouble to give me a star? Thank you! Thank you! Thank you!";
    static ui_element_t bilibili_element;
    bilibili_element.text = &bilibili_text;
    Create_element(&Bilibili_Item, &bilibili_element);
}

/*
 * 菜单构建函数
 * 该函数不接受参数，也不返回任何值。
 * 功能：静态地构建一个菜单系统。
 */
void Create_MenuTree(ui_t *ui)
{
    AddPage("[HomePage]", &Home_Page, UI_PAGE_ICON);
        AddItem("[HomePage]", UI_ITEM_ONCE_FUNCTION, logo_allArray[0], &HomeHead_Item, &Home_Page, NULL, Draw_Home);
        AddItem(" [Exchange]", UI_ITEM_ONCE_FUNCTION, logo_allArray[7], &Money_Item, &Home_Page, NULL, Show_Joints);
        AddItem(" [System]", UI_ITEM_PARENTS, logo_allArray[1], &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &System_Page, UI_PAGE_TEXT);
                AddItem("[System]", UI_ITEM_RETURN, NULL, &SystemHead_Item, &System_Page, &Home_Page, NULL);
                AddItem(" -Menu Color", UI_ITEM_DATA, NULL, &MenuColor_Item, &System_Page, NULL, NULL);   
                AddItem(" -Cursor Ani", UI_ITEM_DATA, NULL, &CursorAni_Item, &System_Page, NULL, NULL);
        // AddItem(" -Image", UI_ITEM_LOOP_FUNCTION, logo_allArray[2], &Image_Item, &Home_Page, NULL, Show_Logo);
        // AddItem(" -Github", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Home_Page, NULL, NULL);
        AddItem(" [Version]", UI_ITEM_ONCE_FUNCTION, logo_allArray[5], &Version_Item, &Home_Page, NULL, Show_Version);
        // AddItem(" -Wave", UI_ITEM_WAVE, logo_allArray[6], &Wave_Item, &Home_Page, NULL, NULL);
}

void MiaoUi_Setup(ui_t *ui)
{
  Create_UI(ui, &Money_Item); // 创建UI, 必须给定一个头项目
  Draw_Home(ui);
}