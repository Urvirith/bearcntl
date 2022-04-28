#ifndef USB_H_
#define USB_H_

#include "common.h"
#include "pointer.h"

// USB DRIVER
// ARM USB MODULE
// USB Description - is on pg 1948
// USB Registers (Programming Manual) - is on 1963

/* Universal Serial Bus (USB) */
typedef struct {
	volatile u32   EPR[8];			// Endpoint Specific Registers
	volatile u32   Reserved0[8];	// RESERVED
	volatile u32   CNTR;			// Control Register
	volatile u32   ISTR;			// Interrupt Status Register
	volatile u32   FNR;				// Frame Number Register
	volatile u32   DADDR; 	    	// Device Address Register
	volatile u32   BTABLE; 	    	// Buffer Table Address Register
	volatile u32   LPMCSR; 	    	// LPM Control And Status Register
	volatile u32   BCDR; 	    	// Battery Charging Register
} USB_TypeDef;

typedef struct {
	volatile u8    ADDR_TX;			// Transmission Buffer Address Register
	volatile u16   COUNT_TX;		// Transmission Byte Count Register
	volatile u8    ADDR_RX;			// Reception Buffer Address Register
	volatile u16   COUNT_RX;		// Reception Byte Count Register
} USBData_TypeDef;

/* Public Functions */


#endif /* USB_H_ */