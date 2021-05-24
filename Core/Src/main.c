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

	uint8_t rx_dat;
	uint32_t i;
  while (1)
  {


	  rx_dat = SPI_W6100_RCR(0x0000);
	  GPIOC->ODR ^= GPIO_ODR_OD8;
	  for (i=0; i<1000000; i++);
	  rx_dat = SPI_W6100_RCR(0x0001);
	  GPIOC->ODR ^= GPIO_ODR_OD8;
	  for (i=0; i<1000000; i++);
	  rx_dat = SPI_W6100_RCR(0x0003);
	  GPIOC->ODR ^= GPIO_ODR_OD8;
	  for (i=0; i<1000000; i++);
	  rx_dat = SPI_W6100_RCR(0x0002);
	  GPIOC->ODR ^= GPIO_ODR_OD8;
	  for (i=0; i<1000000; i++);

  }

}

























