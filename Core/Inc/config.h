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

// Interrupt Priority Group Definitions
#define PRIGROUP_16G_0S ((const uint32_t) 0x03)
#define PRIGROUP_8G_2S ((const uint32_t) 0x04)
#define PRIGROUP_4G_4S ((const uint32_t) 0x05)
#define PRIGROUP_2G_8S ((const uint32_t) 0x06)
#define PRIGROUP_0G_16S ((const uint32_t) 0x07)


void SystemRegisterCFG(void);				// Registers configuration
void serverOffResponse(uint8_t sck_nbr);	// Callback function registered for a socket close response
void serverStartResponse(uint8_t sck_nbr);	// Callback function registered for a socket open response


#endif /* INC_CONFIG_H_ */
