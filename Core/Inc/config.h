/*
 * config.h
 *
 *  Created on: 24 May 2021
 *      Author: user
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "stm32f405xx.h"

#define ON 1
#define OFF 0

#define REG 1
#define TX_BUF 2
#define RX_BUF 3





void SystemRegisterCFG(void);


uint8_t SPI_Eth_RT(uint8_t data);
void SPI_Eth_SS(uint8_t state);
uint8_t SPI_W6100_RCR(uint16_t adr);
void SPI_W6100_WCR(uint16_t adr, uint8_t val);
uint8_t SPI_W6100_RSOCK(uint16_t adr, uint8_t socket_nbr, uint8_t block);
void SPI_W6100_WSOCK(uint16_t adr, uint8_t val, uint8_t socket_nbr, uint8_t block);
#endif /* INC_CONFIG_H_ */
