/*
 * control_sys.h
 *
 *  Created on: 3 Jan 2022
 *      Author: Michal Makowka
 */

#ifndef INC_CONTROL_SYS_H_
#define INC_CONTROL_SYS_H_

#include <stdio.h>
#include <string.h>
#include "stm32f405xx.h"
#include "config.h"
#include "w6100.h"
#include "can.h"

// Ethernet TCP/IP auxiliary variables
char rx_dat[40];	// Ethernet socket buffer
uint32_t destination_adr;		// Socket destination address


CAN_MESSAGE canMessages[2];

void canVariables(CAN_MESSAGE * cmg);
void dataPacketReceived(void);				// Callback function executed when data packet is received

#endif /* INC_CONTROL_SYS_H_ */
