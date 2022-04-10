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

/* Structure For The RX Message */
typedef struct { 
	u32				ID;				// Standard or Extended Identifier
	u32				FIDX;			// Filter Index - 0 Frame Matching Filter Index FIDX, 1 Frame Did Not Match RX Filter Element
	u32				DLC;			// Data Length Code
	u32				RXTS;			// Timestamp Counter Value, Resolution Based On TSCC, TCP
	bool 			ESI;			// Error State - 0: Depends On Error Passive Flag, 1: Transmitted Recessive
	bool			XTD;			// Extended Identifier - 0: Standard - 11 Bit, 1: Extended - 29 Bit
	bool			RTR;			// Remote Transmission Request 0: Data Frame, 1: Remote Frame
	bool			ANMF;			// Accepted Non Matching Frame - 0: Received Fram Matching FIDX, Received Frame not matching any RX Filter Element 
	bool			FDF;			// FD Format - 0: Classical CAN Format, 1: Frame Transmitted In CAN FD Format
	bool			BRS;			// Bit Rate Switching - 0: CANFD Transmitted Without Bit Switching, 1: CANFD Transmitted With Bit Switching
	u8				DATA[64];		// 64 Bytes Of Data
} FDCANMsgRX_TypeDef;

/* Structure For The TX Message */
typedef struct { 
	u32				ID;				// Standard or Extended Identifier
	u32				MM;				// Message Marker
	u32				DLC;			// Data Length Code
	bool 			ESI;			// Error State - 0: Depends On Error Passive Flag, 1: Transmitted Recessive
	bool			XTD;			// Extended Identifier - 0: Standard - 11 Bit, 1: Extended - 29 Bit
	bool			RTR;			// Remote Transmission Request 0: Data Frame, 1: Remote Frame
	bool			EFC;			// Event FIFO Control - 0: Do Not Store TX Events, 1: Store TX Events
	bool			FDF;			// FD Format - 0: Classical CAN Format, 1: Frame Transmitted In CAN FD Format
	bool			BRS;			// Bit Rate Switching - 0: CANFD Transmitted Without Bit Switching, 1: CANFD Transmitted With Bit Switching
	u8				DATA[64];		// 64 Bytes Of Data
} FDCANMsgTX_TypeDef;

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