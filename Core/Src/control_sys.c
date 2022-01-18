/*
 * control_sys.c
 *
 *  Created on: 3 Jan 2022
 *      Author: Michal Makowka
 */
#include "control_sys.h"

// CAN variables: array enum references
enum can_msg {can_on_msg, can_off_msg, can_brk_eng, can_brk_dsng};

// Define CAN variable structures below
void canVariables(CAN_MESSAGE * cmg) {

	strcpy(cmg[can_on_msg].data, "mb_0000");
	cmg[can_on_msg].format = STANDARD_FORMAT;
	cmg[can_on_msg].type = DATA_FRAME;
	cmg[can_on_msg].len = sizeof(cmg[can_on_msg]);
	cmg[can_on_msg].id = ALL_NODES;

	strcpy(cmg[can_off_msg].data, "mb_0001");
	cmg[can_off_msg].format = STANDARD_FORMAT;
	cmg[can_off_msg].type = DATA_FRAME;
	cmg[can_off_msg].len = sizeof(cmg[can_off_msg]);
	cmg[can_off_msg].id = ALL_NODES;

	strcpy(cmg[can_brk_eng].data, "mb_b_50");
	cmg[can_brk_eng].format = STANDARD_FORMAT;
	cmg[can_brk_eng].type = DATA_FRAME;
	cmg[can_brk_eng].len = sizeof(cmg[can_brk_eng]);
	cmg[can_brk_eng].id = BRAKING;

	strcpy(cmg[can_brk_dsng].data, "mb_b_00");
	cmg[can_brk_dsng].format = STANDARD_FORMAT;
	cmg[can_brk_dsng].type = DATA_FRAME;
	cmg[can_brk_dsng].len = sizeof(cmg[can_brk_dsng]);
	cmg[can_brk_dsng].id = BRAKING;

}


void dataPacketReceived(char * RxBuf) {
	/* Reaction on Data Received */

	// Check Ethernet
		if (!strcmp(RxBuf, "systems_on\n")) {
			GPIOB->ODR |= GPIO_ODR_OD14;
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Systems Enabled\n", sizeof("Systems Enabled\n"));
			Can_Tx_Msg(&canMessages[can_on_msg]);

		}
		else if (!strcmp(RxBuf, "systems_off\n"))	{
			GPIOB->ODR &= ~GPIO_ODR_OD14;
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Systems Disabled\n", sizeof("Systems Disabled\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_off_msg]);
		}
		else if (!strcmp(RxBuf, "systems_st\n"))	{

			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"System active\n", sizeof("System active\n"));

		}
		else if (!strcmp(RxBuf, "brake_on\n"))	{

			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Brk on requested\n", sizeof("Brk on requested\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_brk_eng]);
		}
		else if (!strcmp(RxBuf, "brake_off\n"))	{

			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Brk off requested\n", sizeof("Brk off requested\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_brk_dsng]);
		}
		else {
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Command unknown...\n", sizeof("Command unknown...\n"));
		}


}

void canMessageReceived(CAN_MESSAGE msg) {

	if (!strcmp(msg.data,"br_b_50")) {
		W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Brk on: success\n", sizeof("Brk on: success\n"));
	}
	if (!strcmp(msg.data,"br_b_00")) {
		W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Brk off: success\n", sizeof("Brk off: success\n"));
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
