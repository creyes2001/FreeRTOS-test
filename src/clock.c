#include "stm32f446xx.h"
#include "clock.h"

void SysClk_Config(void){
// 1. Enable HSE and wait for it to be ready
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    // 2. Enable power controller and set voltage scaling (Scale 1 = max performance)
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;  // Voltage Scale 1

    // 3. Configure Flash latency (5 WS required for 180 MHz at 3.3V)
    FLASH->ACR = FLASH_ACR_LATENCY_5WS
               | FLASH_ACR_PRFTEN
               | FLASH_ACR_ICEN
               | FLASH_ACR_DCEN;

    // 4. Configure PLL: HSE=8MHz, M=8, N=360, P=2, Q=7
    RCC->PLLCFGR = (8  << RCC_PLLCFGR_PLLM_Pos)   // M = 8
                 | (360 << RCC_PLLCFGR_PLLN_Pos)   // N = 360
                 | (0  << RCC_PLLCFGR_PLLP_Pos)    // P = 2 (0b00 = /2)
                 | RCC_PLLCFGR_PLLSRC_HSE           // Source = HSE
                 | (7  << RCC_PLLCFGR_PLLQ_Pos);   // Q = 7

    // 5. Enable PLL and wait
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // 6. Configure bus prescalers
    RCC->CFGR = RCC_CFGR_HPRE_DIV1    // AHB  = 180 MHz
              | RCC_CFGR_PPRE1_DIV4   // APB1 =  45 MHz (max 45 MHz)
              | RCC_CFGR_PPRE2_DIV2;  // APB2 =  90 MHz (max 90 MHz)

    // 7. Switch SYSCLK to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

}
