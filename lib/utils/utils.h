#include <stdint.h>

#include "stm32f4xx_hal.h"

#define LED_PIN GPIO_PIN_12
#define LED_GPIO_PORT GPIOD
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

void utils_init(void);
void LED_toggle(void);
void stack_usage_scan_init(uint32_t max_stack_size);
uint32_t stack_usage_scan(void);

#define bm_decls uint32_t bm_s, bm_e
#define bm_start() bm_s = DWT->CYCCNT
#define bm_end() bm_e = DWT->CYCCNT
#define bm_result() (bm_e - bm_s)

extern volatile uint32_t stack_usage_sample_current, stack_usage_sample_max;

#define stack_usage_sample_get_last() stack_usage_sample_current
#define stack_usage_sample_get_max() stack_usage_sample_max
