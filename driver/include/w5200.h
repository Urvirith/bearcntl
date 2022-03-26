#ifndef W5200_H_
#define W5200_H_

#include "hal/include/common.h"

/* 
    WIZNet W5200 Chipset
    http://wiznethome.cafe24.com/wp-content/uploads/wiznethome/Chip/W5200/Documents/W5200_DS_V140E.pdf
    http://wiznethome.cafe24.com/wp-content/uploads/wiznethome/Network%20Module/WIZ820io/Document/WIZ820io_User_Manual_V1.0.pdf
*/

typedef struct {
    volatile u32 CR1;               // Control Register 1
    volatile u32 CR2;               // Control Register 2
    volatile u32 SR;                // Baud Rate Register Register
    volatile u32 DR;                // Guard Time / Prescaler Register
    volatile u32 CRCPR;             // Receiver Timeout Register
    volatile u32 RXCRCPR;           // Request Register
    volatile u32 TXCRCPR;           // Interrupt And Status Register
} W5200_TypeDef;

#endif /* W5200_H_ */
