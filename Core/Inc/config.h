/*
 * config.h
 *
 *  Created on: 24 May 2021
 *      Author: Michal Makowka
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <stdio.h>
#include <string.h>
#include "stm32f405xx.h"

volatile uint16_t delay;


void delay_ms(uint16_t ms);

void SystemRegisterCFG(void);				// Registers configuration
void serverOffResponse(uint8_t sck_nbr);	// Callback function registered for a socket close response
void serverStartResponse(uint8_t sck_nbr);	// Callback function registered for a socket open response

void dataPacketReceived(void);				// Callback function executed when data packet is received

#endif /* INC_CONFIG_H_ */
