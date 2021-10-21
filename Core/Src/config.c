/*
 * config.c
 *
 *	Hyperlink W6100 Ethernet Library
 *
 *  Created on: 15 Jun 2021
 *  Author: Michal Makowka
 *
 *  v1.0
 *
 *  (C) All rights reserved.
 *
 *  https://mmttechnologies.com/
 */
#include "config.h"


void SystemRegisterCFG(void) {

	// *** Configure System Clock (36MHz for each system BUS) ***
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;		// HSE as an input to the PLL loop
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP_1;		// PLLP DIV = /2
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP_0;
	RCC->PLLCFGR |= (0b001001000 << 6);			// Multiplication factor PLLN = 72
	RCC->PLLCFGR |= (0b000100);					// PLLM DIV = /4
	RCC->CFGR |= RCC_CFGR_SW_PLL;				// System Clock MUX switched to PLL
	RCC->CFGR |= (RCC_CFGR_HPRE_DIV2 << RCC_CFGR_HPRE_Pos);		// AHB DIV = /2
	RCC->CR |= RCC_CR_HSEON;	// HSE ON
	while (!(RCC->CR & RCC_CR_HSERDY)); // Wait for HSE ON
	RCC->CR |= RCC_CR_PLLON;	// PLL ON
	while ((RCC->CR & RCC_CR_PLLRDY)); // Check if PLL not locked
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// Check for a correct source set
	RCC->CR &= ~RCC_CR_HSION;	// 16MHz HSI OFF


	// RCC Configuration
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;	// GPIO Clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;		// SPI1 clock

	// Standard GPIO Config
	GPIOC->MODER |= GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0 | GPIO_MODER_MODE11_0 | GPIO_MODER_MODE12_0;	// STM LEDs output
	GPIOC->ODR |= GPIO_ODR_OD8 | GPIO_ODR_OD9 | GPIO_ODR_OD11 | GPIO_ODR_OD12;		// STM LEDs OFF

	// W6100 External Reset config
	GPIOC->MODER |= GPIO_MODER_MODE4_0;		// Output
	GPIOC->ODR |= GPIO_ODR_OD4;				// Level high (reset disable)

	// SPI GPIO Config
	GPIOA->MODER |= GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;		// SPI1 AFIO Set (NSS Software)
	GPIOA->ODR |= GPIO_ODR_OD4;		// Slave select HIGH (disable)
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0 | GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL6_0 |
					 GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL7_0 | GPIO_AFRL_AFRL7_2;			// SPI1 AFIO => SPI1

	// SPI Config
	SPI1->CR1 |= SPI_CR1_SPE | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_MSTR;


	// SysTic Config
	SysTick_Config(16000000);
}





__attribute__((interrupt)) void SysTick_Handler(void){
//	GPIOC->ODR ^= GPIO_ODR_OD11;
}





















