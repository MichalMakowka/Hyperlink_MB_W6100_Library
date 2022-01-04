/*
 * control_sys.c
 *
 *  Created on: 3 Jan 2022
 *      Author: Michal Makowka
 */
#include "control_sys.h"

enum canmsg {can_on_msg, can_off_msg};

void canVariables(CAN_MESSAGE * cmg) {

	strcpy(cmg[can_on_msg].data, "mb_0000");
	cmg[can_on_msg].format = STANDARD_FORMAT;
	cmg[can_on_msg].type = DATA_FRAME;
	cmg[can_on_msg].len = sizeof(cmg[can_on_msg]);
	cmg[can_on_msg].id = 0x01;

	strcpy(cmg[can_off_msg].data, "mb_0001");
	cmg[can_off_msg].format = STANDARD_FORMAT;
	cmg[can_off_msg].type = DATA_FRAME;
	cmg[can_off_msg].len = sizeof(cmg[can_off_msg]);
	cmg[can_off_msg].id = 0x02;

}


void dataPacketReceived(void) {
	/* Reaction on Data Received */
	// Check Ethernet
	if (W6100_ReceiveData(0, destination_adr, (uint8_t*)rx_dat, sizeof(rx_dat))) {		// Check if data arrived
		if (!strcmp(rx_dat, "systems_on\n")) {
			GPIOC->ODR &= ~GPIO_ODR_OD12;
			// Send msg to the client
			W6100_TransmitData(0, destination_adr, (uint8_t*)"System Enabled\n", sizeof("System Enabled\n"));
			Can_Tx_Msg(&canMessages[can_on_msg]);

		}
		else if (!strcmp(rx_dat, "systems_off\n"))	{
			GPIOC->ODR |= GPIO_ODR_OD12;
			// Send msg to the client
			W6100_TransmitData(0, destination_adr, (uint8_t*)"System Disabled\n", sizeof("System Disabled\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_off_msg]);
		}
		else if (!strcmp(rx_dat, "systems_st\n"))	{
			GPIOC->ODR ^= GPIO_ODR_OD12;
			// Send msg to the client
			W6100_TransmitData(0, destination_adr, (uint8_t*)"Checking status...\n", sizeof("Checking status...\n"));
		}
		else {
			// Send msg to the client
			W6100_TransmitData(0, destination_adr, (uint8_t*)"Command unknown...\n", sizeof("Command unknown...\n"));
		}
	}

}

void serverOffResponse(uint8_t sck_nbr) {
	GPIOC->ODR |= GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;
}

void serverStartResponse(uint8_t sck_nbr) {
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;
}
