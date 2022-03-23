/*
 * control_sys.c
 *
 *  Created on: 3 Jan 2022
 *      Author: Michal Makowka
 */
#include "control_sys.h"

char brake_val_tab[2];

// CAN variables: array enum references
enum can_msg {can_systems_on, can_systems_off, can_status_request, can_status_ok, can_status_fault, can_brake_ctrl, can_propulsion_ctrl};

// Define CAN variable structures below
void canVariables(CAN_MESSAGE * cmg) {

	strcpy(cmg[can_systems_on].data, "mb_0001");
	cmg[can_systems_on].format = STANDARD_FORMAT;
	cmg[can_systems_on].type = DATA_FRAME;
	cmg[can_systems_on].len = sizeof(cmg[can_systems_on]);
	cmg[can_systems_on].id = ALL_NODES;

	strcpy(cmg[can_systems_off].data, "mb_0002");
	cmg[can_systems_off].format = STANDARD_FORMAT;
	cmg[can_systems_off].type = DATA_FRAME;
	cmg[can_systems_off].len = sizeof(cmg[can_systems_off]);
	cmg[can_systems_off].id = ALL_NODES;

	strcpy(cmg[can_status_request].data, "mb_1010");
	cmg[can_status_request].format = STANDARD_FORMAT;
	cmg[can_status_request].type = DATA_FRAME;
	cmg[can_status_request].len = sizeof(cmg[can_status_request]);
	cmg[can_status_request].id = ALL_NODES;

	strcpy(cmg[can_status_ok].data, "mb_0000");
	cmg[can_status_ok].format = STANDARD_FORMAT;
	cmg[can_status_ok].type = DATA_FRAME;
	cmg[can_status_ok].len = sizeof(cmg[can_status_ok]);
	cmg[can_status_ok].id = ALL_NODES;

	strcpy(cmg[can_status_fault].data, "mb_1111");
	cmg[can_status_fault].format = STANDARD_FORMAT;
	cmg[can_status_fault].type = DATA_FRAME;
	cmg[can_status_fault].len = sizeof(cmg[can_status_fault]);
	cmg[can_status_fault].id = ALL_NODES;

	strcpy(cmg[can_brake_ctrl].data, "mb_b_xx");		// "xx" value to be substituted before transmission
	cmg[can_brake_ctrl].format = STANDARD_FORMAT;
	cmg[can_brake_ctrl].type = DATA_FRAME;
	cmg[can_brake_ctrl].len = sizeof(cmg[can_brake_ctrl]);
	cmg[can_brake_ctrl].id = BRAKING;

	strcpy(cmg[can_propulsion_ctrl].data, "mb_p_xx");		// "xx" value to be substituted before transmission
	cmg[can_propulsion_ctrl].format = STANDARD_FORMAT;
	cmg[can_propulsion_ctrl].type = DATA_FRAME;
	cmg[can_propulsion_ctrl].len = sizeof(cmg[can_propulsion_ctrl]);
	cmg[can_propulsion_ctrl].id = PROPULSION;


}


void dataPacketReceived(char * RxBuf) {
	/* Reaction on Data Received */

	// Check Ethernet
		if (!strcmp(RxBuf, "AT+st?\n")) {
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"MBR: OK\n", sizeof("MBR: OK\n"));
			Can_Tx_Msg(&canMessages[can_status_request]);

		}
		else if (!strcmp(RxBuf, "AT+on\n"))	{
			GPIOB->ODR |= GPIO_ODR_OD14;
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Systems EN...\n", sizeof("Systems EN...\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_systems_on]);
		}
		else if (!strcmp(RxBuf, "AT+off\n"))	{
			GPIOB->ODR &= ~GPIO_ODR_OD14;
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Systems DIS...\n", sizeof("Systems DIS...\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_systems_off]);
		}
		else if (!strncmp(RxBuf, "AT+B+xx\n", 5))	{
			brake_val_tab[0] = RxBuf[5];
			brake_val_tab[1] = RxBuf[6];
			// Send msg to the client
			char strTemp[13];
			sprintf(strTemp, "BRK SET: %c%c\n", brake_val_tab[0], brake_val_tab[1]);
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)(strTemp), sizeof(strTemp));
			// Send CAN frame
			canMessages[can_brake_ctrl].data[5] = brake_val_tab[0];
			canMessages[can_brake_ctrl].data[6] = brake_val_tab[1];
			Can_Tx_Msg(&canMessages[can_brake_ctrl]);
		}
		else {
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Command unknown...\n", sizeof("Command unknown...\n"));
		}


}

void canMessageReceived(CAN_MESSAGE msg) {

	// Status request Handler
	if (!strcmp(msg.data,"br_0000")) {
		W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"BRK: OK\n", sizeof("BRK: OK\n"));
		Can_Tx_Msg(&canMessages[can_status_ok]);
	}
	if (!strcmp(msg.data,"pr_0000")) {
		W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"PRP: OK\n", sizeof("PRP: OK\n"));
		Can_Tx_Msg(&canMessages[can_status_ok]);
	}
	if (!strcmp(msg.data,"br_1111")) {
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"BRK: FAULT\n", sizeof("BRK: FAULT\n"));
			Can_Tx_Msg(&canMessages[can_status_fault]);
	}
	if (!strcmp(msg.data,"pr_1111")) {
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"PRP: FAULT\n", sizeof("PRP: FAULT\n"));
			Can_Tx_Msg(&canMessages[can_status_fault]);
	}
	if (!strncmp(msg.data,"br_b_xx", 5)) {
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"BRK: SUCCESS\n", sizeof("BRK: SUCCESS\n"));
			Can_Tx_Msg(&canMessages[can_status_fault]);
	}
	if (!strncmp(msg.data,"br_p_xx", 5)) {
				W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"PRP: SUCCESS\n", sizeof("PRP: SUCCESS\n"));
				Can_Tx_Msg(&canMessages[can_status_fault]);
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

void setBrakeVal(uint8_t value) {
	itoa(value, brake_val_tab, 10);
}
