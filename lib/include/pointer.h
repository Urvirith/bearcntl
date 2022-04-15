#ifndef POINTER_H_
#define POINTER_H_

#include "common.h"

/* Public Functions */
bool get_ptr_vol_bit_u32(volatile u32 *ptr, u32 val);
void set_ptr_vol_bit_u32(volatile u32 *ptr, u32 val);
void sr_ptr_vol_bit_u32(volatile u32 *ptr, u32 val);
void clr_ptr_vol_bit_u32(volatile u32 *ptr, u32 val);
u32 get_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask);
void set_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask, u32 val);
u8 get_ptr_vol_raw_u8(volatile u8 *ptr);
void set_ptr_vol_raw_u8(volatile u8 *ptr, u8 val);
u32 get_ptr_vol_raw_u32(volatile u32 *ptr);
void set_ptr_vol_raw_u32(volatile u32 *ptr, u32 val);

#endif /* POINTER_H_ */ 