/*
 * control_sys.c
 *
 *  Created on: 3 Jan 2022
 *      Author: Michal Makowka
 */
#include "control_sys.h"

// CAN variables: array enum references
enum can_msg {can_on_msg, can_off_msg};

// Define CAN variable structures below
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


void dataPacketReceived(char * RxBuf) {
	/* Reaction on Data Received */
	// Check Ethernet

		if (!strcmp(RxBuf, "systems_on\n")) {
			GPIOC->ODR &= ~GPIO_ODR_OD12;
			// Send msg to the client
			W6100_TransmitData(0, socket_dest_adr[0], (uint8_t*)"Systems Enabled\n", sizeof("Systems Enabled\n"));
			Can_Tx_Msg(&canMessages[can_on_msg]);

		}
		else if (!strcmp(RxBuf, "systems_off\n"))	{
			GPIOC->ODR |= GPIO_ODR_OD12;
			// Send msg to the client
			W6100_TransmitData(0, socket_dest_adr[0], (uint8_t*)"Systems Disabled\n", sizeof("Systems Disabled\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_off_msg]);
		}
		else if (!strcmp(RxBuf, "systems_st\n"))	{
			GPIOC->ODR ^= GPIO_ODR_OD12;
			// Send msg to the client
			W6100_TransmitData(0, socket_dest_adr[0], (uint8_t*)"Checking status...\n", sizeof("Checking status...\n"));
		}
		else {
			// Send msg to the client
			W6100_TransmitData(0, socket_dest_adr[0], (uint8_t*)"Command unknown...\n", sizeof("Command unknown...\n"));
		}


}

void canMessageReceived(CAN_MESSAGE msg) {
	GPIOC->ODR ^= GPIO_ODR_OD11;
}

void serverOffResponse(uint8_t sck_nbr) {
	GPIOC->ODR |= GPIO_ODR_OD8;
	GPIOC->ODR &= ~GPIO_ODR_OD9;
}

void serverStartResponse(uint8_t sck_nbr) {
	GPIOC->ODR &= ~GPIO_ODR_OD8;
	GPIOC->ODR |= GPIO_ODR_OD9;
}
