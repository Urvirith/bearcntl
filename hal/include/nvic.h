#ifndef NVIC_H_
#define NVIC_H_

#include "common.h"
#include "pointer.h"

// NVIC DRIVER
// ARM NVIC MODULE
// NVIC Description - is on pg 529
// NVIC Registers (Programming Manual) - is on 178

/* Nested vectored interrupt controller (NVIC) */
typedef struct {
	volatile u32   ISER[16];		// Interrupt Set-Enable register
	volatile u32   Reserved0[16];	// Offset
	volatile u32   ICER[16]; 	    // Interrupt Clear-Enable register
	volatile u32   Reserved1[16];  	// Offset
	volatile u32   ISPR[16];		// Interrupt Set-Pending Registers
	volatile u32   Reserved2[16];	// Offset
	volatile u32   ICPR[16];		// Interrupt Clear-Pending Registers
	volatile u32   Reserved3[16];	// Offset
	volatile u32   IABR[16];		// Interrupt Active Bit Registers
	volatile u32   Reserved4[16];	// Offset
    volatile u32   ITNS[16];		// Interrupt Target Non-Secure Registers
	volatile u32   Reserved5[16];	// Offset
	volatile u8    IPR[480];		// Interrupt Priority Registers
} NVIC_TypeDef;

/* Public Functions */
void nvic_enable_interrupt(NVIC_TypeDef *ptr, u32 irq_num);
void nvic_set_priority(NVIC_TypeDef *ptr, u32 irq_num, u8 priority, u8 sub_priority);

#endif /* NVIC_H_ */
