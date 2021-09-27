/*
 * can.c
 *
 *  Created on: 21 Sep 2021
 *      Author: Michal Makowka
 */

#include "can.h"


void CanInit(void) {

	// *** Pin configuration and remap assignment *** //
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;		// Enable CAN 1 CLK

	GPIOB->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;	// PB8 and PB9 Alternative Function
	GPIOB->AFR[1] |= GPIO_AFRH_AFRH0_3 | GPIO_AFRH_AFRH0_0 | GPIO_AFRH_AFRH1_0 | GPIO_AFRH_AFRH1_3;		// Alternative function mapped to CAN1_Tx and CAN1_Rx

	CAN1->MCR |= CAN_MCR_INRQ;			// Initialisation mode on

	// *** CAN Test Mode *** //
	CAN1->BTR |= CAN_BTR_LBKM;			// Loopback mode enabled (For Debug purposes)
	// ********************* //


	CAN1->MCR &= ~CAN_MCR_INRQ;			// Initialisation mode off
	while (CAN1->MSR & CAN_MCR_INRQ);

	CAN1->BTR = 0x001c0001;				// CAN Bitrate: 2010000 (data for the logic analyser debug purposes)

	CAN1->MCR &= ~CAN_MCR_SLEEP;


}


void Can_Tx_Msg(CAN_MESSAGE * msg) {

	CAN1->sTxMailBox[0].TIR = 0;		// Empty TIR register

	if(msg->format == STANDARD_FORMAT)	{
		CAN1->sTxMailBox[0].TIR |= (u_int) (msg->id << 21) | CAN_ID_STD;
	} else {
		CAN1->sTxMailBox[0].TIR |= (u_int) (msg->id << 3) | CAN_ID_EXT;
	}

	if (msg->type == DATA_FRAME)  {                 // DATA FRAME
	    CAN1->sTxMailBox[0].TIR |= CAN_RTR_DATA;
	} else {                                        // REMOTE FRAME
	    CAN1->sTxMailBox[0].TIR |= CAN_RTR_REMOTE;
	}

	CAN1->sTxMailBox[0].TDLR = (((unsigned int)msg->data[3] << 24) |
	                          ((unsigned int)msg->data[2] << 16) |
	                          ((unsigned int)msg->data[1] <<  8) |
	                          ((unsigned int)msg->data[0]));
	CAN1->sTxMailBox[0].TDHR = (((unsigned int)msg->data[7] << 24) |
	                          ((unsigned int)msg->data[6] << 16) |
	                          ((unsigned int)msg->data[5] <<  8) |
	                          ((unsigned int)msg->data[4]));

	CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT1R_DLC;
	CAN1->sTxMailBox[0].TDTR |=  (msg->len & CAN_TDT0R_DLC);

	CAN1->IER |= CAN_IER_TMEIE;                      // enable  TME interrupt
	CAN1->sTxMailBox[0].TIR |=  CAN_TI0R_TXRQ;       // transmit message

}



