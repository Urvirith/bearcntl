#ifndef POINTER_H_
#define POINTER_H_

#include "common.h"

/* Public Functions */
/* 32-Bit Functions */
bool get_ptr_vol_bit_u32(volatile u32 *ptr, u32 value);                             /* Get a pointer bit in a u32 ptr Example value (10000 >> 5) -> 1 */
void set_ptr_vol_bit_u32(volatile u32 *ptr, u32 value);                             /* Set a pointer bit in a u32 ptr Example value (1 << 5) */
void clr_ptr_vol_bit_u32(volatile u32 *ptr, uint32_t value);                        /* Clear a pointer bit in a u32 ptr Example value (1 << 5) */
u32 get_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask);                       /* Get a pointer bit in a u32 ptr Example value (10000 >> 5) -> 1 */
void set_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask, uint32_t value);      /* Write to a pointer with offset and mask size, ensure mask >= value */
u8 get_ptr_vol_raw_u8(volatile u8 *ptr);                                            /* Get a pointer value */
void set_ptr_vol_raw_u8(volatile u8 *ptr, u8 value);                                /* Write to a pointer with raw value */
u32 get_ptr_vol_raw_u32(volatile u32 *ptr);                                         /* Get a pointer value */
void set_ptr_vol_raw_u32(volatile u32 *ptr, u32 value);                             /* Write to a pointer with raw value */

#endif /* POINTER_H_ */