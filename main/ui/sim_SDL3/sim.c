#include <stdio.h>
#include <stdlib.h>
#include "./lib/MiaoUI/core/ui.h"
#include "./lib/MiaoUI/display/dispDriver.h"
#include "./lib/MiaoUI/ui_conf.h"
#include "./sim.h"

ui_t ui;
int Wave_TestData;

int main(void)
{
    diapInit();
    MiaoUi_Setup(&ui);
    // test();
    for (;;) {
        ui_loop(&ui);
    }
    return 0;
}
