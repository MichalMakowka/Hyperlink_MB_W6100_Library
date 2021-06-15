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

	GPIOC->ODR &= ~GPIO_ODR_OD9;

	uint8_t rx_dat[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t i;
	uint32_t destination_adr;
	uint32_t get_size, gSn_RX_MAX, get_start_address;
	uint32_t Sn_RX_RD_temp;


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

		// If data received
		while ((SPI_W6100_RSOCK(Sn_IR, 0, REG) & 0b00000100) != 0x04);	// Wait for data
		SPI_W6100_WSOCK(Sn_IRCLR, 0x04, 0, REG);	// Clear data interrupt

		// Read data from the buffer
		get_size = (SPI_W6100_RSOCK(Sn_RX_RSR0, 0, REG) << 8);
		get_size |= SPI_W6100_RSOCK(Sn_RX_RSR1, 0, REG);
		gSn_RX_MAX = (SPI_W6100_RSOCK(Sn_RX_BSR, 0, REG) * 1024);
		get_start_address = (SPI_W6100_RSOCK(Sn_RX_RD0, 0, REG) << 8);
		get_start_address |= SPI_W6100_RSOCK(Sn_RX_RD1, 0, REG);

		for (i=0; i<get_size; i++) {
			rx_dat[i] = SPI_W6100_RSOCK((get_start_address+i), 0, RX_BUF);
		}
		if (rx_dat[0] == 'O' && rx_dat[1] == 'N') { GPIOC->ODR &= ~GPIO_ODR_OD12; }
		else if (rx_dat[0] == 'O' && rx_dat[1] == 'F' && rx_dat[2] == 'F')	{ GPIOC->ODR |= GPIO_ODR_OD12; }
		memcpy(&get_start_address, &destination_adr, get_size);

		Sn_RX_RD_temp = ((SPI_W6100_RSOCK(Sn_RX_RD0, 0, REG) << 8));
		Sn_RX_RD_temp |= SPI_W6100_RSOCK(Sn_RX_RD1, 0, REG);
		Sn_RX_RD_temp += get_size;
		SPI_W6100_WSOCK(Sn_RX_RD0, (Sn_RX_RD_temp>>8), 0, REG);
		SPI_W6100_WSOCK(Sn_RX_RD1, (Sn_RX_RD_temp), 0, REG);
		SPI_W6100_WSOCK(Sn_CR, 0x40, 0, REG);
		while((SPI_W6100_RSOCK(Sn_CR, 0, REG)) != 0x00);
		memset(rx_dat, '0', sizeof(rx_dat));

  }

}

























