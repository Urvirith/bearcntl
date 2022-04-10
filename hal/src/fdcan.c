/* FD Controller Area Network (FDCAN) */
#include "fdcan.h"

/* Register Masks */
/* NBTP */
#define TS2_MASK			MASK_7_BIT
#define TS1_MASK			MASK_8_BIT
#define BRP_MASK			MASK_9_BIT
#define SJW_MASK			MASK_7_BIT

/* Register Bits */
/* CCCR */
#define INIT				BIT_0
#define CCE					BIT_1
#define FDOE				BIT_8

/* Register Offsets */
/* NBTP */
#define TS2_OFFSET			0
#define TS1_OFFSET			8
#define BRP_OFFSET			16
#define SJW_OFFSET			25


/* Private Functions */
static inline void set_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw);

/* NEED TO MOVE THESE UP TO MAKE GENERIC */
/* CAN Speed Settings */
#define SJW              	2
#define BRP                 6
#define TS1                	11
#define TS2                	4

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

#define RXFIFO0_START       (FDCAN_RAM_BASE + FILTER_CNT_11BYTE * 4 + FILTER_CNT_29BYTE * 4)
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


void fdcan_open(FDCAN_TypeDef *ptr){
	uint32_t* ptr;
	//CLK Source

	set_ptr_vol_bit_u32(&ptr->CCCR, INIT);				// Initalize CAN
	while (!get_ptr_vol_bit_u32(&ptr->CCCR, INIT));		// Wait For CAN Initialized
	set_ptr_vol_bit_u32(&ptr->CCCR, CCE);				// Enable Configuration
	clr_ptr_vol_bit_u32(&ptr->CCCR, FDOE);				// Switch To Classic CAN	

	/* Clean-up memory */
	for(uint32_t* i=(uint32_t*)FDCAN_START_AD; i< (uint32_t*)FDCAN_END_AD; i++){
		*i=0;
	}

	/* Timings */
	set_timing_register(ptr, (TS2 - 1),  (TS1 - 1),  (BRP - 1),  (SJW - 1));

	/* Standard ID filter */
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
	ptr->SIDFC = 0;
	ptr->GFC = 0;
	#endif

	/* RX FIFO */	
    ptr->RXF0C  = (RXFIFO0_OFFSET << RXF0C_F0SA_Pos)|\
					 (RXFIFO0_CNT << RXF0C_F0S_Pos);
	
	/* Tx Event */
	ptr->TXEFC = 0; //No Event
	/* TX FIFO mode */
	ptr->TXBC  = (FDCAN_TXFIFO_CNT << FDCAN_TXBC_TFQS_Pos)|\
					(FDCAN_TXFIFO_OFFSET << FDCAN_TXBC_TBSA_Pos);
					
		
	ptr->IE |= FDCAN_IE_RF0NE;
	ptr->ILE |= FDCAN_ILE_EINT0;

	clr_ptr_vol_bit_u32(&ptr->CCCR, INIT);				// Normal Operation
	while (get_ptr_vol_bit_u32(&ptr->CCCR, INIT));		// Wait For Normal Operation
}

void fdcan_read(struct can_message* msg,uint8_t idx){
	struct can_fifo_element *fifo;

	fifo = (struct can_fifo_element *)(RXFIFO0_START + idx * RXFIFO_EL_SIZE);

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

void fdcan_write(struct can_message *msg){
	struct can_fifo_element *fifo;
	uint8_t tx_index;
	
	if ((FDCAN1->TXFQS & FDCAN_TXFQS_TFQF) != 0) {
		ERROR("TX FIFO full");
	};
	
	tx_index= (FDCAN1->TXFQS >> 16) & 0xF;

	fifo = (struct can_fifo_element *)(FDCAN_TXFIFO_START + tx_index * FDCAN_TXFIFO_EL_SIZE);
	

	fifo->b0 = (msg->msg_id << 18);
	fifo->b1 = (8 << 16);  //Data size
	fifo->b2 = (msg->msg[3] << 24)|(msg->msg[2] << 16)|(msg->msg[1] << 8)|msg->msg[0];
	fifo->b3 = (msg->msg[7] << 24)|(msg->msg[6] << 16)|(msg->msg[5] << 8)|msg->msg[4];

	FDCAN1->TXBAR |= (1 << tx_index);   
}


static inline void set_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw) {
	set_ptr_vol_u32(&ptr->NBTP, TS2_OFFSET, TS2_MASK, ts2);
	set_ptr_vol_u32(&ptr->NBTP, TS1_OFFSET, TS1_MASK, ts1);
	set_ptr_vol_u32(&ptr->NBTP, BRP_OFFSET, BRP_MASK, brp);
	set_ptr_vol_u32(&ptr->NBTP, SJW_OFFSET, SJW_MASK, sjw);
}