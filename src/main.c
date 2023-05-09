#ifndef UNIT_TEST

#include <inttypes.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "utils.h"

#define MAX_STACK_SIZE 32768

int main(void) {
    bm_decls;
    stack_usage_scan_init(MAX_STACK_SIZE);
    utils_init();

    while (1) {
        printf("stack_usage sample_last = %" PRIu32 " sample_max = %" PRIu32 " scan = %" PRIu32 "\n",
               stack_usage_sample_get_last(), stack_usage_sample_get_max(), stack_usage_scan());

        __disable_irq();
        bm_start();

        LED_toggle();

        bm_end();
        __enable_irq();

        HAL_Delay(1000);

        printf("cycles = %" PRIu32 "\n", bm_result());
    }
}

#endif  // UNIT_TEST
