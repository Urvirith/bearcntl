#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;

/* Standard Bit Shifts for 32-bit words */
#define BIT_0       ((u32)1 << 0)
#define BIT_1       ((u32)1 << 1)
#define BIT_2       ((u32)1 << 2)
#define BIT_3       ((u32)1 << 3)
#define BIT_4       ((u32)1 << 4)
#define BIT_5       ((u32)1 << 5)
#define BIT_6       ((u32)1 << 6)
#define BIT_7       ((u32)1 << 7)
#define BIT_8       ((u32)1 << 8)
#define BIT_9       ((u32)1 << 9)
#define BIT_10      ((u32)1 << 10)
#define BIT_11      ((u32)1 << 11)
#define BIT_12      ((u32)1 << 12)
#define BIT_13      ((u32)1 << 13)
#define BIT_14      ((u32)1 << 14)
#define BIT_15      ((u32)1 << 15)
#define BIT_16      ((u32)1 << 16)
#define BIT_17      ((u32)1 << 17)
#define BIT_18      ((u32)1 << 18)
#define BIT_19      ((u32)1 << 19)
#define BIT_20      ((u32)1 << 20)
#define BIT_21      ((u32)1 << 21)
#define BIT_22      ((u32)1 << 22)
#define BIT_23      ((u32)1 << 23)
#define BIT_24      ((u32)1 << 24)
#define BIT_25      ((u32)1 << 25)
#define BIT_26      ((u32)1 << 26)
#define BIT_27      ((u32)1 << 27)
#define BIT_28      ((u32)1 << 28)
#define BIT_29      ((u32)1 << 29)
#define BIT_30      ((u32)1 << 30)
#define BIT_31      ((u32)1 << 31)

/* Standard Bit Masks for 32-bit words */
#define MASK_1_BIT  (u32)0x00000001
#define MASK_2_BIT  (u32)0x00000003
#define MASK_3_BIT  (u32)0x00000007
#define MASK_4_BIT  (u32)0x0000000F
#define MASK_5_BIT  (u32)0x0000001F
#define MASK_6_BIT  (u32)0x0000003F
#define MASK_7_BIT  (u32)0x0000007F
#define MASK_8_BIT  (u32)0x000000FF
#define MASK_9_BIT  (u32)0x000001FF
#define MASK_10_BIT (u32)0x000003FF
#define MASK_11_BIT (u32)0x000007FF
#define MASK_12_BIT (u32)0x00000FFF
#define MASK_13_BIT (u32)0x00001FFF
#define MASK_14_BIT (u32)0x00003FFF
#define MASK_15_BIT (u32)0x00007FFF
#define MASK_16_BIT (u32)0x0000FFFF
#define MASK_17_BIT (u32)0x0001FFFF
#define MASK_18_BIT (u32)0x0003FFFF
#define MASK_19_BIT (u32)0x0007FFFF
#define MASK_20_BIT (u32)0x000FFFFF
#define MASK_21_BIT (u32)0x001FFFFF
#define MASK_22_BIT (u32)0x003FFFFF
#define MASK_23_BIT (u32)0x007FFFFF
#define MASK_24_BIT (u32)0x00FFFFFF
#define MASK_25_BIT (u32)0x01FFFFFF
#define MASK_26_BIT (u32)0x03FFFFFF
#define MASK_27_BIT (u32)0x07FFFFFF
#define MASK_28_BIT (u32)0x0FFFFFFF
#define MASK_29_BIT (u32)0x1FFFFFFF
#define MASK_30_BIT (u32)0x3FFFFFFF
#define MASK_31_BIT (u32)0x7FFFFFFF
#define MASK_32_BIT (u32)0xFFFFFFFF

// Enumeration of avaliable ranges of clocks
typedef enum msi_range {
    Clk100kHz, 
    Clk200kHz, 
    Clk400kHz, 
    Clk800kHz, 
    Clk1MHz, 
    Clk2MHz, 
    Clk4MHz, 
    Clk8MHz, 
    Clk16MHz, 
    Clk24MHz, 
    Clk32MHz, 
    Clk48MHz
} MSI_Range;

/* DELAY TIME FOR 50us, 1MHz = 1 Cycle Per Microsecond, Therefore 16 Mhz * 50 cycles = 800 Cycles = 50us */
#define TIMEOUT (u32)(16 * 50)

/* Public Functions */
/* 32-Bit Functions */
bool get_ptr_vol_bit_u32(volatile u32 *ptr, u32 value);                               /* Get a pointer bit in a u32 ptr Example value (10000 >> 5) -> 1 */
void set_ptr_vol_bit_u32(volatile u32 *ptr, u32 value);                               /* Set a pointer bit in a u32 ptr Example value (1 << 5) */
void clr_ptr_vol_bit_u32(volatile u32 *ptr, uint32_t value);                               /* Clear a pointer bit in a u32 ptr Example value (1 << 5) */
u32 get_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask);               /* Get a pointer bit in a u32 ptr Example value (10000 >> 5) -> 1 */
void set_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask, uint32_t value);   /* Write to a pointer with offset and mask size, ensure mask >= value */
u8 get_ptr_vol_raw_u8(volatile u8 *ptr);                                              /* Get a pointer value */
void set_ptr_vol_raw_u8(volatile u8 *ptr, u8 value);                                  /* Write to a pointer with raw value */
u32 get_ptr_vol_raw_u32(volatile u32 *ptr);                                           /* Get a pointer value */
void set_ptr_vol_raw_u32(volatile u32 *ptr, u32 value);                               /* Write to a pointer with raw value */

#endif /* COMMON_H_ */