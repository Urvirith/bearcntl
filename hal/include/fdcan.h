#ifndef FDCAN_H_
#define FDCAN_H_

#include "common.h"
#include "pointer.h"
#include "l552ze.h"

// FDCAN DRIVER
// ARM FDCAN MODULE
// FDCAN Description - is on pg 1883
// FDCAN Registers (Programming Manual) - is on 1913

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

// Union For Defining Words 
typedef union {
	u32 reg;
	struct {
		u32 EXID	:18;			// Extended ID
		u32 STID	:11;			// Standard ID
		u32 RTR		:1;				// Remote Transmission Request
		u32 XTD		:1;				// Extended ID Flag: 0 - Std, 1 - Ext
		u32 ESI		:1;				// CAN FD Format
	} fields;
	struct {
		u32 ID		:29;			// Combined ID
		u32			:3;				// Reserved
	} id;
} FDCANE1_TypeDef;					// ELEMENT

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
	FDCANE1_TypeDef HEADER;			// Header Data
	FDCANRX_TypeDef RX;				// RX Elements
	u8				DATA[64];		// 64 Bytes Of Data
} FDCANMsgRX_TypeDef;

/* Structure For The TX Message */
typedef struct { 
	FDCANE1_TypeDef HEADER;			// Header Data
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


#endif /* FDCAN_H_ */