#ifndef FDCAN_H_
#define FDCAN_H_

#include "common.h"
#include "pointer.h"

// FDCAN DRIVER
// ARM FDCAN MODULE
// FDCAN Description - is on pg 1883
// FDCAN Registers (Programming Manual) - is on 1913

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

/* FD Controller Area Network (FDCAN) */
typedef struct {
	volatile u32   	CREL;			// Interrupt Set-Enable register
	volatile u32   	ENDN;			// Endian Register
	volatile u32   	Reserved0; 	    // RESERVED
	volatile u32   	DBTP;  			// Data Bit Timing And Prescaler Register
	volatile u32   	TEST;			// Test Register Registers
	volatile u32   	RWD;			// RAM Watchdog Register
	volatile u32   	CCCR;			// CC Control Register
	volatile u32   	NBTP;			// Nominal Bit Timing And Prescaler Register
	volatile u32   	TSCC;			// Timestamp Counter Configuration Register
	volatile u32   	TSCV;			// Timestamp Counter Value Register
    volatile u32   	TOCC;			// Timeout Counter Configuration Register
	volatile u32   	TOCV;			// Timeout Counter Value Register
	volatile u32   	Reserved1[4];	// RESERVED
	volatile u32   	ECR;			// Error Counter Register
	volatile u32   	PSR;			// Protocol Status Register
	volatile u32   	TDCR;			// Transmitter Delay Compensation Register
	volatile u32   	Reserved2; 	    // RESERVED
	volatile u32   	IR; 	    	// Interrupt Register
	volatile u32   	IE; 	    	// Interrupt Enable Register
	volatile u32	ILS; 	    	// Interrupt Line Select Register
	volatile u32   	ILE; 	    	// Interrupt Line Enable Register
	volatile u32   	Reserved3[8];	// RESERVED
	volatile u32   	RXCFG; 	    	// Global Filter Configuration Register
	volatile u32   	XIDAM; 	    	// Extended ID and Mask Register
	volatile u32   	HPMS; 	    	// High Priority Message Status Register
	volatile u32   	RXF0S; 	    	// RX FIFO Status Register
	volatile u32   	RXF0A; 	    	// RX FIFO Acknowledge Register
	volatile u32   	RXF1S; 	    	// RX FIFO Status Register
	volatile u32   	RXF1A; 	    	// RX FIFO Acknowledge Register
	volatile u32   	Reserved4[8];	// RESERVED
	volatile u32   	TXBC; 	    	// TX Buffer Configuration Register
	volatile u32   	TXFQS; 	    	// TX FIFO Queue Status Register
	volatile u32   	TXBRP; 	    	// TX Buffer Request Pending Register
	volatile u32   	TXBAR; 	    	// TX Buffer Add Request Register
	volatile u32   	TXBCR; 	    	// TX Buffer Cancellation Request Register
	volatile u32   	TXBTO; 	    	// TX Buffer Transmission Occurred Register
	volatile u32   	TXBCF; 	    	// TX Buffer Cancellation Finished Register
	volatile u32   	TXBTIE; 	    // TX Buffer Transmission Interrupt Enable Register
	volatile u32   	TXBCIE; 	    // TX Buffer Cancellation Finished Interrupt Enable Register
	volatile u32   	TXEFS; 	    	// TX Event FIFO Status Register
	volatile u32   	TXEFA; 	    	// TX Event FIFO Acknowledge Register
	volatile u32   	CKDIV; 	    	// CFG Clock Divider Register
} FDCAN_TypeDef;

typedef struct {
	u32 FLS[RAM_FLS_ELM * RAM_FLS_LEN];
	u32 FLE[RAM_FLE_ELM * RAM_FLE_LEN];
	u32 RF0[RAM_RF0_ELM * RAM_RF0_LEN];
	u32 RF1[RAM_RF1_ELM * RAM_RF1_LEN];
	u32 TEF[RAM_TEF_ELM * RAM_TEF_LEN];
	u32 TFQ[RAM_TFQ_ELM * RAM_TFQ_LEN];
} FDCANRAM_TypeDef;

// Union For Defining Words 
typedef union {
	u32 reg;
	struct {
		u32 SFID2	:11;			// Second ID Of Standard ID Filter Element
		u32 		:5;				// Reserved
		u32 SFID1	:11;			// First ID Of Standard ID Filter Element
		u32 SFEC	:3;				// Stamdard Filter Element Configuration, 000 Disable, 001 RX FIFO 0 On Filter Match, 010 RX FIFO 1 On Filter Match, 011 Reject ID on Match, 100 Set Priority If Filter Matches, 101 Set Priority and Store FIFO0, 110 Set Priority and Store FIFO1
		u32 SFT		:2;				// Filter Type - 00 Range Filter SFID 1 - SFID 2, 01 Dual Range Filter SFID1 or SFID2, 10, Classic Filter - SFID1 = Filter SFID = Filter 2, 11, Filter Disabled
	} fields;
} FDCANSTDFilter_TypeDef;			// ELEMENT

// Union For Defining Words 
typedef union {
	u32 reg;
	struct {
		u32 EXID	:18;			// Extended ID
		u32 STID	:11;			// Standard ID
		u32 RTR		:1;				// Remote Transmission Request
		u32 XTD		:1;				// Extended ID Flag: 0 - Std, 1 - Ext
		u32 ESI		:1;				// Errpr State Indicator 0 - Node Is Error Active, 1- Node is Error Passive
	} fields;
	struct {
		u32 ID		:29;			// Combined ID
		u32			:3;				// Reserved
	} id;
} FDCANHE_TypeDef;					// ELEMENT

typedef union {
	u32 reg;
	struct {
		u32 RXTS	:16;			// RX Timestamp - Only Used In RX
		u32 DLC 	:4;				// Data Length Code: Classic CAN 0 - 8 Data Bytes, FDCAN 9 - 15 = 12/16/20/24/32/48/64 Data Bytes
		u32 BRS		:1;				// Bit Rate Switch: 0 No Bit Rate Switch, 1 - Bit Rate Switch
		u32 FD		:1;				// FD Format: 0 Standard Frame Format, 1 - FDCAN Frame Format
		u32			:2;				// Reserved
		u32 FIDX	:7;				// Filter Index - Index Of Matching RX Acceptance Filter Element
		u32	ANMF	:1;				// Accepted Non-Matching Frames - Configured at RXGFC ANFS and ANFE
	} fields;
} FDCANRX_TypeDef;

typedef union {
	u32 reg;
	struct {
		u32 		:16;			// Reserved
		u32 DLC 	:4;				// Data Length Code: Classic CAN 0 - 8 Data Bytes, FDCAN 9 - 15 = 12/16/20/24/32/48/64 Data Bytes
		u32 BRS		:1;				// Bit Rate Switch: 0 No Bit Rate Switch, 1 - Bit Rate Switch
		u32 FD		:1;				// FD Format: 0 Standard Frame Format, 1 - FDCAN Frame Format
		u32			:1;				// Reserved
		u32	EFC		:1;				// Event FIFO Control: 0 - Do Not Store TX Events, 1 Store TX Events
		u32 MM		:8;				// Message Marker - 
	} fields;
} FDCANTX_TypeDef;

/* Structure For The RX Message */
typedef struct { 
	FDCANHE_TypeDef HEADER;			// Header Data
	FDCANRX_TypeDef RX;				// RX Elements
	u8				DATA[64];		// 64 Bytes Of Data
} FDCANMsgRX_TypeDef;

/* Structure For The TX Message */
typedef struct { 
	FDCANHE_TypeDef HEADER;			// Header Data
	FDCANTX_TypeDef TX;				// TX Elements
	u8				DATA[64];		// 64 Bytes Of Data
} FDCANMsgTX_TypeDef;

/* Structure For The Register Starting Locations */
typedef struct {
	u32 			STDFilterSA; 	// Standard Filter List Start Address
  	u32 			EXTFilterSA; 	// Extended Filter List Start Address
  	u32 			RXFIFO0SA;      // Rx FIFO 0 Start Address
  	u32 			RXFIFO1SA;      // Rx FIFO 1 Start Address
  	u32 			TXEventFIFOSA;  // Tx Event FIFO Start Address
  	u32 			TXFIFOQSA;      // Tx FIFO/Queue Start Address
} FDCANRamAddress_TypeDef;

/* Enumerations */
// Baud Rates
typedef enum fdcan_baud {
    Baud125kB,
    Baud250kB,
    Baud500kB,
    Baud1MB
} FDCAN_Baud;

// Word Length - Bits 12 -64 will drive 8 bits in CAN 2.0 automatically
typedef enum fdcan_wordlength {
    Bits0,
    Bits1,
    Bits2,
    Bits3,
	Bits4,
	Bits5,
	Bits6,
	Bits7,
	Bits8,
	Bits12,
	Bits16,
	Bits20,
	Bits24,
	Bits32,
	Bits48,
	Bits64
} FDCAN_WordLength;

/* Public Functions */
void fdcan_open(FDCAN_TypeDef *ptr, u32 ram_base);
bool fdcan_read(FDCAN_TypeDef *ptr, FDCANRAM_TypeDef *ram, FDCANMsgRX_TypeDef *msg);
bool fdcan_write(FDCAN_TypeDef *ptr, FDCANRAM_TypeDef *ram, FDCANMsgTX_TypeDef *msg);
void fdcan_clr_ir(FDCAN_TypeDef *ptr, u32 bit);
u32 fdcan_get_ir(FDCAN_TypeDef *ptr);
void fdcan_set_fls(FDCANRAM_TypeDef *ptr, u32 ind, FDCANSTDFilter_TypeDef std);

#endif /* FDCAN_H_ */