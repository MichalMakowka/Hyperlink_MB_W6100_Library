/*
 * w6100.h
 *
 *  Created on: 15 Jun 2021
 *      Author: Michal Makowka
 */

/* *** W6100 ETHERNET CONTROLLER Library ***
 *
 * void W6100_INIT (void) - Ethernet uC and W6100 chip initialisation.
 * uint32_t W6100_OpenTCPSocket (uint8_t sck_nbr, uint16_t port) - Function used to open a TCP socket.
 * uint8_t W6100_ReceiveData(uint8_t sck_nbr, uint32_t dest_adr, uint8_t * tab, uint8_t size) - Function used to check for a new message.
 * void W6100_TransmitData(uint8_t sck_nbr, uint32_t dest_adr, uint8_t * tab, uint8_t size) - Function used to transmit data.
 *
 * void registerSocketCloseCallback(void (*callback)(uint8_t sck_nbr)) - Register user function with
 * socket number parameter executed after the socket is closed.
 *
 * void registerSocketOpenCallback(void (*callback)(uint8_t sck_nbr)) - Register user function with
 * socket number parameter executed after the socket is opened.
 *
 * void registerDataReceivedCallback(void (*callback)(void)) - Register user function which is executed after W6100 informes about
 * data in Rx buffer to be read (W6100_ReceiveData should be used inside this function).
 *
 * *****************************************
 */
#ifndef INC_W6100_H_
#define INC_W6100_H_

#include <stdio.h>
#include <string.h>
#include "stm32f405xx.h"

/* *** WIZZNET W6100 CONFIG SECTION *** */
// W6100 MAC ADDRESS //
#define MAC0	0x11
#define MAC1	0x22
#define MAC2	0x33
#define MAC3	0xaa
#define MAC4	0xbb
#define MAC5	0xcc

// GATEWAY IP ADDRESS //
#define GIP0	192
#define GIP1	168
#define GIP2	0
#define GIP3	1

// SUBNET MASK //
#define SBM0	255
#define SBM1	255
#define SBM2	255
#define SBM3	0

// IPv4 SOURCE IP ADDRESS //
#define IPV0	192
#define IPV1	168
#define IPV2	0
#define IPV3	27
/* *** END OF CONFIG SECTION *** */


#define ON 1
#define OFF 0

#define REG 1
#define TX_BUF 2
#define RX_BUF 3

/* COMMON W6100 REGISTER */
#define	CIDR0		0x0000
#define	CIDR1		0x0001
#define	VER0		0x0002
#define	VER1		0x0003
#define	SYSR		0x2000
#define	SYCR0		0x2004
#define	SYCR1		0x2005
#define	TCNTR0		0x2016
#define	TCNTR1		0x2017
#define	TCNTCLR		0x2020
#define	IR			0x2100
#define	SIR			0x2101
#define	SLIR		0x2102
#define	IMR			0x2104
#define	IRCLR		0x2108
#define	SIMR		0x2114
#define	SLIMR		0x2124
#define	SLIRCLR		0x2128
#define	SLPSR		0x212C
#define	SLCR		0x2130
#define	PHYSR		0x3000
#define	PHYRAR		0x3008
#define	PHYDIR0		0x300C
#define	PHYDIR1		0x300D
#define	PHYDOR0		0x3010
#define	PHYDOR1		0x3011
#define	PHYACR		0x3014
#define	PHYDIVR		0x3018
#define	PHYCR0		0x301C
#define	PHYCR1		0x301D
#define	NET4MR		0x4000
#define	NET6MR		0x4004
#define	NETMR		0x4008
#define	NETMR2		0x4009
#define	PTMR		0x4100
#define	PMNR		0x4104
#define	PHAR0		0x4108
#define	PHAR1		0x4109
#define	PHAR2		0x410A
#define	PHAR3		0x410B
#define	PHAR4		0x410C
#define	PHAR5		0x410D
#define	PSIDR0		0x4110
#define	PSIDR1		0x4111
#define	PMRUR0		0x4114
#define	PMRUR1		0x4115
#define	SHAR0		0x4120
#define	SHAR1		0x4121
#define	SHAR2		0x4122
#define	SHAR3		0x4123
#define	SHAR4		0x4124
#define	SHAR5		0x4125
#define	GAR0		0x4130
#define	GAR1		0x4131
#define	GAR2		0x4132
#define	GAR3		0x4133
#define	SUBR0		0x4134
#define	SUBR1		0x4135
#define	SUBR2		0x4136
#define	SUBR3		0x4137
#define	SIPR0		0x4138
#define	SIPR1		0x4139
#define	SIPR2		0x413A
#define	SIPR3		0x413B
#define	LLAR0		0x4140
#define	LLAR1		0x4141
#define	LLAR2		0x4142
#define	LLAR3		0x4143
#define	LLAR4		0x4144
#define	LLAR5		0x4145
#define	LLAR6		0x4146
#define	LLAR7		0x4147
#define	LLAR8		0x4148
#define	LLAR9		0x4149
#define	LLAR10		0x414A
#define	LLAR11		0x414B
#define	LLAR12		0x414C
#define	LLAR13		0x414D
#define	LLAR14		0x414E
#define	LLAR15		0x414F
#define	GUAR0		0x4150
#define	GUAR1		0x4151
#define	GUAR2		0x4152
#define	GUAR3		0x4153
#define	GUAR4		0x4154
#define	GUAR5		0x4155
#define	GUAR6		0x4156
#define	GUAR7		0x4157
#define	GUAR8		0x4158
#define	GUAR9		0x4159
#define	GUAR10		0x415A
#define	GUAR11		0x415B
#define	GUAR12		0x415C
#define	GUAR13		0x415D
#define	GUAR14		0x415E
#define	GUAR15		0x415F
#define	SUB6R0		0x4160
#define	SUB6R1		0x4161
#define	SUB6R2		0x4162
#define	SUB6R3		0x4163
#define	SUB6R4		0x4164
#define	SUB6R5		0x4165
#define	SUB6R6		0x4166
#define	SUB6R7		0x4167
#define	SUB6R8		0x4168
#define	SUB6R9		0x4169
#define	SUB6R10		0x416A
#define	SUB6R11		0x416B
#define	SUB6R12		0x416C
#define	SUB6R13		0x416D
#define	SUB6R14		0x416E
#define	SUB6R15		0x416F
#define	GA6R0		0x4170
#define	GA6R1		0x4171
#define	GA6R2		0x4172
#define	GA6R3		0x4173
#define	GA6R4		0x4174
#define	GA6R5		0x4175
#define	GA6R6		0x4176
#define	GA6R7		0x4177
#define	GA6R8		0x4178
#define	GA6R9		0x4179
#define	GA6R10		0x417A
#define	GA6R11		0x417B
#define	GA6R12		0x417C
#define	GA6R13		0x417D
#define	GA6R14		0x417E
#define	GA6R15		0x417F
#define	SLDIP6R0	0x4180
#define	SLDIP6R1	0x4181
#define	SLDIP6R2	0x4182
#define	SLDIP6R3	0x4183
#define	SLDIP6R4	0x4184
#define	SLDIP6R5	0x4185
#define	SLDIP6R6	0x4186
#define	SLDIP6R7	0x4187
#define	SLDIP6R8	0x4188
#define	SLDIP6R9	0x4189
#define	SLDIP6R10	0x418A
#define	SLDIP6R11	0x418B
#define	SLDIP6R12	0x418C
#define	SLDIP6R13	0x418D
#define	SLDIP6R14	0x418E
#define	SLDIP6R15	0x418F
#define	SLDHAR0		0x4190
#define	SLDHAR1		0x4191
#define	SLDHAR2		0x4192
#define	SLDHAR3		0x4193
#define	SLDHAR4		0x4194
#define	SLDHAR5		0x4195
#define	PINGIDR0	0x4198
#define	PINGIDR1	0x4199
#define	PINGSEQR0	0x419C
#define	PINGSEQR1	0x419D
#define	UIPR0		0x41A0
#define	UIPR1		0x41A1
#define	UIPR2		0x41A2
#define	UIPR3		0x41A3
#define	UPORTR0		0x41A4
#define	UPORTR1		0x41A5
#define	UIP6R0		0x41B0
#define	UIP6R1		0x41B1
#define	UIP6R2		0x41B2
#define	UIP6R3		0x41B3
#define	UIP6R4		0x41B4
#define	UIP6R5		0x41B5
#define	UIP6R6		0x41B6
#define	UIP6R7		0x41B7
#define	UIP6R8		0x41B8
#define	UIP6R9		0x41B9
#define	UIP6R10		0x41BA
#define	UIP6R11		0x41BB
#define	UIP6R12		0x41BC
#define	UIP6R13		0x41BD
#define	UIP6R14		0x41BE
#define	UIP6R15		0x41BF
#define	UPORT6R0	0x41C0
#define	UPORT6R1	0x41C1
#define	INTPTMR0	0x41C5
#define	INTPTMR1	0x41C6
#define	PLR			0x41D0
#define	PFR			0x41D4
#define	VLTR0		0x41D8
#define	VLTR1		0x41D9
#define	VLTR2		0x41DA
#define	VLTR3		0x41DB
#define	PLTR0		0x41DC
#define	PLTR1		0x41DD
#define	PLTR2		0x41DE
#define	PLTR3		0x41DF
#define	PAR0		0x41E0
#define	PAR1		0x41E1
#define	PAR2		0x41E2
#define	PAR3		0x41E3
#define	PAR4		0x41E4
#define	PAR5		0x41E5
#define	PAR6		0x41E6
#define	PAR7		0x41E7
#define	PAR8		0x41E8
#define	PAR9		0x41E9
#define	PAR10		0x41EA
#define	PAR11		0x41EB
#define	PAR12		0x41EC
#define	PAR13		0x41ED
#define	PAR14		0x41EE
#define	PAR15		0x41EF
#define	ICMP6BLKR	0x41F0
#define	CHPLCKR		0x41F4
#define	NETLCKR		0x41F5
#define	PHYLCKR		0x41F6
#define	RTR0		0x4200
#define	RTR1		0x4201
#define	RCR			0x4204
#define	SLRTR0		0x4208
#define	SLRTR1		0x4209
#define	SLRCR		0x420C
#define	SLHOPR		0x420F


/* SOCKET REGISTER */
#define	Sn_MR		0x0000
#define	Sn_PSR		0x0004
#define	Sn_CR		0x0010
#define	Sn_IR		0x0020
#define	Sn_IMR		0x0024
#define	Sn_IRCLR	0x0028
#define	Sn_SR		0x0030
#define	Sn_ESR		0x0031
#define	Sn_PNR		0x0100
#define	Sn_TOSR		0x0104
#define	Sn_TTLR		0x0108
#define	Sn_FRGR0	0x010C
#define	Sn_FRGR1	0x010D
#define	Sn_MSSR0	0x0110
#define	Sn_MSSR1	0x0111
#define	Sn_PORTR0	0x0114
#define	Sn_PORTR1	0x0115
#define	Sn_DHAR0	0x0118
#define	Sn_DHAR1	0x0119
#define	Sn_DHAR2	0x011A
#define	Sn_DHAR3	0x011B
#define	Sn_DHAR4	0x011C
#define	Sn_DHAR5	0x011D
#define	Sn_DIPR0	0x0120
#define	Sn_DIPR1	0x0121
#define	Sn_DIPR2	0x0122
#define	Sn_DIPR3	0x0123
#define	Sn_DIP6R0	0x0130
#define	Sn_DIP6R1	0x0131
#define	Sn_DIP6R2	0x0132
#define	Sn_DIP6R3	0x0133
#define	Sn_DIP6R4	0x0134
#define	Sn_DIP6R5	0x0135
#define	Sn_DIP6R6	0x0136
#define	Sn_DIP6R7	0x0137
#define	Sn_DIP6R8	0x0138
#define	Sn_DIP6R9	0x0139
#define	Sn_DIP6R10	0x013A
#define	Sn_DIP6R11	0x013B
#define	Sn_DIP6R12	0x013C
#define	Sn_DIP6R13	0x013D
#define	Sn_DIP6R14	0x013E
#define	Sn_DIP6R15	0x013F
#define	Sn_DPORTR0	0x0140
#define	Sn_DPORTR1	0x0141
#define	Sn_MR2		0x0144
#define	Sn_RTR0		0x0180
#define	Sn_RTR1		0x0181
#define	Sn_RCR		0x0184
#define	Sn_KPALVTR	0x0188
#define	Sn_TX_BSR	0x0200
#define	Sn_TX_FSR0	0x0204
#define	Sn_TX_FSR1	0x0205
#define	Sn_TX_RD0	0x0208
#define	Sn_TX_RD1	0x0209
#define	Sn_TX_WR0	0x020C
#define	Sn_TX_WR1	0x020D
#define	Sn_RX_BSR	0x0220
#define	Sn_RX_RSR0	0x0224
#define	Sn_RX_RSR1	0x0225
#define	Sn_RX_RD0	0x0228
#define	Sn_RX_RD1	0x0229
#define	Sn_RX_WR0	0x022C
#define	Sn_RX_WR1	0x022D



uint32_t socket_dest_adr[2];		// Socket[n] destination addresses




void W6100_INIT (void);

uint32_t W6100_OpenTCPSocket (uint8_t sck_nbr, uint16_t port);
uint8_t W6100_ReceiveData(uint8_t sck_nbr, uint32_t dest_adr, uint8_t * tab, uint8_t size);
void W6100_TransmitData(uint8_t sck_nbr, uint32_t dest_adr, uint8_t * tab, uint8_t size);

void W6100_PassiveCloseSocket(uint8_t sck_nbr);

static void (*socketCloseCallback)(uint8_t sck_nbr);
void registerSocketCloseCallback(void (*callback)(uint8_t sck_nbr));

static void (*socketOpenCallback)(uint8_t sck_nbr);
void registerSocketOpenCallback(void (*callback)(uint8_t sck_nbr));

static void (*dataReceivedCallback)(char * RxBuf);
void registerDataReceivedCallback(void (*callback)(char * RxBuf));

uint8_t SPI_Eth_RT(uint8_t data);
void SPI_Eth_SS(uint8_t state);
uint8_t SPI_W6100_RCR(uint16_t adr);
void SPI_W6100_WCR(uint16_t adr, uint8_t val);
uint8_t SPI_W6100_RSOCK(uint16_t adr, uint8_t socket_nbr, uint8_t block);
void SPI_W6100_WSOCK(uint16_t adr, uint8_t val, uint8_t socket_nbr, uint8_t block);


#endif /* INC_W6100_H_ */
