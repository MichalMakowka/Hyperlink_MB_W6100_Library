/*
 * config.h
 *
 *  Created on: 24 May 2021
 *      Author: user
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#define ON 1
#define OFF 0
#include "stm32f405xx.h"




void SystemRegisterCFG(void);


uint8_t SPI_Eth_RT(uint8_t data);
void SPI_Eth_SS(uint8_t state);
uint8_t SPI_W6100_RCR(uint16_t adr);
#endif /* INC_CONFIG_H_ */
