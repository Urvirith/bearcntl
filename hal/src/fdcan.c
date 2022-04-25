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

/* TXFQS */
#define TFFL_MASK			MASK_3_BIT
#define TFGI_MASK			MASK_2_BIT

/* Register Bits */
/* TEST */
#define LBCK				BIT_4

/* CCCR */
#define INIT				BIT_0
#define CCE					BIT_1
#define MON					BIT_5
#define DAR					BIT_6
#define TESTB				BIT_7
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

/* TXFQS */
#define TFFL_OFFSET			0
#define TFGI_OFFSET			8

/* FDCAN RAM Declarations */
#define LENU32				(u32)4				// Bytes In A u32 - Element Is Always 4 Bytes Long

/* RAM Setup */
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

/* Private Functions */
static inline void set_nominal_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw);
static inline void set_databit_timing_register(FDCAN_TypeDef *ptr, u32 ts2, u32 ts1, u32 brp, u32 sjw);
static inline void calculate_ram_addresses(FDCAN_TypeDef *ptr, u32 ram_base);
static inline u32 calc_bytes(u32 dlc);

/* NEED TO MOVE THESE UP TO MAKE GENERIC */
/* CAN Speed Settings */
#define SJW              	2
#define BRP                 6
#define TS1                	11
#define TS2                	4

// Open the driver
void fdcan_open(FDCAN_TypeDef *ptr, u32 ram_base) {
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
	
	set_ptr_vol_bit_u32(&ptr->CCCR, MON);
	set_ptr_vol_bit_u32(&ptr->CCCR, TESTB);
	set_ptr_vol_bit_u32(&ptr->TEST, LBCK);

	set_nominal_timing_register(ptr, TS2, TS1, BRP, SJW);	// Set Timing for Standard CAN arbitration and bit rate

	calculate_ram_addresses(ptr, ram_base);

	clr_ptr_vol_bit_u32(&ptr->CCCR, INIT);					// Normal Operation
	while (get_ptr_vol_bit_u32(&ptr->CCCR, INIT));			// Wait For Normal Operation
}

bool fdcan_read(FDCAN_TypeDef *ptr, FDCANRAM_TypeDef *ram, FDCANMsgRX_TypeDef *msg) {
	u32 ind = 0;	// Index Fetched
	u32 offset = 0;
	u32 reg = 0;
	u32 elm = 0;
	u32 bytes = 0; 

	if (get_ptr_vol_u32(&ptr->RXF0S, FxFL_OFFSET, FxFL_MASK) > 0U) {
		ind = get_ptr_vol_u32(&ptr->RXF0S, FxGI_OFFSET, FxGI_MASK);
		offset = ind * RAM_RF0_LEN;
		msg->HEADER.reg = get_ptr_vol_raw_u32(&ram->RF0[offset]);
		offset += 1;
		msg->RX.reg = get_ptr_vol_raw_u32(&ram->RF0[offset]);
		offset += 1;
		bytes = calc_bytes(msg->RX.fields.DLC); // Get the number of bytes transfered

		// Verify the number of elements in system
		if ((bytes % 4) == 0) {
			elm = bytes / LENU32;
		} else {
			elm = (bytes / LENU32) + 1;
		}

		for (u32 i = 0; i < elm; i++) {  // NEED TO CHANGE TO MODULUS TO ROUND OUT DATA
			if ((i + offset) >= ((ind * RAM_RF0_LEN) + RAM_RF0_LEN)) { 
				return false;
			}

			reg = get_ptr_vol_raw_u32(&ram->RF0[i + offset]);
			msg->DATA[(i * LENU32) + 0] = (reg >> 0) & MASK_8_BIT;
			msg->DATA[(i * LENU32) + 1] = (reg >> 8) & MASK_8_BIT;
			msg->DATA[(i * LENU32) + 2] = (reg >> 16) & MASK_8_BIT;
			msg->DATA[(i * LENU32) + 3] = (reg >> 24) & MASK_8_BIT;
		}
		return true;
	} else if (get_ptr_vol_u32(&ptr->RXF1S, FxFL_OFFSET, FxFL_MASK) > 0U) {
		ind = get_ptr_vol_u32(&ptr->RXF1S, FxGI_OFFSET, FxGI_MASK);
		offset = ind * RAM_RF1_LEN;
		msg->HEADER.reg = get_ptr_vol_raw_u32(&ram->RF1[offset]);
		offset += 1;
		msg->RX.reg = get_ptr_vol_raw_u32(&ram->RF1[offset]);
		offset += 1;
		bytes = calc_bytes(msg->RX.fields.DLC); // Get the number of bytes transfered

		// Verify the number of elements in system
		if ((bytes % 4) == 0) {
			elm = bytes / LENU32;
		} else {
			elm = (bytes / LENU32) + 1;
		}
		for (u32 i = 0; i < elm; i++) {  // NEED TO CHANGE TO MODULUS TO ROUND OUT DATA
			if ((i + offset) >= ((ind * RAM_RF1_LEN) + RAM_RF1_LEN)) { 
				return false;
			}

			reg = get_ptr_vol_raw_u32(&ram->RF1[i + offset]);
			msg->DATA[(i * LENU32) + 0] = (u8)((reg >> 0) & MASK_8_BIT);
			msg->DATA[(i * LENU32) + 1] = (u8)((reg >> 8) & MASK_8_BIT);
			msg->DATA[(i * LENU32) + 2] = (u8)((reg >> 16) & MASK_8_BIT);
			msg->DATA[(i * LENU32) + 3] = (u8)((reg >> 24) & MASK_8_BIT);
		}
		return true;
	} else {
		return false;
	}
}

bool fdcan_write(FDCAN_TypeDef *ptr, FDCANRAM_TypeDef *ram, FDCANMsgTX_TypeDef *msg) {
	u32 ind = 0;	// Index Fetched
	u32 offset = 0;
	u32 reg = 0;
	u32 elm = 0;
	u32 bytes = 0; 
	
	if (get_ptr_vol_u32(&ptr->TXFQS, TFFL_OFFSET, TFFL_MASK) >= RAM_TFQ_ELM) { // TX FIFO Full, Return
		return false;
	}

	ind = get_ptr_vol_u32(&ptr->TXFQS, TFGI_OFFSET, TFGI_MASK);
	offset = ind * RAM_TFQ_LEN;
	set_ptr_vol_raw_u32(&ram->TFQ[offset], msg->HEADER.reg);
	offset += 1;
	set_ptr_vol_raw_u32(&ram->TFQ[offset], msg->TX.reg);
	offset += 1;
	bytes = calc_bytes(msg->TX.fields.DLC); // Get the number of bytes transfered

	// Verify the number of elements in system
	if ((bytes % 4) == 0) {
		elm = bytes / LENU32;
	} else {
		elm = (bytes / LENU32) + 1;
	}

	for (u32 i = 0; i < elm; i++) {  // NEED TO CHANGE TO MODULUS TO ROUND OUT DATA
		if ((i + offset) >= ((ind * RAM_TFQ_LEN) + RAM_TFQ_LEN)) { 
			return false;
		}

		reg |= (u32)(msg->DATA[(i * LENU32) + 0]) << 0;
		reg |= (u32)(msg->DATA[(i * LENU32) + 1]) << 8;
		reg |= (u32)(msg->DATA[(i * LENU32) + 2]) << 16;
		reg |= (u32)(msg->DATA[(i * LENU32) + 3]) << 24;
		set_ptr_vol_raw_u32(&ram->TFQ[i + offset], reg);
	}

	set_ptr_vol_bit_u32(&ptr->TXBAR, 1 << ind);

	return true;   
}

// Clears an error or status bit
void fdcan_clr_ir(FDCAN_TypeDef *ptr, u32 bit) {
	set_ptr_vol_bit_u32(&ptr->IR, bit);
}

// Gets an error or status bit 
void fdcan_get_ir(FDCAN_TypeDef *ptr, u32 bit) {
	get_ptr_vol_bit_u32(&ptr->IR, bit);
}

// Gets an error or status bit 
void fdcan_set_ir(FDCANRAM_TypeDef *ptr, u32 ind, FDCANSTDFilter_TypeDef std) {
	if (ind < RAM_FLS_ELM) {
		set_ptr_vol_raw_u32(&ptr->FLS[ind], std.reg);
	}
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
static inline void calculate_ram_addresses(FDCAN_TypeDef *ptr, u32 ram_base) {
//static inline void calculate_ram_addresses(FDCAN_TypeDef *ptr, FDCANRamAddress_TypeDef *ram, u32 ram_base) {
	// Standard Filter Elements and Extended Filter  
	set_ptr_vol_u32(&ptr->RXCFG, LSS_OFFSET, LSS_MASK, RAM_FLS_ELM);
	set_ptr_vol_u32(&ptr->RXCFG, LSE_OFFSET, LSE_MASK, RAM_FLE_ELM);

	/*
	ram->STDFilterSA = ram_base + RAM_FLSSA;		// Standard Filter Base Address
	ram->EXTFilterSA = ram_base + RAM_FLESA;		// Extended Filter Base Address
	ram->RXFIFO0SA = ram_base + RAM_RF0SA;			// RX FIFO 0 Base Address
	ram->RXFIFO0SA = ram_base + RAM_RF1SA;			// RX FIFO 1 Base Address
	ram->TXEventFIFOSA = ram_base + RAM_TEFSA;		// TX event FIFO Base Address
	ram->TXFIFOQSA = ram_base + RAM_TFQSA;			// TX FIFO/Queue Base Address
	*/

	// Clear Ram Buffer 
	for (u32 RAMcounter = ram_base; RAMcounter < (ram_base + RAM_SIZE); RAMcounter += LENU32) {
		*(volatile u32*)(RAMcounter) = (u32)0x00000000;
	}
}

/* 
	Data Length Code: 
	Classic CAN 0 - 8 Data Bytes 
	FDCAN 9 - 15 = 12/16/20/24/32/48/64 Data Bytes
*/
static inline u32 calc_bytes(u32 dlc) {
	switch(dlc) {
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return 4;
		case 5:
			return 5;
		case 6:
			return 6;
		case 7:
			return 7;
		case 8:
			return 8;
		case 9:
			return 12;
		case 10:
			return 16;
		case 11:
			return 20;
		case 12:
			return 24;
		case 13:
			return 32;
		case 14:
			return 48;
		case 15:
			return 64;
	}
}