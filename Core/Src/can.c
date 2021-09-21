/*
 * can.c
 *
 *  Created on: 21 Sep 2021
 *      Author: Michal Makowka
 */

#include "can.h"


void CanInit(void) {

	// *** Pin configuration and remap assignment *** //
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;		// Enable CAN 1 CLK

	GPIOB->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;	// PB8 and PB9 Alternative Function
	GPIOB->AFR[1] |= GPIO_AFRH_AFRH0_3 | GPIO_AFRH_AFRH0_0 | GPIO_AFRH_AFRH1_0 | GPIO_AFRH_AFRH1_3;		// Alternative function mapped to CAN1_Tx and CAN1_Rx

}
