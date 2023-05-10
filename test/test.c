#include <unity.h>

#include "stm32f4xx_hal.h"
#include "utils.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void callback_1ms(void) {
    if (HAL_GetTick() % 512 == 0) {
        LED_Toggle();
    }
}

void passing_test(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void failing_test(void) {
    TEST_ASSERT_EQUAL(2 + 2, 5);
}

int main(int argc, char **argv) {
    utils_init();

    UNITY_BEGIN();
    RUN_TEST(passing_test);
    RUN_TEST(failing_test);
    UNITY_END();

    while (1) {
    }
}
