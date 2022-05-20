/*
 * control_sys.c
 *
 *  Created on: 3 Jan 2022
 *      Author: Michal Makowka
 */
#include "control_sys.h"

char brake_val_tab[2];
char propulsion_val_tab[2];

volatile int sysTicks;

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
		if (!strcmp(RxBuf, "AT+st?\023")) {
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"MBR: OK\n", sizeof("MBR: OK\n"));
			Can_Tx_Msg(&canMessages[can_status_request]);

		}
		else if (!strncmp(RxBuf, "CO+x+x+x+x\023", 3)) {
			OpenControl(RxBuf);
		}
		else if (!strncmp(RxBuf, "CC+x+x+x+x\023", 3)) {
			ClosedControl(RxBuf);
		}
		else if (!strcmp(RxBuf, "AT+on\023"))	{
			GPIOB->ODR |= GPIO_ODR_OD14;
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Systems EN...\n", sizeof("Systems EN...\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_systems_on]);
		}
		else if (!strcmp(RxBuf, "AT+off\023"))	{
			GPIOB->ODR &= ~GPIO_ODR_OD14;
			// Send msg to the client
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Systems DIS...\n", sizeof("Systems DIS...\n"));
			// Send CAN frame
			Can_Tx_Msg(&canMessages[can_systems_off]);
		}
		else if (!strncmp(RxBuf, "AT+B+xx\023", 5))	{
			brake_val_tab[0] = RxBuf[5];
			brake_val_tab[1] = RxBuf[6];
			// Send msg to the client
			char strTemp[13];
			sprintf(strTemp, "BRK SET: %c%c\023", brake_val_tab[0], brake_val_tab[1]);
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)(strTemp), sizeof(strTemp));
			// Send CAN frame
			canMessages[can_brake_ctrl].data[5] = brake_val_tab[0];
			canMessages[can_brake_ctrl].data[6] = brake_val_tab[1];
			Can_Tx_Msg(&canMessages[can_brake_ctrl]);
		}
		else if (!strncmp(RxBuf, "AT+P+xx\023", 5))	{
			propulsion_val_tab[0] = RxBuf[5];
			propulsion_val_tab[1] = RxBuf[6];
			// Send msg to the client
			char strTemp[13];
			sprintf(strTemp, "PRP SET: %c%c\023", propulsion_val_tab[0], propulsion_val_tab[1]);
			W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)(strTemp), sizeof(strTemp));
			// Send CAN frame
			canMessages[can_propulsion_ctrl].data[5] = propulsion_val_tab[0];
			canMessages[can_propulsion_ctrl].data[6] = propulsion_val_tab[1];
			Can_Tx_Msg(&canMessages[can_propulsion_ctrl]);
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
	if (!strncmp(msg.data,"pr_p_xx", 5)) {
		W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"PRP: SUCCESS\n", sizeof("PRP: SUCCESS\n"));
		Can_Tx_Msg(&canMessages[can_status_fault]);
	}

}

void OpenControl(char * command) {

	char * token;
	uint8_t AccelTime, DesiredSpeed, CruiseTime, BrakingTime;
	token = strtok(command, "+");
	token = strtok(NULL, "+");
//	AccelTime = atoi(token);
	AccelTime = 4;	// Default value
//	token = strtok(NULL, "+");
	DesiredSpeed = atoi(token);
	token = strtok(NULL, "+");
	CruiseTime = atoi(token);
	token = strtok(NULL, "+");
	BrakingTime = atoi(token);

	// Brake OFF
	canMessages[can_brake_ctrl].data[5] = '1';
	canMessages[can_brake_ctrl].data[6] = '0';
	Can_Tx_Msg(&canMessages[can_brake_ctrl]);
	delay_ms(4000);
	// Acceleration process
	unsigned int delSetAccel = ((30-DesiredSpeed)/AccelTime)*1000;
	unsigned int delSetBrake = ((30-DesiredSpeed)/BrakingTime)*1000;
	char pValDec, pValUni;
	uint8_t i;
	for (i = 30; i>DesiredSpeed; i--) {
		if (i<10) {
			pValDec = '0';
			pValUni = ((i % 10)+'0');

		} else {
			pValDec = ((i/10) + '0');
			pValUni = ((i % 10)+'0');
		}
		canMessages[can_propulsion_ctrl].data[5] = pValDec;
		canMessages[can_propulsion_ctrl].data[6] = pValUni;
		Can_Tx_Msg(&canMessages[can_propulsion_ctrl]);
		delay_ms(delSetAccel);
	}

	// Cruise Time (no feedback signal)
	delay_ms(CruiseTime * 1000);

	// Deceleration
	for (i = DesiredSpeed; i<30; i++) {
		if (i<10) {
			canMessages[can_propulsion_ctrl].data[5] = '0';
			canMessages[can_propulsion_ctrl].data[6] = ((i % 10)+'0');
		} else {
			canMessages[can_propulsion_ctrl].data[5] = ((i/10)+'0');
			canMessages[can_propulsion_ctrl].data[6] = ((i % 10)+'0');
		}
		Can_Tx_Msg(&canMessages[can_propulsion_ctrl]);

		delay_ms(delSetBrake);
	}
	canMessages[can_propulsion_ctrl].data[5] = '0';
	canMessages[can_propulsion_ctrl].data[6] = '0';
	Can_Tx_Msg(&canMessages[can_propulsion_ctrl]);
	// Brake ON
	canMessages[can_brake_ctrl].data[5] = '2';
	canMessages[can_brake_ctrl].data[6] = '0';
	Can_Tx_Msg(&canMessages[can_brake_ctrl]);
	W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Cruise Complete", sizeof("Cruise Complete"));

}


void ClosedControl(char * command) {
	char * token;
	uint8_t AccelTime, DesiredSpeed, CruiseDistance, BrakingTime;
	token = strtok(command, "+");
	token = strtok(NULL, "+");
//	AccelTime = atoi(token);
	AccelTime = 4;		// Default value
//	token = strtok(NULL, "+");
	DesiredSpeed = atoi(token);
	token = strtok(NULL, "+");
	CruiseDistance = atoi(token);
	token = strtok(NULL, "+");
	BrakingTime = atoi(token);

	// Brake OFF
	canMessages[can_brake_ctrl].data[5] = '1';
	canMessages[can_brake_ctrl].data[6] = '0';
	Can_Tx_Msg(&canMessages[can_brake_ctrl]);
	delay_ms(4000);
	// Enable IO-Link sensor
	GPIOB->ODR |= GPIO_ODR_OD14;
	// Acceleration process
	unsigned int delSetAccel = ((30-DesiredSpeed)/AccelTime)*1000;
	unsigned int delSetBrake = ((30-DesiredSpeed)/BrakingTime)*1000;
	char pValDec, pValUni;
	uint8_t i;
	for (i = 30; i>DesiredSpeed; i--) {
		if (i<10) {
			pValDec = '0';
			pValUni = ((i % 10)+'0');

		} else {
			pValDec = ((i/10) + '0');
			pValUni = ((i % 10)+'0');
		}
		canMessages[can_propulsion_ctrl].data[5] = pValDec;
		canMessages[can_propulsion_ctrl].data[6] = pValUni;
		Can_Tx_Msg(&canMessages[can_propulsion_ctrl]);
		delay_ms(delSetAccel);
	}

	// Cruise Time with adjusted photoelectric feedback signal
	for (uint8_t d = 0; d < CruiseDistance; d++) {
		delay_ms(delSetAccel);
		GPIOC->ODR &= ~GPIO_ODR_OD11;
		if (!(GPIOA->IDR & GPIO_IDR_ID3)) {
			GPIOC->ODR |= GPIO_ODR_OD11;

		} else {
			delay_ms(delSetAccel);
			GPIOC->ODR |= GPIO_ODR_OD11;
			GPIOC->ODR &= ~GPIO_ODR_OD12;
			while(!(GPIOA->IDR & GPIO_IDR_ID3));	// Waiting for Navigation Aid pass
			while((GPIOA->IDR & GPIO_IDR_ID3));
		}
		GPIOC->ODR |= GPIO_ODR_OD11 | GPIO_ODR_OD12;
	}
	// Disable IO-Link sensor
	GPIOB->ODR &= ~GPIO_ODR_OD14;
	// Deceleration process
	for (i = DesiredSpeed; i<30; i++) {
		if (i<10) {
			canMessages[can_propulsion_ctrl].data[5] = '0';
			canMessages[can_propulsion_ctrl].data[6] = ((i % 10)+'0');
		} else {
			canMessages[can_propulsion_ctrl].data[5] = ((i/10)+'0');
			canMessages[can_propulsion_ctrl].data[6] = ((i % 10)+'0');
		}
		Can_Tx_Msg(&canMessages[can_propulsion_ctrl]);

		delay_ms(delSetBrake);
	}
	canMessages[can_propulsion_ctrl].data[5] = '0';
	canMessages[can_propulsion_ctrl].data[6] = '0';
	Can_Tx_Msg(&canMessages[can_propulsion_ctrl]);
	// Brake ON
	canMessages[can_brake_ctrl].data[5] = '2';
	canMessages[can_brake_ctrl].data[6] = '0';
	Can_Tx_Msg(&canMessages[can_brake_ctrl]);
	W6100_TransmitData(1, socket_dest_adr[1], (uint8_t*)"Cruise Complete", sizeof("Cruise Complete"));
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


void delay_ms(int ms) {
	sysTicks = 0;
	while(sysTicks < ms);
}


__attribute__((interrupt)) void SysTick_Handler(void){
	sysTicks++;
}



