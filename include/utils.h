#include "stm32f4xx_hal.h"

#define LED_PIN GPIO_PIN_12
#define LED_GPIO_PORT GPIOD
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

void utils_init(void);
void LED_toggle(void);

#define bm_decls uint32_t bm_s, bm_e
#define bm_start() bm_s = DWT->CYCCNT
#define bm_end() bm_e = DWT->CYCCNT
#define bm_result() (bm_e - bm_s)
