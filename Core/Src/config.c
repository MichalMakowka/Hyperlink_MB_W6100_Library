/*
 * config.c
 *
 *  Created on: 24 May 2021
 *      Author: Michal Makowka
 */
#include "config.h"


void SystemRegisterCFG(void) {
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
//	GPIOC->ODR ^= GPIO_ODR_OD8;
}





















