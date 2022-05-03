#ifndef CANOPEN_H_
#define CANOPEN_H_

#include "common.h"

/*
    CANOpen
    Standard Industrial Protocol
    Uses Include - Robotics, Remote I/O, Safety I/O
*/

#define CO_NODE_MASK        (u32)0x007F     // Standard ID, Node Mask
#define CO_FUNC_MASK        (u32)0x0780     // Standard ID, Function Code Mask
#define CO_MAX_CNT          MASK_12_BIT     // 12 Bit Mask As Max Count

typedef union {
	u32 reg;
	struct {
		u32 NODE	:11;			        // Node Address 1 - 127 - Set to 11 Bit To Accomidate Func Code and Node
		u32 STATE 	:8;				        // State - Max of 0xFF
		u32 HB		:1;		                // Heartbeat, Used for toggling of NMT If Internal State Stays the Same Increase 
		u32 CNT		:12;			        // FD Format: 0 Standard Frame Format, 1 - FDCAN Frame Format
	} fields;
} CANOpen_TypeDef;

/* Enumerations */
// Baud Rates
typedef enum canopen_state {
    CO_Bootup           = (u32)0x00,        // Boot Up (Initialising) State
    CO_Stopped          = (u32)0x04,        // Stopped State
    CO_Operational      = (u32)0x05,        // Operational State
    CO_Preoperational   = (u32)0x7F,        // Pre-Operational State
    CO_Unknown          = (u32)0xFF,        // Unknown State
} CANOpen_State;


#endif /* CANOPEN_H_ */