/* Main Entry Of Program */
#include "main.h"

#define RCC         ((RCC_TypeDef *)RCC_BASE)
#define GPIOA       ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB       ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC       ((GPIO_TypeDef *)GPIOC_BASE)

/* Extern Keyword Allows To Be Call */
extern void SystemInit() {
    rcc_write_ahb2_enr(RCC, RCC_GPIOA_AHB2EN);          // ENABLE GPIOA
    rcc_write_ahb2_enr(RCC, RCC_GPIOB_AHB2EN);          // ENABLE GPIOB
    rcc_write_ahb2_enr(RCC, RCC_GPIOC_AHB2EN);          // ENABLE GPIOC
    rcc_set_pll(RCC, Rcc_MSI, 48, 0);                   // SET MULTIPLIER AND DIVIDER FOR PLL
    rcc_set_pllclk(RCC, Rcc_PLL8);                      // SET DIVIDER FOR PLLCLK
    rcc_start_pll(RCC);                                 // START PLL
    rcc_set_pllclk_en(RCC);                             // SET PLLCLK OUTPUT
    rcc_write_ccipr1(RCC, RCC_FDCAN_CCIPR1_PLLQCLK);    // SELECT CCIPR1 FDCAN PLLQCLK
    rcc_write_apb1_enr2(RCC, RCC_FDCAN_APB1_ENR2);      // ENABLE RCC FDCAN CLOCK
    rcc_sys_clk(RCC, 3);
}

extern void main() {
    gpio_type(GPIOA, LED_RED_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    gpio_type(GPIOB, LED_BLU_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    gpio_type(GPIOC, LED_GRN_PIN, Gpio_Output, Gpio_Push_Pull, AF0);

    int i = 0;
    volatile int lim = 1000000;
    while (1) {
        for (i = 0; i < lim * 24; i++) {
            if (i == lim * 6) {
                gpio_set_pin(GPIOC, LED_GRN);
            } else if (i == lim * 12) {
                gpio_set_pin(GPIOB, LED_BLU);
            } else if (i == lim * 18) {
                gpio_set_pin(GPIOA, LED_RED);
            } else if (i == 0) {
                gpio_clr_pin(GPIOC, LED_GRN);
                gpio_clr_pin(GPIOB, LED_BLU);
                gpio_clr_pin(GPIOA, LED_RED);
            }
        }
    }
}

extern void __aeabi_unwind_cpp_pr0() {
    //loop {}
}
