#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "utils.h"

void LED_Init();

int main(void) {
    bm_decls;
    utils_init();

    while (1) {
        bm_start();

        LED_toggle();
        HAL_Delay(1000);

        bm_end();

        printf("cycles = %lu\n", bm_result());
    }
}
