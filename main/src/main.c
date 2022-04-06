/* Main Entry Of Program */
#include "main.h"

#define RCC         ((RCC_TypeDef *)RCC_BASE)
#define GPIOA       ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB       ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC       ((GPIO_TypeDef *)GPIOC_BASE)
#define TIMER2      ((TIMER_TypeDef *)TIMER2_BASE)

/* Extern Keyword Allows To Be Call */
extern void SystemInit() {
    rcc_write_ahb2_enr(RCC, RCC_GPIOA_AHB2EN);          // ENABLE GPIOA
    rcc_write_ahb2_enr(RCC, RCC_GPIOB_AHB2EN);          // ENABLE GPIOB
    rcc_write_ahb2_enr(RCC, RCC_GPIOC_AHB2EN);          // ENABLE GPIOC
    rcc_set_pll(RCC, Rcc_MSI, 48, 0);                   // SET MULTIPLIER AND DIVIDER FOR PLL
    rcc_set_pllclk(RCC, Rcc_PLL6);                      // SET DIVIDER FOR PLLCLK
    rcc_start_pll(RCC);                                 // START PLL
    rcc_set_pllclk_en(RCC);                             // SET PLLCLK OUTPUT
    rcc_write_ccipr1(RCC, RCC_FDCAN_CCIPR1_PLLQCLK);    // SELECT CCIPR1 FDCAN PLLQCLK
    rcc_write_apb1_enr2(RCC, RCC_FDCAN_APB1_ENR2);      // ENABLE RCC FDCAN CLOCK
    rcc_write_apb1_enr1(RCC, RCC_TIMER2_APB1R1EN);      // ENABLE TIMER 2
    rcc_sys_clk(RCC, 3);
}

extern void main() {
    gpio_type(GPIOA, LED_RED_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    gpio_type(GPIOB, LED_BLU_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    gpio_type(GPIOC, LED_GRN_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    timer_open(TIMER2, Timer_Cont, Timer_Upcount);
    timer_set_time(TIMER2, 1000, 32000, 10000);
    timer_start(TIMER2);

    u32 i = 0;
    while (1) {
        if (timer_get_flag(TIMER2)) {
            if (i == 1) {
                gpio_set_pin(GPIOC, LED_GRN);
            } else if (i == 2) {
                gpio_set_pin(GPIOB, LED_BLU);
            } else if (i == 3) {
                //gpio_set_pin(GPIOA, LED_RED);
            } else if (i == 0) {
                gpio_clr_pin(GPIOC, LED_GRN);
                gpio_clr_pin(GPIOB, LED_BLU);
                //gpio_clr_pin(GPIOA, LED_RED);
            }

            sr_ptr_vol_bit_u32(&GPIOA->ODR, LED_RED);

            i+= 1;
            if (i > 3) {
                i = 0;
            }

            timer_clr_flag(TIMER2);
        }
    }
}

extern void __aeabi_unwind_cpp_pr0() {
    //loop {}
}
