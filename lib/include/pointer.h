#ifndef POINTER_H_
#define POINTER_H_

#include "common.h"

/* Inline Function Definitions */
inline bool get_ptr_vol_bit_u32(volatile u32 *ptr, u32 val) {
    if ((*ptr & val) > 0) {
        return true;
    } else {
        return false;
    }
}

/* Set a pointer bit in a u32 ptr Example value (1 << 5), 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline void set_ptr_vol_bit_u32(volatile u32 *ptr, u32 val){
    *ptr |= val; 
}

/* Set Reset a pointer bit in a u32 ptr Example value (1 << 5), 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline void sr_ptr_vol_bit_u32(volatile u32 *ptr, u32 val){
    *ptr ^= val; 
}

/* Clear a pointer bit in a u32 ptr Example value (1 << 5) 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline void clr_ptr_vol_bit_u32(volatile u32 *ptr, u32 val){
    *ptr &= (~val); 
}

/* Get a pointer bit in a u32 ptr Example value (10000 >> 5) -> 1 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline u32 get_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask){
    return((*ptr >> offset) & mask); 
}

/* Write to a pointer with offset and mask size, ensure mask >= value 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline void set_ptr_vol_u32(volatile u32 *ptr, u32 offset, u32 mask, u32 val){
    *ptr &= (~(mask << offset));
    *ptr |= (val << offset);
}

/* Get a pointer value 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline u8 get_ptr_vol_raw_u8(volatile u8 *ptr){
    return *ptr; 
}

/* Write to a pointer with raw value, ensure mask >= value 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline void set_ptr_vol_raw_u8(volatile u8 *ptr, u8 val){
    *ptr = val;
}

/* Get a pointer value 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline u32 get_ptr_vol_raw_u32(volatile u32 *ptr){
    return *ptr; 
}

/* Write to a pointer with raw value, ensure mask >= value 
   If passing a stuct pointer ie. GPIOB->ODR, you can pass by reference &GPIO->ODR */
inline void set_ptr_vol_raw_u32(volatile u32 *ptr, u32 val){
    *ptr = val;
}

#endif /* POINTER_H_ */ 