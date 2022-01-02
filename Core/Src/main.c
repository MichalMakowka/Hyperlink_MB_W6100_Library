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



int main(void)
{

	SystemRegisterCFG();

	GPIOC->ODR &= ~GPIO_ODR_OD9;						// Server RED LED ON

	// Ethernet TCP/IP auxiliary variables
	char rx_dat[20];	// Ethernet socket buffer
	uint32_t destination_adr;		// Socket destination address

	// Server responses
	uint8_t on_message[20] = {"System Enabled\n"};
	uint8_t off_message[20] = {"System Disabled\n"};


	W6100_INIT();										// Initialise W6100 with basic network information

	/* Socket Configuration */
	uint8_t txTotalSize = 0;
	uint8_t rxTotalSize = 0;

	for (uint8_t i=0; i<7; i++) {
		SPI_W6100_WSOCK(Sn_TX_BSR, 0x02, i, REG);		// assign 2 Kbytes TX buffer per SOCKET
		SPI_W6100_WSOCK(Sn_RX_BSR, 0x02, i, REG);		// assign 2 Kbytes RX buffer per SOCKET
		txTotalSize += 0x02;
		rxTotalSize += 0x02;
	}

	destination_adr = W6100_OpenTCPSocket(0, 5000);		// Open TCP socket 0 on port 5000 and return its destination address


	CAN_MESSAGE can_on_msg;
	strcpy(can_on_msg.data, "mb_0000");
	can_on_msg.format = STANDARD_FORMAT;
	can_on_msg.type = DATA_FRAME;
	can_on_msg.len = sizeof(can_on_msg);
	can_on_msg.id = 0x01;

	CAN_MESSAGE can_off_msg;
	strcpy(can_off_msg.data, "mb_0001");
	can_off_msg.format = STANDARD_FORMAT;
	can_off_msg.type = DATA_FRAME;
	can_off_msg.len = sizeof(can_off_msg);
	can_off_msg.id = 0x02;

	CanInit();

	Can_Set_Filter(0x03, STANDARD_FORMAT);
	Can_Set_Filter(0x04, STANDARD_FORMAT);



	while (1) {

		// Check Ethernet
		if (W6100_ReceiveData(0, destination_adr, (uint8_t*)rx_dat, sizeof(rx_dat))) {		// Check if data arrived
			if (!strcmp(rx_dat, "on\n")) {
				GPIOC->ODR &= ~GPIO_ODR_OD12;
				// Send msg to the client
				W6100_TransmitData(0, destination_adr, on_message, sizeof(on_message));
				Can_Tx_Msg(&can_on_msg);

			}
			else if (!strcmp(rx_dat, "off\n"))	{
				GPIOC->ODR |= GPIO_ODR_OD12;
				// Send msg to the client
				W6100_TransmitData(0, destination_adr, off_message, sizeof(off_message));
				// Send CAN frame
				Can_Tx_Msg(&can_off_msg);
			}
		}

  }

}

























