/*
 * w6100.c
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
#include "w6100.h"




void SPI_Eth_SS(uint8_t state) {
	if (state) {
		GPIOA->ODR &= ~GPIO_ODR_OD4;
	}
	else if (!state) {
		GPIOA->ODR |= GPIO_ODR_OD4;
	}
}

uint8_t SPI_Eth_RT(uint8_t data) {
	while(!(SPI1->SR & SPI_SR_TXE));	// Wait for Tx buffer empty
	SPI1->DR = data;
	while(!(SPI1->SR & SPI_SR_RXNE));	// Wait for Rx buffer not empty
	data = SPI1->DR;
	return data;
}

uint8_t SPI_W6100_RCR(uint16_t adr) {
	// See Page 76 - W6100 datasheet
	uint8_t dat;
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(0x00);		// Send Control Byte	[CR, Read, Variable Length Data Mode]
	dat = SPI_Eth_RT(0x00);	// Send garbage data to read the Common Register
	SPI_Eth_SS(OFF);		// NSS Slave Disable
	return dat;
}

void SPI_W6100_WCR(uint16_t adr, uint8_t val) {
	/* See Page 76 - W6100 datasheet */
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(0x04);		// Send Control Byte	[CR, Write, Variable Length Data Mode]
	SPI_Eth_RT(val);		// Send val to be written in the register
	SPI_Eth_SS(OFF);
}

uint8_t SPI_W6100_RSOCK(uint16_t adr, uint8_t socket_nbr, uint8_t block) {
	uint8_t dat;
	uint8_t cb_temp = 0x00;
	cb_temp |= (socket_nbr << 5);
	cb_temp |= (block << 3);
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(cb_temp);	// Send Control Byte
	dat = SPI_Eth_RT(0x00);		// Send garbage data to read the Common Register
	SPI_Eth_SS(OFF);
	return dat;
}

void SPI_W6100_WSOCK(uint16_t adr, uint8_t val, uint8_t socket_nbr, uint8_t block) {
	uint8_t cb_temp = 0x04;
	cb_temp |= (socket_nbr << 5);
	cb_temp |= (block << 3);
	SPI_Eth_SS(ON);			// NSS Slave Enable
	SPI_Eth_RT(adr>>8);		// Send upper address half
	SPI_Eth_RT(adr);		// Send lower address half
	SPI_Eth_RT(cb_temp);	// Send Control Byte
	SPI_Eth_RT(val);		// Send val to be written in the register
	SPI_Eth_SS(OFF);
}

void W6100_INIT(void) {
	/* *** W6100 Init *** */
	SPI_W6100_WCR(NETLCKR, 0x3a);	// NETLCKR		Network settings unlock

	SPI_W6100_WCR(SHAR0, MAC0);	// SHAR[5:0]	Set hardware MAC address
	SPI_W6100_WCR(SHAR1, MAC1);
	SPI_W6100_WCR(SHAR2, MAC2);
	SPI_W6100_WCR(SHAR3, MAC3);
	SPI_W6100_WCR(SHAR4, MAC4);
	SPI_W6100_WCR(SHAR5, MAC5);

	SPI_W6100_WCR(GAR0, GIP0);	// GAR[0:3]		Gateway IP address 192.168.0.1
	SPI_W6100_WCR(GAR1, GIP1);
	SPI_W6100_WCR(GAR2, GIP2);
	SPI_W6100_WCR(GAR3, GIP3);

	SPI_W6100_WCR(SUBR0, SBM0);	// SUBR[0:3]	Subnet mask address 255:255:255:0
	SPI_W6100_WCR(SUBR1, SBM1);
	SPI_W6100_WCR(SUBR2, SBM2);
	SPI_W6100_WCR(SUBR3, SBM3);

	SPI_W6100_WCR(SIPR0, IPV0);	// SIPR[0:3]	IPv4 Source IP Address 192.168.0.27
	SPI_W6100_WCR(SIPR1, IPV1);
	SPI_W6100_WCR(SIPR2, IPV2);
	SPI_W6100_WCR(SIPR3, IPV3);

	SPI_W6100_WCR(LLAR0, 0xfe);	// Link Local Address, FE80::1322:33FF:FEAA:BBCC
	SPI_W6100_WCR(LLAR1, 0x80);
	SPI_W6100_WCR(LLAR2, 0x00);
	SPI_W6100_WCR(LLAR3, 0x01);
	SPI_W6100_WCR(LLAR4, 0x00);
	SPI_W6100_WCR(LLAR5, 0x00);
	SPI_W6100_WCR(LLAR6, 0x00);
	SPI_W6100_WCR(LLAR7, 0x00);
	SPI_W6100_WCR(LLAR8, 0x13);
	SPI_W6100_WCR(LLAR9, 0x22);
	SPI_W6100_WCR(LLAR10, 0x33);
	SPI_W6100_WCR(LLAR11, 0xff);
	SPI_W6100_WCR(LLAR12, 0xfe);
	SPI_W6100_WCR(LLAR13, 0xaa);
	SPI_W6100_WCR(LLAR14, 0xbb);
	SPI_W6100_WCR(LLAR15, 0xcc);

	SPI_W6100_WCR(GUAR0, 0x20);	// Global Unicast Address, 2001:0DB8:E001::1222:33FF:FEAA:BBCC
	SPI_W6100_WCR(GUAR1, 0x01);
	SPI_W6100_WCR(GUAR2, 0x0d);
	SPI_W6100_WCR(GUAR3, 0xb8);
	SPI_W6100_WCR(GUAR4, 0xe0);
	SPI_W6100_WCR(GUAR5, 0x01);
	SPI_W6100_WCR(GUAR6, 0x00);
	SPI_W6100_WCR(GUAR7, 0x00);
	SPI_W6100_WCR(GUAR8, 0x13);
	SPI_W6100_WCR(GUAR9, 0x22);
	SPI_W6100_WCR(GUAR10, 0x33);
	SPI_W6100_WCR(GUAR11, 0xff);
	SPI_W6100_WCR(GUAR12, 0xfe);
	SPI_W6100_WCR(GUAR13, 0xaa);
	SPI_W6100_WCR(GUAR14, 0xbb);
	SPI_W6100_WCR(GUAR15, 0xcc);

	SPI_W6100_WCR(SUB6R0, 0xff);	// IPv6 Subnet Prefix, FFFF:FFFF::
	SPI_W6100_WCR(SUB6R1, 0xff);
	SPI_W6100_WCR(SUB6R2, 0xff);
	SPI_W6100_WCR(SUB6R3, 0xff);
	SPI_W6100_WCR(SUB6R4, 0xff);
	SPI_W6100_WCR(SUB6R5, 0xff);
	SPI_W6100_WCR(SUB6R6, 0xff);
	SPI_W6100_WCR(SUB6R7, 0xff);
	SPI_W6100_WCR(SUB6R8, 0x00);
	SPI_W6100_WCR(SUB6R9, 0x00);
	SPI_W6100_WCR(SUB6R10, 0x00);
	SPI_W6100_WCR(SUB6R11, 0x00);
	SPI_W6100_WCR(SUB6R12, 0x00);
	SPI_W6100_WCR(SUB6R13, 0x00);
	SPI_W6100_WCR(SUB6R14, 0x00);
	SPI_W6100_WCR(SUB6R15, 0x00);

	SPI_W6100_WCR(GA6R0, 0xfe);	// IPv6 Gateway Address, FFFF:FFFF::
	SPI_W6100_WCR(GA6R1, 0x80);
	SPI_W6100_WCR(GA6R2, 0x00);
	SPI_W6100_WCR(GA6R3, 0x00);
	SPI_W6100_WCR(GA6R4, 0x00);
	SPI_W6100_WCR(GA6R5, 0x00);
	SPI_W6100_WCR(GA6R6, 0x00);
	SPI_W6100_WCR(GA6R7, 0x00);
	SPI_W6100_WCR(GA6R8, 0x13);
	SPI_W6100_WCR(GA6R9, 0x22);
	SPI_W6100_WCR(GA6R10, 0x33);
	SPI_W6100_WCR(GA6R11, 0xff);
	SPI_W6100_WCR(GA6R12, 0xfe);
	SPI_W6100_WCR(GA6R13, 0x44);
	SPI_W6100_WCR(GA6R14, 0x55);
	SPI_W6100_WCR(GA6R15, 0x66);

	SPI_W6100_WCR(NETLCKR, 0x00);	// NETLCKR		Network settings lock
}




uint32_t W6100_OpenTCPSocket (uint8_t sck_nbr) {
	uint32_t dest_adr;
	/* *** Open Socket as TCP4 *** */
	SPI_W6100_WSOCK(Sn_MR, 0x01, sck_nbr, REG);				// Set TCP4 mode
	SPI_W6100_WSOCK(Sn_PORTR0, 0x13, sck_nbr, REG);			// Set PORT 5000
	SPI_W6100_WSOCK(Sn_PORTR1, 0x88, sck_nbr, REG);
	SPI_W6100_WSOCK(Sn_CR, 0x01, sck_nbr, REG);				// Set OPEN command
	while ((SPI_W6100_RSOCK(Sn_CR, sck_nbr, REG)) != 0x00);	// Wait until OPEN command is cleared


	SPI_W6100_WSOCK(Sn_CR, 0x02, sck_nbr, REG);				// Set LISTEN command
	while ((SPI_W6100_RSOCK(Sn_CR, sck_nbr, REG)) != 0x00);	// Wait until LISTEN command is cleared

	// CONNECT NOW
	while ((SPI_W6100_RSOCK(Sn_SR, sck_nbr, REG)) != 0x17);	// Wait until SOCKET ESTABLISHED

	/* HARDWARE RESPONSE ON THE SOCKET OPEN */
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;
	/* END OF HARDWARE RESPONSE */							// Socket established

	SPI_W6100_WSOCK(Sn_IRCLR, 0x01, sck_nbr, REG);			// Interrupt clear

	// Read destination address
	dest_adr = (SPI_W6100_RSOCK(Sn_DIPR0, sck_nbr, REG) << 24);
	dest_adr |= (SPI_W6100_RSOCK(Sn_DIPR1, sck_nbr, REG) << 16);
	dest_adr |= (SPI_W6100_RSOCK(Sn_DIPR2, sck_nbr, REG) << 8);
	dest_adr |= SPI_W6100_RSOCK(Sn_DIPR3, sck_nbr, REG);

	return dest_adr;	// Return destination address
}




uint8_t W6100_ReceiveData(uint8_t sck_nbr, uint32_t dest_adr, uint8_t * tab, uint8_t size) {
	uint8_t i;
	uint32_t get_size, gSn_RX_MAX, get_start_address, Sn_RX_RD_temp;

	if ((SPI_W6100_RSOCK(Sn_SR, sck_nbr, REG)) == 0x1c) {								// Check if socket close request pending
		W6100_PassiveCloseSocket(sck_nbr);
	}

	if ((SPI_W6100_RSOCK(Sn_IR, sck_nbr, REG) & 0b00000100) == 0x04) {					// Check if data received

			// Clear data interrupt
			SPI_W6100_WSOCK(Sn_IRCLR, 0x04, sck_nbr, REG);

			// Read data from the buffer
			get_size = (SPI_W6100_RSOCK(Sn_RX_RSR0, sck_nbr, REG) << 8);
			get_size |= SPI_W6100_RSOCK(Sn_RX_RSR1, sck_nbr, REG);
			gSn_RX_MAX = (SPI_W6100_RSOCK(Sn_RX_BSR, sck_nbr, REG) * 1024);
			get_start_address = (SPI_W6100_RSOCK(Sn_RX_RD0, sck_nbr, REG) << 8);
			get_start_address |= SPI_W6100_RSOCK(Sn_RX_RD1, sck_nbr, REG);

			// Move data to the array
			memset(tab, '\0', size);
			for (i=0; i<get_size; i++) {
				tab[i] = SPI_W6100_RSOCK((get_start_address+i), sck_nbr, RX_BUF);
			}

			memcpy(&get_start_address, &dest_adr, get_size);

			Sn_RX_RD_temp = (SPI_W6100_RSOCK(Sn_RX_RD0, sck_nbr, REG) << 8);
			Sn_RX_RD_temp |= SPI_W6100_RSOCK(Sn_RX_RD1, sck_nbr, REG);
			Sn_RX_RD_temp += get_size;
			SPI_W6100_WSOCK(Sn_RX_RD0, (Sn_RX_RD_temp>>8), sck_nbr, REG);
			SPI_W6100_WSOCK(Sn_RX_RD1, (Sn_RX_RD_temp), sck_nbr, REG);
			SPI_W6100_WSOCK(Sn_CR, 0x40, sck_nbr, REG);
			while((SPI_W6100_RSOCK(Sn_CR, sck_nbr, REG)) != 0x00);

			return 1;	// Return 1 if data was received
	}
	else 	return 0;	// Return 0 of no data was received
}

void W6100_TransmitData(uint8_t sck_nbr, uint32_t dest_adr, uint8_t * tab, uint8_t size) {
	uint8_t i;
	uint8_t send_size = size;
	uint32_t gSn_TX_MAX, get_start_address, Sn_TX_WR_temp, Sn_TX_FSR_temp;

	gSn_TX_MAX = (SPI_W6100_RSOCK(Sn_TX_BSR, sck_nbr, REG) * 1024);						// Socket TX buffer size

	if(send_size > gSn_TX_MAX) send_size = gSn_TX_MAX;


	while(send_size > Sn_TX_FSR_temp) {													// wait until Socket Tx buffer is free
		Sn_TX_FSR_temp = (SPI_W6100_RSOCK(Sn_TX_FSR0, sck_nbr, REG) << 8);
		Sn_TX_FSR_temp |= SPI_W6100_RSOCK(Sn_TX_FSR1, sck_nbr, REG);
	}

	get_start_address = (SPI_W6100_RSOCK(Sn_TX_WR0, sck_nbr, REG) << 8);
	get_start_address |= SPI_W6100_RSOCK(Sn_TX_WR1, sck_nbr, REG);


	Sn_TX_WR_temp = (SPI_W6100_RSOCK(Sn_TX_WR0, sck_nbr, REG) << 8);
	Sn_TX_WR_temp |= SPI_W6100_RSOCK(Sn_TX_WR1, sck_nbr, REG);
	Sn_TX_WR_temp += size;
	SPI_W6100_WSOCK(Sn_TX_WR0, (Sn_TX_WR_temp>>8), sck_nbr, REG);
	SPI_W6100_WSOCK(Sn_TX_WR1, (Sn_TX_WR_temp), sck_nbr, REG);

	// Move data to the array
	for (i=0; i<size; i++) {
		SPI_W6100_WSOCK((get_start_address+i), tab[i], sck_nbr, TX_BUF);
	}

	memcpy(&get_start_address, &dest_adr, send_size);

	SPI_W6100_WSOCK(Sn_CR, 0x20, sck_nbr, REG);											// SEND command sent to TCP/TCP6 mode
	while(SPI_W6100_RSOCK(Sn_CR, sck_nbr, REG) != 0x00);								// Wait for SEND command clear

	while(((SPI_W6100_RSOCK(Sn_IR, sck_nbr, REG) & 0x10) == 0) && ((SPI_W6100_RSOCK(Sn_IR, sck_nbr, REG) & 0x08) == 0));

	if((SPI_W6100_RSOCK(Sn_IR, sck_nbr, REG) & 0x10) == 0x10) SPI_W6100_WSOCK(Sn_IRCLR, 0x10, sck_nbr, REG);	// Clear SENDOK interrupt

}



void W6100_PassiveCloseSocket(uint8_t sck_nbr) {
	SPI_W6100_WSOCK(Sn_CR, 0x08, sck_nbr, REG);						// Send FIN packet (DISCON command)
	while((SPI_W6100_RSOCK(Sn_CR, sck_nbr, REG)) != 0x00);			// Wait for DISCON command clear
	// Wait for ACK packet
	while((((SPI_W6100_RSOCK(Sn_IR, sck_nbr, REG)) & 0b10) == 0) && (((SPI_W6100_RSOCK(Sn_IR, sck_nbr, REG)) & 0b1000) == 0));

	if ((((SPI_W6100_RSOCK(Sn_IR, sck_nbr, REG)) & 0b10) == 0b10)) {
		SPI_W6100_WSOCK(Sn_IRCLR, 0x02, sck_nbr, REG);				// Clear DISCON interrupt
		while((SPI_W6100_RSOCK(Sn_SR, sck_nbr, REG)) != 0x00);		// Wait until socket is CLOSED
	}
	/* HARDWARE RESPONSE ON THE SOCKET CLOSE */
	GPIOC->ODR |= GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;
	/* END OF HARDWARE ESPONSE */
}












