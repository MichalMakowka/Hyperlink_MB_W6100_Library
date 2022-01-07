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

	SystemRegisterCFG();			// Setup basic peripheral registers
	canVariables(canMessages);		// Initialises CAN msg structures for the communication

	registerSocketCloseCallback(serverOffResponse);		// Register callback function for server close
	registerSocketOpenCallback(serverStartResponse);	// Register callback function for server start (socket connected)
	registerDataReceivedCallback(dataPacketReceived);	// Register callback function when data packets received
	registerCanMsgRxCallback(canMessageReceived);

	GPIOC->ODR &= ~GPIO_ODR_OD9;						// Server RED LED ON

	W6100_INIT();										// Initialise W6100 with basic network information

	/* Socket 0 Configuration */
	uint8_t txTotalSize = 0;
	uint8_t rxTotalSize = 0;

	for (uint8_t i=0; i<7; i++) {
		SPI_W6100_WSOCK(Sn_TX_BSR, 0x04, i, REG);		// assign 4 Kbytes TX buffer per SOCKET
		SPI_W6100_WSOCK(Sn_RX_BSR, 0x04, i, REG);		// assign 4 Kbytes RX buffer per SOCKET
		txTotalSize += 0x04;
		rxTotalSize += 0x04;
	}

	socket_dest_adr[0] = W6100_OpenTCPSocket(0, 5000);		// Open TCP socket 0 on port 5000 and return its destination address
	socket_dest_adr[1] = W6100_OpenTCPSocket(1, 5010);		// Open TCP socket 1 on port 5000 and return its destination address


	CanInit();

	Can_Set_Filter(0x03, STANDARD_FORMAT);
	Can_Set_Filter(0x04, STANDARD_FORMAT);

	int idxx = 0;
	char buf[15] = "";
	/* MAIN Loop */
	while (1) {

		if (!(GPIOA->IDR & GPIO_IDR_ID3)) {
			GPIOC->ODR &= ~GPIO_ODR_OD11;
			idxx++;
			sprintf(buf, "cnt: %i\n", idxx);
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)buf, sizeof(buf));
			while(!(GPIOA->IDR & GPIO_IDR_ID3));

		} else {
			GPIOC->ODR |= GPIO_ODR_OD11;
		}


  }
}


