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

void SPI_Eth_SS(uint8_t state) {
	if (state) {
		GPIOA->ODR &= ~GPIO_ODR_OD4;
	}
	else if (!state) {
		GPIOA->ODR |= GPIO_ODR_OD4;
	}
}

uint8_t SPI_Eth_RT(uint8_t data) {
	while(!(SPI1->SR & SPI_SR_TXE));	// Wait for Tx buffer empty
	SPI1->DR = data;
	while(!(SPI1->SR & SPI_SR_RXNE));	// Wait for Rx buffer not empty
	data = SPI1->DR;
	return data;
}

uint8_t SPI_W6100_RCR(uint16_t adr) {
	// See Page 76 - W6100 datasheet
	uint8_t dat;
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(0x00);		// Send Control Byte	[CR, Read, Variable Length Data Mode]
	dat = SPI_Eth_RT(0x00);	// Send garbage data to read the Common Register
	SPI_Eth_SS(OFF);		// NSS Slave Disable
	return dat;
}

void SPI_W6100_WCR(uint16_t adr, uint8_t val) {
	// See Page 76 - W6100 datasheet
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(0x04);		// Send Control Byte	[CR, Write, Variable Length Data Mode]
	SPI_Eth_RT(val);		// Send val to be written in the register
	SPI_Eth_SS(OFF);
}

uint8_t SPI_W6100_RSOCK(uint16_t adr, uint8_t socket_nbr, uint8_t block) {
	uint8_t dat;
	uint8_t cb_temp = 0x00;
	cb_temp |= (socket_nbr << 5);
	cb_temp |= (block << 3);
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(cb_temp);	// Send Control Byte
	dat = SPI_Eth_RT(0x00);		// Send garbage data to read the Common Register
	SPI_Eth_SS(OFF);
	return dat;
}

void SPI_W6100_WSOCK(uint16_t adr, uint8_t val, uint8_t socket_nbr, uint8_t block) {
	uint8_t cb_temp = 0x04;
	cb_temp |= (socket_nbr << 5);
	cb_temp |= (block << 3);
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(cb_temp);	// Send Control Byte
	SPI_Eth_RT(val);		// Send val to be written in the register
	SPI_Eth_SS(OFF);
}





__attribute__((interrupt)) void SysTick_Handler(void){
//	GPIOC->ODR ^= GPIO_ODR_OD8;
}





















