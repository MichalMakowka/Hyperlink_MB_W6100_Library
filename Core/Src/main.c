/*
 * This is W6100 Hyperlink Motherboard Ethernet library.
 * Designed by Michal Makowka
 *
 * (C) All rights reserved.
 *
 * v 1.0
 */
#include <stdio.h>
#include <string.h>
#include "stm32f405xx.h"
#include "config.h"
#include "w6100.h"




int main(void)
{

	SystemRegisterCFG();

	GPIOC->ODR &= ~GPIO_ODR_OD9;						// Server RED LED ON

	uint8_t rx_dat[20];
	memset(rx_dat, '0', sizeof(rx_dat));

	uint32_t destination_adr;


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

	destination_adr = W6100_OpenTCPSocket(0);			// Open TCP socket 0 and return its destination address


	while (1) {

		if (W6100_ReceiveData(0, destination_adr, rx_dat)) {		// Check if data arrived
			if (rx_dat[0] == 'A' && rx_dat[1] == 'B') { GPIOC->ODR &= ~GPIO_ODR_OD12; }
			else if (rx_dat[0] == 'C' && rx_dat[1] == 'D' && rx_dat[2] == 'F')	{ GPIOC->ODR |= GPIO_ODR_OD12; }
			memset(rx_dat, '0', sizeof(rx_dat));
		}


  }

}

























