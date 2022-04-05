/* FD Controller Area Network (FDCAN) */
#include "fdcan.h"


#define START_ADDR FDCAN_RAM_BASE

/* CAN Speed Settings */
#define PSC                 6
#define JW                  2
#define SEG1                11
#define SEG2                4

#define FILTER_CNT_11BYTE   2
#define FILTER_CNT_29BYTE   0
#define RXFIFO0_CNT         10
#define RXFIFO1_CNT         0
#define TX_EVENT_FIFO_CNT   0
#define TXFIFO_CNT          10

#define RXFIFO_HEAD         8 
#define RXFIFO_DATA_SIZE    8
#define RXFIFO_EL_SIZE      (RXFIFO_HEAD + RXFIFO_DATA_SIZE)
#define RXFIFO_EL_W_SIZE    (RXFIFO_EL_SIZE / 4)

#define RXFIFO0_START       (START_ADDR + FILTER_CNT_11BYTE * 4 + FILTER_CNT_29BYTE * 4)
#define RXFIFO0_OFFSET      (FILTER_CNT_11BYTE + FILTER_CNT_29BYTE)

#define TXFIFO_START        (RXFIFO0_START + RXFIFO0_CNT * RXFIFO_EL_SIZE)
#define TXFIFO_OFFSET       (RXFIFO0_OFFSET + RXFIFO0_CNT * 4)

#define TXFIFO_HEAD         8 
#define TXFIFO_DATA_SIZE    8
#define TXFIFO_EL_SIZE      (TXFIFO_HEAD + TXFIFO_DATA_SIZE)
#define TXFIFO_EL_W_SIZE    (TXFIFO_EL_SIZE / 4)


/* Filter */
#define FILTER_TYPE_RANGE   0
#define FILTER_TYPE_DUAL    1
#define FILTER_TYPE_CLASSIC 2
#define FILTER_TYPE_DISABLE 3

#define FILTER_STORE_FIFO0  1
#define FILTER_STORE_FIFO1  2
#define FILTER_REJECT       3


void can1_init(void){
	uint32_t* ptr;

	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN; 
	//MODE - 10 AF
	GPIOB->MODER &= ~(GPIO_MODER_MODER8|GPIO_MODER_MODER9);
    GPIOB->MODER |= (GPIO_MODER_MODER8_1|GPIO_MODER_MODER9_1);

	//Push-pull mode 0
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_8|GPIO_OTYPER_OT_9);
	//Very High speed 11
	
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9);
    GPIOB->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR8_Pos)|\
					  (S_VH << GPIO_OSPEEDER_OSPEEDR9_Pos);

	GPIOB->AFR[1] &= GPIO_AFRH_AFRH8|GPIO_AFRH_AFRH9;
    GPIOB->AFR[1] |= (9 << GPIO_AFRH_AFRH8_Pos)|\
					 (9 << GPIO_AFRH_AFRH9_Pos);
	//CLK Source
    RCC->D2CCIP1R |= RCC_D2CCIP1R_FDCANSEL_0; //PLL1Q  
	RCC->APB1HENR |= RCC_APB1HENR_FDCANEN;
    
    FDCAN1->CCCR   |= FDCAN_CCCR_INIT;     //Initialize the CAN module
	while( FDCAN_CCCR_INIT == 0);         // wait 
    FDCAN1->CCCR   |= FDCAN_CCCR_CCE;     //CCE bit enabled to start configuration
    FDCAN1->CCCR &= ~ (FDCAN_CCCR_FDOE);  //Classic CAN
	

	/* Clean-up memory */
	for(uint32_t* i=(uint32_t*)FDCAN_START_AD; i< (uint32_t*)FDCAN_END_AD; i++){
		*i=0;
	}

	/* Timings */
	FDCAN1->NBTP = ((CAN_JW-1) << FDCAN_NBTP_NSJW_Pos)|\
				   ((CAN_PSC-1) << FDCAN_NBTP_NBRP_Pos)|\
				   ((CAN_SEG1-1) << FDCAN_NBTP_NTSEG1_Pos)|\
				   ((CAN_SEG2-1) << FDCAN_NBTP_NTSEG2_Pos);

	/* Standart ID filter, старт с 0 смещения памяти */
	#ifdef CAN_ENABLE_FILTERS
	FDCAN1->GFC = FDCAN_GFC_ANFS|FDCAN_GFC_ANFE|FDCAN_GFC_RRFS|FDCAN_GFC_RRFE;
	FDCAN1->SIDFC = (FDCAN_11B_FILTER_CNT << FDCAN_SIDFC_LSS_Pos);
	ptr=(uint32_t*)FDCAN_START_AD;
	*ptr=(FILTER_TYPE_DUAL << 30)|\
		 (FILTER_STORE_FIFO0 << 27)|\
		 (OBD_ANS << 16)|\
		 (ABS_ANS);
	ptr++;
	*ptr=(FILTER_TYPE_DUAL << 30)|\
		 (FILTER_STORE_FIFO0 << 27)|\
		 (SWA_ANS << 16)|\
		 (TEST_ANS);
	#else
	FDCAN1->SIDFC = 0;
	FDCAN1->GFC = 0;
	#endif

	/* RX FIFO */	
    FDCAN1->RXF0C  = (FDCAN_RXFIFO0_OFFSET << FDCAN_RXF0C_F0SA_Pos)|\
					 (FDCAN_RXFIFO0_CNT << FDCAN_RXF0C_F0S_Pos);
	
	/* Tx Event */
	FDCAN1->TXEFC = 0; //No Event
	/* TX FIFO mode */
	FDCAN1->TXBC  = (FDCAN_TXFIFO_CNT << FDCAN_TXBC_TFQS_Pos)|\
					(FDCAN_TXFIFO_OFFSET << FDCAN_TXBC_TBSA_Pos);
					
		
	FDCAN1->IE |= FDCAN_IE_RF0NE;
	FDCAN1->ILE |= FDCAN_ILE_EINT0;

	FDCAN1->CCCR   &= ~(FDCAN_CCCR_INIT);
	NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
}

void get_msg(struct can_message* msg,uint8_t idx){
	struct can_fifo_element *fifo;

	fifo = (struct can_fifo_element *)(FDCAN_RXFIFO0_START + idx * FDCAN_RXFIFO_EL_SIZE);

	msg->msg_id= (fifo->b0 >> 18) & 0x7FF;
	msg->msg[0] = (uint8_t)(fifo->b2 & 0xFF);
	msg->msg[1] = (uint8_t)((fifo->b2 >> 8) & 0xFF);
	msg->msg[2] = (uint8_t)((fifo->b2 >> 16) & 0xFF);
	msg->msg[3] = (uint8_t)((fifo->b2 >> 24) & 0xFF);

	msg->msg[4] = (uint8_t)(fifo->b3 & 0xFF);
	msg->msg[5] = (uint8_t)((fifo->b3 >> 8) & 0xFF);
	msg->msg[6] = (uint8_t)((fifo->b3 >> 16) & 0xFF);
	msg->msg[7] = (uint8_t)((fifo->b3 >> 24) & 0xFF);



}

void FDCAN1_IT0_IRQHandler(void) {
	uint8_t rx_index;
	struct can_message msg;
   
	if((FDCAN1->IR & FDCAN_IR_RF0N) != 0){
		FDCAN1->IR = FDCAN_IR_RF0N;
		rx_index= (uint8_t)((FDCAN1->RXF0S >> 8) & 0x1F);
		get_msg(&msg,rx_index);
		DEBUG("Idx: %d MSG ID: %X DATA: %X %X %X %X %X %X %X %X",rx_index,msg.msg_id,msg.msg[0],msg.msg[1],msg.msg[2],msg.msg[3],msg.msg[4],msg.msg[5],msg.msg[6],msg.msg[7]);

		FDCAN1->RXF0A = rx_index;
		parse_msg(&msg);
	};
	
	if((FDCAN1->IR & FDCAN_IR_RF0L) != 0){
			FDCAN1->IR = FDCAN_IR_RF0L;
			WARNING("CAN RX IRQ, lost message");
	};

	if((FDCAN1->IR & FDCAN_IR_RF0F) != 0){
			WARNING("CAN RX IRQ FIFO full");
			while(FDCAN1->RXF0S & FDCAN_RXF0S_F0FL_Msk){
				rx_index= (uint8_t)((FDCAN1->RXF0S >> 8) & 0x1F);
				get_msg(&msg,rx_index);
				DEBUG("Idx: %d MSG ID: %X DATA: %X %X %X %X %X %X %X %X",rx_index,msg.msg_id,msg.msg[0],msg.msg[1],msg.msg[2],msg.msg[3],msg.msg[4],msg.msg[5],msg.msg[6],msg.msg[7]);

				FDCAN1->RXF0A = rx_index;
				parse_msg(&msg);
			}
			FDCAN1->IR = FDCAN_IR_RF0F;
	};


}


void cansend(struct can_message *msg){
	struct can_fifo_element *fifo;
	uint8_t tx_index;
	
	if ((FDCAN1->TXFQS & FDCAN_TXFQS_TFQF) != 0) {
		ERROR("TX FIFO full");
	};
	
	tx_index= (FDCAN1->TXFQS >> 16) & 0xF;

	DEBUG("TXFIFO index %d ID: %X",tx_index,msg->msg_id);

	fifo = (struct can_fifo_element *)(FDCAN_TXFIFO_START + tx_index * FDCAN_TXFIFO_EL_SIZE);
	

	fifo->b0 = (msg->msg_id << 18);
	fifo->b1 = (8 << 16);  //Data size
	fifo->b2 = (msg->msg[3] << 24)|(msg->msg[2] << 16)|(msg->msg[1] << 8)|msg->msg[0];
	fifo->b3 = (msg->msg[7] << 24)|(msg->msg[6] << 16)|(msg->msg[5] << 8)|msg->msg[4];

	FDCAN1->TXBAR |= (1 << tx_index);   
}


void parse_msg(struct can_message *msg){
	switch (msg->msg_id){
		case OBD_ANS:
					switch (msg->msg[2]){
						case ENGINE_RPM:
										can_data.rpm=(((uint16_t)msg->msg[3] << 8)+msg->msg[4])/4;
										break;
						case ENGINE_COOLANT_TEMP:
										can_data.coolant_temp=msg->msg[3]-40;
										break;
						case SPEED:
										can_data.speed=msg->msg[3];
										break;
						case DROS:
										can_data.dros=msg->msg[4]/2;
										break;
						default :
										break;  
					}
					can_data.iqr_call_count++;
					can_send_next();
				break;  

		default :
				break;  
	};

	can_data.iqr_overall_call_count++;
}