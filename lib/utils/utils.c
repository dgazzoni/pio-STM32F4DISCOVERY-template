#include "utils.h"

#include <errno.h>
#include <unistd.h>

#include "stm32f4xx_hal.h"

int _write(int file, char *data, int len) {
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }

    for (int i = 0; i < len; i++) {
        ITM_SendChar(data[i]);
    }

    return len;
}

static void BM_Init(void) {
    // Enable TRCENA
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // Enable DWT_CYCCNT in DWT_CTRL
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static void LED_Init() {
    LED_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void LED_toggle(void) {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
}

void SysTick_Handler(void) {
    HAL_IncTick();
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    // Enable Power Control clock
    __HAL_RCC_PWR_CLK_ENABLE();

    // The voltage scaling allows optimizing the power consumption when the device is
    // clocked below the maximum system frequency, to update the voltage scaling value
    // regarding system frequency refer to product datasheet.
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    // Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;      // External 8 MHz xtal on OSC_IN/OSC_OUT
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;  // 8 MHz / 8 * 336 / 2 = 168 MHz
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;              // VCO input clock = 1 MHz / PLLM = 1 MHz
    RCC_OscInitStruct.PLL.PLLN = 192;            // VCO output clock = VCO input clock * PLLN = 192 MHz
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV8;  // PLLCLK = VCO output clock / PLLP = 24 MHz
    RCC_OscInitStruct.PLL.PLLQ = 4;              // USB clock = VCO output clock / PLLQ = 48 MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        while (1)
            ;
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  // 24 MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         // 24 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;          // 24 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;          // 24 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        while (1)
            ;
    }
}

void utils_init(void) {
    HAL_Init();

    SystemClock_Config();

    BM_Init();
    LED_Init();
}