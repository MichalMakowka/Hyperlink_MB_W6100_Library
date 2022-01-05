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

// *** Update this variable with a number of CAN message types which can be TRANSMITTED ***
#define CAN_MSG_CNT 2


// Ethernet TCP/IP auxiliary variables
//char rx_dat[40];				// Ethernet socket buffer


CAN_MESSAGE canMessages[CAN_MSG_CNT];

void canVariables(CAN_MESSAGE * cmg);
void dataPacketReceived(char * RxBuf);	// Callback function executed when data packet is received
void canMessageReceived(CAN_MESSAGE msg);	// Callback function executed when CAN message is received

#endif /* INC_CONTROL_SYS_H_ */
