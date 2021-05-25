/*
 * This is W6100 Hyperlink Motherboard Ethernet library.
 * Designed by Michal Makowka
 *
 * (C) All rights reserved.
 *
 * v 1.0
 */

#include "stm32f405xx.h"
#include "config.h"




int main(void)
{

	SystemRegisterCFG();

	GPIOC->ODR &= ~GPIO_ODR_OD9;

	uint8_t rx_dat;


	/* *** W6100 Init *** */
	SPI_W6100_WCR(NETLCKR, 0x3a);	// NETLCKR		Network settings unlock

	SPI_W6100_WCR(SHAR0, 0x11);	// SHAR[5:0]	Set hardware MAC address
	SPI_W6100_WCR(SHAR1, 0x22);
	SPI_W6100_WCR(SHAR2, 0x33);
	SPI_W6100_WCR(SHAR3, 0xaa);
	SPI_W6100_WCR(SHAR4, 0xbb);
	SPI_W6100_WCR(SHAR5, 0xcc);

	SPI_W6100_WCR(GAR0, 0xc0);	// GAR[0:3]		Gateway IP address 192.168.0.1
	SPI_W6100_WCR(GAR1, 0xa8);
	SPI_W6100_WCR(GAR2, 0x00);
	SPI_W6100_WCR(GAR3, 0x01);

	SPI_W6100_WCR(SUBR0, 0xff);	// SUBR[0:3]	Subnet mask address 255:255:255:0
	SPI_W6100_WCR(SUBR1, 0xff);
	SPI_W6100_WCR(SUBR2, 0xff);
	SPI_W6100_WCR(SUBR3, 0x00);

	SPI_W6100_WCR(SIPR0, 0xc0);	// SIPR[0:3]	IPv4 Source IP Address 192.168.0.100
	SPI_W6100_WCR(SIPR1, 0xa8);
	SPI_W6100_WCR(SIPR2, 0x00);
	SPI_W6100_WCR(SIPR3, 0x64);

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

	/* *** Socket Configuration *** */
	uint8_t txTotalSize = 0;
	uint8_t rxTotalSize = 0;

	for (uint8_t i=0; i<7; i++) {
		SPI_W6100_WSOCK(Sn_TX_BSR, 0x02, i, REG);	// assign 2 Kbytes TX buffer per SOCKET
		SPI_W6100_WSOCK(Sn_RX_BSR, 0x02, i, REG);	// assign 2 Kbytes RX buffer per SOCKET
		txTotalSize += 0x02;
		rxTotalSize += 0x02;
	}

	/* *** Open Socket0 as TCP4 *** */
	SPI_W6100_WSOCK(Sn_MR, 0x01, 0, REG);	// Set TCP4 mode
	SPI_W6100_WSOCK(Sn_PORTR0, 0x13, 0, REG);	// Set PORT 5000
	SPI_W6100_WSOCK(Sn_PORTR1, 0x88, 0, REG);
	SPI_W6100_WSOCK(Sn_CR, 0x01, 0, REG);	// Set OPEN command
	while ((SPI_W6100_RSOCK(Sn_CR, 0, REG)) != 0x00);	// Wait until OPEN command is cleared


	SPI_W6100_WSOCK(Sn_CR, 0x02, 0, REG);	// Set LISTEN command
	while ((SPI_W6100_RSOCK(Sn_CR, 0, REG)) != 0x00);	// Wait until LISTEN command is cleared

	// CONNECT NOW

	while ((SPI_W6100_RSOCK(Sn_SR, 0, REG)) != 0x17);	// Wait until SOCKET ESTABLISHED
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;		// Socket established
	SPI_W6100_WSOCK(Sn_IRCLR, 0x01, 0, REG);	// Interrupt clear


  while (1)
  {
	  uint32_t i;

	  rx_dat = SPI_W6100_RCR(CIDR0);
	  for (i=0; i<100000; i++);

	  rx_dat = SPI_W6100_RCR(CIDR1);
	  for (i=0; i<100000; i++);

	  rx_dat = SPI_W6100_RCR(VER0);
	  for (i=0; i<100000; i++);

	  rx_dat = SPI_W6100_RCR(VER1);
	  for (i=0; i<100000; i++);

	  rx_dat = SPI_W6100_RCR(PHYDIVR);
	  for (i=0; i<100000; i++);


  }

}

























