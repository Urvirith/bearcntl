/* Main Entry Of Program */
#include "main.h"

#define RCC         ((RCC_TypeDef *)RCC_BASE)
#define GPIOA       ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB       ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC       ((GPIO_TypeDef *)GPIOC_BASE)

/* Extern Keyword Allows To Be Call */
extern void SystemInit() {
    rcc_write_ahb2_enr(RCC, RCC_GPIOA_AHB2EN);
    rcc_write_ahb2_enr(RCC, RCC_GPIOB_AHB2EN);
    rcc_write_ahb2_enr(RCC, RCC_GPIOC_AHB2EN);
    rcc_set_pll(RCC, Rcc_MSI, 0, 24);
    rcc_set_pllclk(RCC, Rcc_PLL2);
    rcc_start_pll(RCC);
    rcc_set_pllclk_en(RCC);
    rcc_sys_clk(RCC, 3);
}

extern void main() {
    set_ptr_vol_u32(&GPIOA->MODER, (LED_RED_PIN * 2), MASK_2_BIT, 1);
    set_ptr_vol_u32(&GPIOB->MODER, (LED_BLU_PIN * 2), MASK_2_BIT, 1);
    set_ptr_vol_u32(&GPIOC->MODER, (LED_GRN_PIN * 2), MASK_2_BIT, 1);
    clr_ptr_vol_bit_u32(&GPIOA->OTYPER, LED_RED);
    clr_ptr_vol_bit_u32(&GPIOB->OTYPER, LED_BLU);
    clr_ptr_vol_bit_u32(&GPIOC->OTYPER, LED_GRN);

    int i = 0;
    while (1) {
        for (i = 0; i < 1200000; i++) {
            if (i == 300000) {
                set_ptr_vol_bit_u32(&GPIOC->ODR, LED_GRN);
            } else if (i == 600000) {
                set_ptr_vol_bit_u32(&GPIOB->ODR, LED_BLU);
            } else if (i == 900000) {
                set_ptr_vol_bit_u32(&GPIOA->ODR, LED_RED);
            } else if (i == 0) {
                clr_ptr_vol_bit_u32(&GPIOC->ODR, LED_GRN);
                clr_ptr_vol_bit_u32(&GPIOB->ODR, LED_BLU);
                clr_ptr_vol_bit_u32(&GPIOA->ODR, LED_RED);
            }
        }
    }
}

extern void __aeabi_unwind_cpp_pr0() {
    //loop {}
}
