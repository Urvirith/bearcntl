/* Nested vectored interrupt controller (NVIC) */
#include "nvic.h"

#define NVIC_32BIT      (u32)32
#define NVIC_8BIT       (u32)8
#define NVIC_ARRAYSIZE  (u32)16
#define NVIC_IPR_SIZE   (u32)480

/* Private Functions */
static u32 nvic_get_reg(u32 irq_num);
static u32 nvic_get_bit(u32 irq_num);
static u8 nvic_get_priority(u8 priority, u8 sub_priority);

/* Enable NVIC Interrupt */
void nvic_enable_interrupt(NVIC_TypeDef *ptr, u32 irq_num) {
    if (nvic_get_reg(irq_num) < NVIC_ARRAYSIZE) {
        set_ptr_vol_bit_u32(&ptr->ISER[nvic_get_reg(irq_num)], nvic_get_bit(irq_num));
    }
}

/* Set NVIC Interrupt Priority */
void nvic_set_priority(NVIC_TypeDef *ptr, u32 irq_num, u8 priority, u8 sub_priority) {
    if (irq_num < NVIC_IPR_SIZE) {
        set_ptr_vol_raw_u8(&ptr->IPR[irq_num], nvic_get_priority(priority, sub_priority));
    }
}

/* Private Function To Get The Bit By (x % 32) ex. 47 % 32 = 15 -> 1 << 15 */
static u32 nvic_get_reg(u32 irq_num) {
    return (irq_num / NVIC_32BIT);
}

/* Private Function To Get The Bit By (x % 32) ex. 47 % 32 = 15 -> 1 << 15 */
static u32 nvic_get_bit(u32 irq_num) {
    return (1 << (irq_num % NVIC_32BIT));
}

/* Private Function To Get The Priority */
static u8 nvic_get_priority(u8 priority, u8 sub_priority) {
    return (((priority) << (6)) | (((sub_priority) & (0x3)) << (4)));
}
