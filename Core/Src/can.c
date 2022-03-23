/*
 * can.c
 *
 *  Created on: 21 Nov 2021
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
//	CAN1->BTR |= CAN_BTR_LBKM;			// Loopback mode enabled (For Debug purposes)
	// ********************* //

	NVIC_EnableIRQ(CAN1_TX_IRQn);
	NVIC_EnableIRQ(CAN1_RX0_IRQn);

	CAN1->IER |= CAN_IER_FMPIE0;		// Interrupt generated when state of FMP[1:0] bits are not 00b

	CAN1->BTR = 0x00080004;				// CAN Bitrate: 500k


	CAN1->MCR &= ~CAN_MCR_INRQ;			// Initialisation mode off
	while (CAN1->MSR & CAN_MCR_INRQ);


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

	CAN1->sTxMailBox[0].TDLR = (((u_int)msg->data[3] << 24) |
	                          ((u_int)msg->data[2] << 16) |
	                          ((u_int)msg->data[1] <<  8) |
	                          ((u_int)msg->data[0]));
	CAN1->sTxMailBox[0].TDHR = (((u_int)msg->data[7] << 24) |
	                          ((u_int)msg->data[6] << 16) |
	                          ((u_int)msg->data[5] <<  8) |
	                          ((u_int)msg->data[4]));

	CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT1R_DLC;
	CAN1->sTxMailBox[0].TDTR |=  (msg->len & CAN_TDT0R_DLC);

	CAN1->IER |= CAN_IER_TMEIE;                      // enable  TME interrupt
	CAN1->sTxMailBox[0].TIR |=  CAN_TI0R_TXRQ;       // transmit message

}


void Can_Rx_Msg(CAN_MESSAGE * msg) {

	if ((CAN1->sFIFOMailBox[0].RIR & CAN_ID_EXT) == 0) { // Standard ID
	    msg->format = STANDARD_FORMAT;
	    msg->id     = (uint32_t)0x000007FF & (CAN1->sFIFOMailBox[0].RIR >> 21);
	  }  else  {                                          // Extended ID
	    msg->format = EXTENDED_FORMAT;
	    msg->id     = (uint32_t)0x0003FFFF & (CAN1->sFIFOMailBox[0].RIR >> 3);
	  }
	                                                  // Read type information
	  if ((CAN1->sFIFOMailBox[0].RIR & CAN_RTR_REMOTE) == 0) {
	    msg->type =   DATA_FRAME;                     // DATA   FRAME
	  }  else  {
	    msg->type = REMOTE_FRAME;                     // REMOTE FRAME
	  }

	  msg->len = (char)0x0000000F & CAN1->sFIFOMailBox[0].RDTR;
	                                                    // Read data
	  msg->data[0] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR);
	  msg->data[1] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR >> 8);
	  msg->data[2] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR >> 16);
	  msg->data[3] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR >> 24);

	  msg->data[4] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR);
	  msg->data[5] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR >> 8);
	  msg->data[6] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR >> 16);
	  msg->data[7] = (u_int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR >> 24);

	  CAN1->RF0R |= CAN_RF0R_RFOM0;                    // Release FIFO 0 output mailbox

}


void Can_Set_Filter(uint id, char format) {

	static unsigned short CAN_filterIdx = 0;
	uint CAN_msgId = 0;

	  if (CAN_filterIdx > 13) {                       // check if Filter Memory is full
	    return;
	  }
	                                                  // Setup identifier information
	  if (format == STANDARD_FORMAT)  {               // Standard ID
	      CAN_msgId  |= (unsigned int)(id << 21) | CAN_ID_STD;
	  }  else  {                                      // Extended ID
	      CAN_msgId  |= (unsigned int)(id <<  3) | CAN_ID_EXT;
	  }

	  CAN1->FMR  |=  CAN_FMR_FINIT;                    // set Initialisation mode for filter banks
	  CAN1->FA1R &=  ~(unsigned int)(1 << CAN_filterIdx); // deactivate filter

	                                                  // initialize filter
	  CAN1->FS1R |= (unsigned int)(1 << CAN_filterIdx);// set 32-bit scale configuration
	  CAN1->FM1R |= (unsigned int)(1 << CAN_filterIdx);// set 2 32-bit identifier list mode

	  CAN1->sFilterRegister[CAN_filterIdx].FR1 = CAN_msgId; //  32-bit identifier
	  CAN1->sFilterRegister[CAN_filterIdx].FR2 = CAN_msgId; //  32-bit identifier

	  CAN1->FFA1R &= ~(unsigned int)(1 << CAN_filterIdx);  // assign filter to FIFO 0
	  CAN1->FA1R  |=  (unsigned int)(1 << CAN_filterIdx);  // activate filter

	  CAN1->FMR &= ~CAN_FMR_FINIT;                     // reset Initialisation mode for filter banks

	  CAN_filterIdx += 1;                             // increase filter index

}


void registerCanMsgRxCallback(void (*callback)(CAN_MESSAGE msg)) {
	canMsgReceivedCallback = callback;
}

void registerCanMsgTxCallback(void (*callback)(void)) {
	canMsgTransmitCallback = callback;
}


__attribute__((interrupt)) void CAN1_TX_IRQHandler (void)  {
	if (CAN1->TSR & CAN_TSR_RQCP0) {                 // request completed mbx 0
	    CAN1->TSR |= CAN_TSR_RQCP0;                  // reset request complete mbx 0
	    CAN1->IER &= ~CAN_IER_TMEIE;                 // disable  TME interrupt
	}
	// Tx Interrupt Action
	if(canMsgTransmitCallback) canMsgTransmitCallback();
}



__attribute__((interrupt)) void CAN1_RX0_IRQHandler (void) {

	CAN_MESSAGE can_rx_message;

	if (CAN1->RF0R & CAN_RF0R_FMP0) {			      // message pending?
		Can_Rx_Msg(&can_rx_message);                  // read the message
		// Rx Interrupt Action
		if (canMsgReceivedCallback) canMsgReceivedCallback(can_rx_message);
	}

}



