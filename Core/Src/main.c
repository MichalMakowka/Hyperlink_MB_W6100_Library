/*
 * main.c
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
#include <stdio.h>
#include <string.h>
#include "stm32f405xx.h"
#include "config.h"
#include "w6100.h"
#include "can.h"
#include "control_sys.h"


int main(void)
{

	SystemRegisterCFG();
	canVariables(canMessages);

	registerSocketCloseCallback(serverOffResponse);		// Register callback function for server close
	registerSocketOpenCallback(serverStartResponse);	// Register callback function for server start (socket connected)
	registerDataReceivedCallback(dataPacketReceived);	// Register callback function when data packets received

	GPIOC->ODR &= ~GPIO_ODR_OD9;						// Server RED LED ON

	W6100_INIT();										// Initialise W6100 with basic network information

	/* Socket Configuration */
	uint8_t txTotalSize = 0;
	uint8_t rxTotalSize = 0;

	for (uint8_t i=0; i<7; i++) {
		SPI_W6100_WSOCK(Sn_TX_BSR, 0x04, i, REG);		// assign 4 Kbytes TX buffer per SOCKET
		SPI_W6100_WSOCK(Sn_RX_BSR, 0x04, i, REG);		// assign 4 Kbytes RX buffer per SOCKET
		txTotalSize += 0x04;
		rxTotalSize += 0x04;
	}

	destination_adr = W6100_OpenTCPSocket(0, 5000);		// Open TCP socket 0 on port 5000 and return its destination address


	CanInit();

	Can_Set_Filter(0x03, STANDARD_FORMAT);
	Can_Set_Filter(0x04, STANDARD_FORMAT);


	/* MAIN Loop */
	while (1) {



  }
}


