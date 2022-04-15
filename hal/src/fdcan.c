/* FD Controller Area Network (FDCAN) */
#include "fdcan.h"

/* Register Masks */
/* NBTP */
#define NTS2_MASK			MASK_7_BIT
#define NTS1_MASK			MASK_8_BIT
#define NBRP_MASK			MASK_9_BIT
#define NSJW_MASK			MASK_7_BIT

/* DBTP */
#define DSJW_MASK			MASK_4_BIT
#define DTS2_MASK			MASK_4_BIT
#define DTS1_MASK			MASK_5_BIT
#define DBRP_MASK			MASK_5_BIT

/* RXCFG */
#define LSS_MASK			MASK_5_BIT
#define LSE_MASK			MASK_4_BIT

/* IR */
#define IR_MASK				MASK_24_BIT

/* RXFxS */
#define FxFL_MASK			MASK_4_BIT
#define FxGI_MASK			MASK_2_BIT
#define FxPI_MASK			MASK_2_BIT

/* RXFxA */
#define FxAI_MASK			MASK_3_BIT

/* Register Bits */
/* CCCR */
#define INIT				BIT_0
#define CCE					BIT_1
#define DAR					BIT_6
#define FDOE				BIT_8
#define BRSE				BIT_9
#define PXHD				BIT_12

/* DBTP */
#define TDC					BIT_23

/* TEST */
#define TDC					BIT_23

/* TXBC */
#define TFQM				BIT_24

/* IR */
#define RF0N				BIT_0
#define RF0F				BIT_1
#define RF0L				BIT_2
#define RF1N				BIT_3
#define RF1F				BIT_4
#define RF1L				BIT_5
#define HPM					BIT_6
#define TC					BIT_7
#define TCF					BIT_8
#define TFE					BIT_9
#define TEFN				BIT_10
#define TEFF				BIT_11
#define TEFL				BIT_12
#define TSW					BIT_13
#define MRAF				BIT_14
#define TOO					BIT_15

/* RXFxS */
#define FxF					BIT_24
#define RFxL				BIT_25
/* Register Offsets */
/* NBTP */
#define NTS2_OFFSET			0
#define NTS1_OFFSET			8
#define NBRP_OFFSET			16
#define NSJW_OFFSET			25

/* DBTP */
#define DSJW_OFFSET			0
#define DTS2_OFFSET			4
#define DTS1_OFFSET			8
#define DBRP_OFFSET			16

/* RXCFG */
#define LSS_OFFSET			16
#define LSE_OFFSET			24

/* IR */
#define IR_OFFSET			0

/* RXFxS */
#define FxFL_OFFSET			0
#define FxGI_OFFSET			8
#define FxPI_OFFSET			16

/* FDCAN RAM Declarations */
#define LENU32				(u32)4				// Bytes In A u32 - Element Is Always 4 Bytes Long

/* RAM Setup */
/* RAM Number Of Elements */
#define RAM_FLS_ELM        	(u32)28         	// Max. Filter List Standard Number
#define RAM_FLE_ELM         (u32)8          	// Max. Filter List Extended Number
#define RAM_RF0_ELM         (u32)3          	// RX FIFO 0 Elements Number
#define RAM_RF1_ELM         (u32)3          	// RX FIFO 1 Elements Number
#define RAM_TEF_ELM         (u32)3         		// TX Event FIFO Elements Number
#define RAM_TFQ_ELM         (u32)3         		// TX FIFO/Queue Elements Number

/* RAM Length Of Elements */
#define RAM_FLS_LEN        	(u32)1  			// Filter Standard Element Size in bytes
#define RAM_FLE_LEN        	(u32)2  			// Filter Extended Element Size in bytes
#define RAM_RF0_LEN        	(u32)18  			// RX FIFO 0 Elements Size in bytes
#define RAM_RF1_LEN        	(u32)18  			// RX FIFO 1 Elements Size in bytes
#define RAM_TEF_LEN        	(u32)2  			// TX Event FIFO Elements Size in bytes
#define RAM_TFQ_LEN        	(u32)18  			// TX FIFO/Queue Elements Size in bytes

/* RAM Length Of Elements Per Byte */
#define RAM_FLS_SIZE        (u32)(1  * LENU32)  // Filter Standard Element Size in bytes
#define RAM_FLE_SIZE        (u32)(2  * LENU32)  // Filter Extended Element Size in bytes
#define RAM_RF0_SIZE        (u32)(18 * LENU32)  // RX FIFO 0 Elements Size in bytes
#define RAM_RF1_SIZE        (u32)(18 * LENU32)  // RX FIFO 1 Elements Size in bytes
#define RAM_TEF_SIZE        (u32)(2  * LENU32)  // TX Event FIFO Elements Size in bytes
#define RAM_TFQ_SIZE        (u32)(18 * LENU32)  // TX FIFO/Queue Elements Size in bytes

/* RAM Start Addresses */
#define RAM_FLSSA 			(u32)0                                          	// Filter List Standard Start Address
#define RAM_FLESA 			(u32)(RAM_FLSSA + (RAM_FLS_ELM * RAM_FLS_SIZE)) 		// Filter List Extended Start Address
#define RAM_RF0SA 			(u32)(RAM_FLESA + (RAM_FLE_ELM * RAM_FLE_SIZE)) 		// RX FIFO 0 Start Address
#define RAM_RF1SA 			(u32)(RAM_RF0SA + (RAM_RF0_ELM * RAM_RF0_SIZE)) 		// RX FIFO 1 Start Address
#define RAM_TEFSA 			(u32)(RAM_RF1SA + (RAM_RF1_ELM * RAM_RF1_SIZE)) 		// TX Event FIFO Start Address
#define RAM_TFQSA 			(u32)(RAM_TEFSA + (RAM_TEF_ELM * RAM_TEF_SIZE)) 		// TX FIFO/Queue Start Address
#define RAM_SIZE  			(u32)(RAM_TFQSA + (RAM_TFQ_ELM * RAM_TFQ_SIZE)) 		// Message RAM size

/* RAM Arrays */
#define RAM

// MOVE OUT AFTER TESTING
typedef struct {
	u32 FLS[RAM_FLS_ELM * RAM_FLS_LEN];
	u32 FLE[RAM_FLE_ELM * RAM_FLE_LEN];
	u32 RF0[RAM_RF0_ELM * RAM_RF0_LEN];
	u32 RF1[RAM_RF1_ELM * RAM_RF1_LEN];
	u32 TEF[RAM_TEF_ELM * RAM_TEF_LEN];
	u32 TFQ[RAM_TFQ_ELM * RAM_TFQ_LEN];
} FDCANRAM_TypeDef;

/* Private Functions */
static inline void set_nominal_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw);
static inline void set_databit_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw);
static inline void calculate_ram_addresses(FDCAN_TypeDef *ptr, FDCANRamAddress_TypeDef *ram);

/* NEED TO MOVE THESE UP TO MAKE GENERIC */
/* CAN Speed Settings */
#define SJW              	2
#define BRP                 6
#define TS1                	11
#define TS2                	4


void fdcan_open(FDCAN_TypeDef *ptr){
	set_ptr_vol_bit_u32(&ptr->CCCR, INIT);					// Initalize CAN
	while (!get_ptr_vol_bit_u32(&ptr->CCCR, INIT));			// Wait For CAN Initialized
	set_ptr_vol_bit_u32(&ptr->CCCR, CCE);					// Enable Configuration
	clr_ptr_vol_bit_u32(&ptr->CCCR, FDOE);					// Switch To Classic CAN
	clr_ptr_vol_bit_u32(&ptr->CCCR, BRSE);					// Disable Flexible Data Rate	
	clr_ptr_vol_bit_u32(&ptr->CCCR, DAR);					// Enable Auto Retransmit
	clr_ptr_vol_bit_u32(&ptr->CCCR, PXHD);					// Enable Protocol Exception

	/* Set FDCAN Operating Mode:
					| Normal | Restricted |    Bus     | Internal | External
					|        | Operation  | Monitoring | LoopBack | LoopBack
		CCCR.TEST 	|   0    |     0      |     0      |    1     |    1
		CCCR.MON  	|   0    |     0      |     1      |    1     |    0
		TEST.LBCK 	|   0    |     0      |     0      |    1     |    1
		CCCR.ASM  	|   0    |     1      |     0      |    0     |    0
	*/
	// Setup a section when wanting to test only

	set_nominal_timing_register(ptr, TS2, TS1, BRP, SJW);	// Set Timing for Standard CAN arbitration and bit rate

	clr_ptr_vol_bit_u32(&ptr->CCCR, INIT);					// Normal Operation
	while (get_ptr_vol_bit_u32(&ptr->CCCR, INIT));			// Wait For Normal Operation
}

bool fdcan_read(FDCAN_TypeDef *ptr, FDCANRAM_TypeDef *ram, FDCANMsgRX_TypeDef *msg) {
	u32 ind = 0;	// Index Fetched
	u32 offset = 0;
	u32 reg = 0; 

	if (get_ptr_vol_u32(&ptr->RXF0S, FxFL_OFFSET, FxFL_MASK) > 0U) {
		ind = get_ptr_vol_u32(&ptr->RXF0S, FxGI_OFFSET, FxGI_MASK);
		offset = ind * RAM_RF0_LEN;
		msg->HEADER.reg = get_ptr_vol_raw_u32(&ram->RF0[offset]);
		offset += 1;
		msg->RX.reg = get_ptr_vol_raw_u32(&ram->RF0[offset]);
		offset += 1;
		for (u32 i = 0; i < msg->RX.fields.DLC / LENU32; i++) {  // NEED TO CHANGE TO MODULUS TO ROUND OUT DATA
			reg = get_ptr_vol_raw_u32(&ram->RF0[i + offset]);
			msg->DATA[(i * LENU32) + 0] = (reg >> 0) & MASK_8_BIT;
			msg->DATA[(i * LENU32) + 1] = (reg >> 8) & MASK_8_BIT;
			msg->DATA[(i * LENU32) + 2] = (reg >> 16) & MASK_8_BIT;
			msg->DATA[(i * LENU32) + 3] = (reg >> 24) & MASK_8_BIT;
		}
		return true;
	} else if (get_ptr_vol_u32(ptr->RXF1S, FxFL_OFFSET, FxFL_MASK) > 0U) {
		ind = get_ptr_vol_u32(&ptr->RXF1S, FxGI_OFFSET, FxGI_MASK);
	} else {
		return false;
	}
}

void fdcan_write(FDCAN_TypeDef *ptr, FDCANRAM_TypeDef *ram, FDCANMsgRX_TypeDef *msg){
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

// Clears an error or status bit
void fdcan_clr_ir(FDCAN_TypeDef *ptr, u32 bit) {
	set_ptr_vol_bit_u32(&ptr->IR, bit);
}

// Gets an error or status bit 
void fdcan_get_ir(FDCAN_TypeDef *ptr, u32 bit) {
	get_ptr_vol_bit_u32(&ptr->IR, bit);
}

// Used For Arbitration for FDCAN and CAN and data rate timing in CAN
static inline void set_nominal_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw) {
	set_ptr_vol_u32(&ptr->NBTP, NTS2_OFFSET, NTS2_MASK, ts2 - 1);
	set_ptr_vol_u32(&ptr->NBTP, NTS1_OFFSET, NTS1_MASK, ts1 - 1);
	set_ptr_vol_u32(&ptr->NBTP, NBRP_OFFSET, NBRP_MASK, brp - 1);
	set_ptr_vol_u32(&ptr->NBTP, NSJW_OFFSET, NSJW_MASK, sjw - 1);
}

// Used For FDCAN flexible data rate timing
static inline void set_databit_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw) {
	set_ptr_vol_u32(&ptr->DBTP, DSJW_OFFSET, DSJW_MASK, sjw - 1);
	set_ptr_vol_u32(&ptr->DBTP, DTS2_OFFSET, DTS2_MASK, ts2 - 1);
	set_ptr_vol_u32(&ptr->DBTP, DTS1_OFFSET, DTS1_MASK, ts1 - 1);
	set_ptr_vol_u32(&ptr->DBTP, DBRP_OFFSET, DBRP_MASK, brp - 1);
}

// Calculates Base Addresses For Use In Read And Writing RAM
static inline void calculate_ram_addresses(FDCAN_TypeDef *ptr, FDCANRamAddress_TypeDef *ram) {
	u32 ram_base = SRAMCAN_BASE;

	// Standard Filter Elements and Extended Filter  
	set_ptr_vol_u32(ptr->RXCFG, LSS_OFFSET, LSS_MASK, RAM_FLS_ELM);
	set_ptr_vol_u32(ptr->RXCFG, LSE_OFFSET, LSE_MASK, RAM_FLE_ELM);

	ram->STDFilterSA = ram_base + RAM_FLSSA;		// Standard Filter Base Address
	ram->EXTFilterSA = ram_base + RAM_FLESA;		// Extended Filter Base Address
	ram->RXFIFO0SA = ram_base + RAM_RF0SA;			// RX FIFO 0 Base Address
	ram->RXFIFO0SA = ram_base + RAM_RF1SA;			// RX FIFO 1 Base Address
	ram->TXEventFIFOSA = ram_base + RAM_TEFSA;		// TX event FIFO Base Address
	ram->TXFIFOQSA = ram_base + RAM_TFQSA;			// TX FIFO/Queue Base Address

	// Clear Ram Buffer 
	for (u32 RAMcounter = ram_base; RAMcounter < (ram_base + RAM_SIZE); RAMcounter += LENU32) {
		*(u32*)(RAMcounter) = (u32)0x00000000;
	}
}