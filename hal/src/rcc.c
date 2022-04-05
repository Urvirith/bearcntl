/* Reset and Clock Control (RCC) */
#include "rcc.h"

/* Register Masks */
/* CR */
#define MSIRANGE_MASK   MASK_4_BIT

/* CFGR */
#define SW_MASK         MASK_2_BIT

/* PLLxxx */
#define PLLSRC_MASK     MASK_2_BIT
#define PLLM_MASK       MASK_4_BIT
#define PLLN_MASK       MASK_7_BIT
#define PLLQ_MASK       MASK_2_BIT
#define PLLR_MASK       MASK_2_BIT

/* CCIP */
#define CCIP_MASK       MASK_2_BIT

/* Register Bits */
/* CR */
#define MSION           BIT_0       
#define MSIRGSEL        BIT_3
#define PLLON           BIT_24
#define PLLRDY          BIT_25

/* PLLxxx */
#define PLLPEN          BIT_17
#define PLLP            BIT_18
#define PLLQEN          BIT_20
#define PLLREN          BIT_24


/* Register Offsets */
/* CR */    
#define MSIRANGE_OFFSET 4 

/* CFGR */
#define SW_OFFSET       0
#define SWS_OFFSET      2

/* PLLxxx */
#define PLLSRC_OFFSET   0
#define PLLM_OFFSET     4
#define PLLN_OFFSET     8
#define PLLQ_OFFSET     21
#define PLLR_OFFSET     25


/* Set the clock speed of the chipset */
void rcc_write_msi_range(RCC_TypeDef *ptr, MSI_Range freq_khz) {
    clr_ptr_vol_bit_u32(&ptr->CR, MSION);
    set_ptr_vol_u32(&ptr->CR, MSIRANGE_OFFSET, MSIRANGE_MASK, freq_khz);
    set_ptr_vol_bit_u32(&ptr->CR, MSIRGSEL);
    set_ptr_vol_bit_u32(&ptr->CR, MSION);
}

/* Set system clock source */
void rcc_sys_clk(RCC_TypeDef *ptr, u32 src) {
    set_ptr_vol_u32(&ptr->CFGR, SW_OFFSET, SW_MASK, src);
    //while (get_ptr_vol_u32(&ptr->CFGR, SWS_OFFSET, SW_MASK) != src) {
        // SPIN WHILE CLOCK IS NOT SET TO SYSTEM CLOCK SOURCE REQUEST
    //};
}

/* Set The PLL */
/*
    freq(VCO CLOCK) = freq(PLL Clock Input) * (PLLN / PLLM)
    freq(PLL_P) = f(VCO Clock) / PLLP
    freq(PLL_Q) = f(VCO Clock) / PLLQ
    freq(PLL_R) = f(VCO Clock) / PLLR
*/
/* Set The VCO Clock freq(VCO CLOCK) = freq(PLL Clock Input) * (PLLN / PLLM) */ 
void rcc_set_pll(RCC_TypeDef *ptr, RCC_PLLSrc src, u32 pllm, u32 plln) {
    clr_ptr_vol_bit_u32(&ptr->CR, PLLON);
    while(get_ptr_vol_bit_u32(&ptr->CR, PLLRDY)){
        // Spin Dead Until PLL Not Ready
    };
    set_ptr_vol_u32(&ptr->PLL_CFGR, PLLSRC_OFFSET, PLLSRC_MASK, src);   // Set Clock Source
    set_ptr_vol_u32(&ptr->PLL_CFGR, PLLM_OFFSET, PLLM_MASK, pllm);      // Set Division Factor 1<=PLLM<=8
    set_ptr_vol_u32(&ptr->PLL_CFGR, PLLN_OFFSET, PLLN_MASK, plln);      // Set Multiply Factor 8<=PLLN<=86
}

/* Start The PLL Clock */
void rcc_start_pll(RCC_TypeDef *ptr) {
    set_ptr_vol_bit_u32(&ptr->CR, PLLON);
    while(!get_ptr_vol_bit_u32(&ptr->CR, PLLRDY)){
        // Spin Dead Until PLL Ready
    };
}

/* Set The PLLQ Clock */
void rcc_set_pll48clk(RCC_TypeDef *ptr, RCC_PLLDiv div) {
    set_ptr_vol_u32(&ptr->PLL_CFGR, PLLQ_OFFSET, PLLQ_MASK, div);
}

/* Set The PLLQ Clock En */
void rcc_set_pll48clk_en(RCC_TypeDef *ptr) {
    set_ptr_vol_bit_u32(&ptr->AHB1_ENR, PLLQEN);
}

/* Set The PLLR Clock */
void rcc_set_pllclk(RCC_TypeDef *ptr, RCC_PLLDiv div) {
    set_ptr_vol_u32(&ptr->PLL_CFGR, PLLR_OFFSET, PLLR_MASK, div);
}

/* Set The PLLR Clock En */
void rcc_set_pllclk_en(RCC_TypeDef *ptr) {
    set_ptr_vol_bit_u32(&ptr->AHB1_ENR, PLLREN);
}

void rcc_write_ahb1_enr(RCC_TypeDef *ptr, u32 val) {
    set_ptr_vol_bit_u32(&ptr->AHB1_ENR, val);
}

void rcc_write_ahb2_enr(RCC_TypeDef *ptr, u32 val) {
    set_ptr_vol_bit_u32(&ptr->AHB2_ENR, val);
}

void rcc_write_ahb3_enr(RCC_TypeDef *ptr, u32 val) {
    set_ptr_vol_bit_u32(&ptr->AHB3_ENR, val);
}

void rcc_write_apb1_enr1(RCC_TypeDef *ptr, u32 val) {
    set_ptr_vol_bit_u32(&ptr->APB1_ENR1, val);
}

void rcc_write_apb1_enr2(RCC_TypeDef *ptr, u32 val) {
    set_ptr_vol_bit_u32(&ptr->APB1_ENR2, val);
}

void rcc_write_apb2_enr(RCC_TypeDef *ptr, u32 val) {
    set_ptr_vol_bit_u32(&ptr->APB2_ENR, val);
}

void rcc_write_ccipr1(RCC_TypeDef *ptr, u32 val) {
    set_ptr_vol_bit_u32(&ptr->CCIPR1, val); 
}
