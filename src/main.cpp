#include "stm32f7xx_hal.h"

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    // Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;  // This value depends on the frequency of the external crystal
    RCC_OscInitStruct.PLL.PLLN = 432; // PLL multiplier: these values are for 216 MHz SysClk
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;   // For USB clock of 48 MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        // Error_Handler(); // Implement this function as needed
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | 
                                   RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;    // No division of HCLK (216 MHz)
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;     // PCLK1 (54 MHz)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;     // PCLK2 (108 MHz)
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
        // Error_Handler(); // Implement this function as needed
    }

    // Enable overdrive mode to reach 216 MHz (for STM32F7 series only)
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        // Error_Handler(); // Implement this function as needed
    }
}

void GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable GPIO Clocks
  __HAL_RCC_GPIOG_CLK_ENABLE();  // Assuming the LED is on GPIOG

  // Configure GPIO pin for the LED
  GPIO_InitStruct.Pin = GPIO_PIN_0;  // Change pin as necessary
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

int main(void) {
  HAL_Init();
  SystemClock_Config();
  GPIO_Init();

  while (1) {
    // Toggle LED
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_0);
    // Debounce delay
    HAL_Delay(100);
    // Wait for button release to avoid repeated toggling
  }
}
