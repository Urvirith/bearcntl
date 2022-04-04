#ifndef USART_H_
#define USART_H_

#include "common.h"
#include "pointer.h"

// USART DRIVER
// ARM USART MODULE
// USART Description - is on pg 1559

/* Universal Synchronous/Asynchronous Receiver Transmitter */
typedef struct {
    volatile u32 CR1;               // Control Register 1
    volatile u32 CR2;               // Control Register 2
    volatile u32 CR3;               // Control Register 3
    volatile u32 BRR;               // Baud Rate Register Register
    volatile u32 GTPR;              // Guard Time / Prescaler Register
    volatile u32 RTOR;              // Receiver Timeout Register
    volatile u32 RQR;               // Request Register
    volatile u32 ISR;               // Interrupt And Status Register
    volatile u32 ICR;               // Interrupt Flag Clear Register
    volatile u32 RDR;               // Receive Data Register
    volatile u32 TDR;               // Transmit Data Register
} USART_TypeDef;

#define SCLK_HZ             1000
#define RTO_TIMEO_VALUE     10

/* Enumerations */

/* Enumeration For Oversample */
typedef enum usart_over_sample {
    USART_Oversample_16,
    USART_Oversample_8
} USART_OverSample;

/* Enumeration For Word Length */
typedef enum usart_word_length {
    USART_8_Bits,
    USART_9_Bits,
    USART_7_Bits
} USART_WordLength;

/* Enumeration For Stop Length */
typedef enum usart_stop_length {
    USART_1_StopBit,
    USART_0_5_StopBit,
    USART_2_StopBit,
    USART_1_5_StopBit
} USART_StopLength;

/* Enumeration For Baud Rate */
typedef enum usart_baud_rate {
    USART_1200_BAUD      = 1200, 
    USART_1800_BAUD      = 1800,
    USART_2400_BAUD      = 2400, 
    USART_4800_BAUD      = 4800,
    USART_9600_BAUD      = 9600, 
    USART_19200_BAUD     = 19200,
    USART_28800_BAUD     = 28800, 
    USART_38400_BAUD     = 38400,
    USART_57600_BAUD     = 57600, 
    USART_76800_BAUD     = 76800,
    USART_115200_BAUD    = 115200, 
    USART_230400_BAUD    = 230400,
    USART_460800_BAUD    = 460800, 
    USART_576000_BAUD    = 576000,
    USART_921600_BAUD    = 921600, 
} USART_BaudRate;

/* Public Functions */

/* Open USART Driver(USART Pointer, Word Length, Stop Length, Baud Rate, Clock Rate KHz, Over Sample - 8 or 16) */
void usart_open(USART_TypeDef *ptr, USART_WordLength word_len, USART_StopLength stop, USART_BaudRate baud, u32 sclk_khz, USART_OverSample over8);
bool usart_get_read(USART_TypeDef *ptr);
int usart_read(USART_TypeDef *ptr, u8* buf, int len);
void usart_write(USART_TypeDef *ptr, u8* buf, int len);

#endif /* USART_H_ */
