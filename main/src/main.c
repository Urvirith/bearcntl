/* Main Entry Of Program */
#include "main.h"

#define RCC         ((RCC_TypeDef *)RCC_BASE)
#define GPIOA       ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB       ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC       ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD       ((GPIO_TypeDef *)GPIOD_BASE)
#define TIMER2      ((TIMER_TypeDef *)TIMER2_BASE)
#define FDCAN       ((FDCAN_TypeDef *)FDCAN_BASE)
#define FDCAN_RAM   ((FDCANRAM_TypeDef *)FDCAN_RAM_BASE)
#define USART3      ((USART_TypeDef *) USART3_BASE)

/* Extern Keyword Allows To Be Call */
extern void SystemInit() {
    rcc_write_ahb2_enr(RCC, RCC_GPIOA_AHB2ENR);          // ENABLE GPIOA
    rcc_write_ahb2_enr(RCC, RCC_GPIOB_AHB2ENR);          // ENABLE GPIOB
    rcc_write_ahb2_enr(RCC, RCC_GPIOC_AHB2ENR);          // ENABLE GPIOC
    rcc_write_ahb2_enr(RCC, RCC_GPIOD_AHB2ENR);          // ENABLE GPIOC
    rcc_set_pll(RCC, Rcc_MSI, 48, 0);                   // SET MULTIPLIER AND DIVIDER FOR PLL
    rcc_set_pllclk(RCC, Rcc_PLL6);                      // SET DIVIDER FOR PLLCLK
    rcc_set_pll48clk(RCC, Rcc_PLL4);                    // SET DIVIDER FOR PLL48CLK
    rcc_start_pll(RCC);                                 // START PLL
    rcc_set_pllclk_en(RCC);                             // SET PLLCLK OUTPUT
    rcc_set_pll48clk_en(RCC);                           // SET PLL48CLK OUTPUT
    rcc_write_ccipr1(RCC, RCC_FDCAN_CCIPR1_PLLQCLK);    // SELECT CCIPR1 FDCAN PLLQCLK
    rcc_write_apb1_enr2(RCC, RCC_FDCAN_APB1ENR2);      // ENABLE RCC FDCAN CLOCK
    rcc_write_apb1_enr1(RCC, RCC_USART3_APB1ENR1);
    rcc_write_apb1_enr1(RCC, RCC_TIMER2_APB1ENR1);      // ENABLE TIMER 2
    rcc_sys_clk(RCC, 3);
}

extern void main() {
    // LED
    gpio_type(GPIOA, LED_RED_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    gpio_type(GPIOB, LED_BLU_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    gpio_type(GPIOC, LED_GRN_PIN, Gpio_Output, Gpio_Push_Pull, AF0);
    
    // FDCAN
    gpio_type(GPIOD, FDCAN_TX, FDCAN_MODE, FDCAN_OTYPE, FDCAN_AF);
    gpio_type(GPIOD, FDCAN_RX, FDCAN_MODE, FDCAN_OTYPE, FDCAN_AF);
    gpio_speed(GPIOD, FDCAN_TX, FDCAN_SPEED);
    gpio_speed(GPIOD, FDCAN_RX, FDCAN_SPEED);
    gpio_pupd(GPIOD, FDCAN_TX, FDCAN_PUPD);
    gpio_pupd(GPIOD, FDCAN_RX, FDCAN_PUPD);

    // USART
    gpio_type(GPIOD, USART3_TX, USART_MODE, USART_OTYPE, USART_AF);
    gpio_type(GPIOD, USART3_RX, USART_MODE, USART_OTYPE, USART_AF);

    timer_open(TIMER2, Timer_Cont, Timer_Upcount);
    timer_set_time(TIMER2, 1000, 32000, 10000);
    timer_start(TIMER2);

    // Start FDCAN
    fdcan_open(FDCAN, FDCAN_RAM);
    usart_open(USART3, USART_8_Bits, USART_1_StopBit, USART_921600_BAUD, 32000, USART_Oversample_16);

    FDCANSTDFilter_TypeDef std;
    std.fields.SFID1 = 1;
    std.fields.SFID2 = 2;
    std.fields.SFEC = 1;
    std.fields.SFT = 0;

    FDCANMsgTX_TypeDef fdcantx;
    fdcantx.HEADER.reg = 0;
    fdcantx.HEADER.fields.STID = 5;
    fdcantx.HEADER.fields.XTD = 0;
    fdcantx.HEADER.fields.ESI = 0;
    fdcantx.TX.reg = 0;
    fdcantx.TX.fields.DLC = 15;
    fdcantx.TX.fields.EFC = 1;
    fdcantx.TX.fields.EFC = 0;
    for (u8 j = 0; j < 64; j++) {
        fdcantx.DATA[j] = 0;
    }
    fdcantx.DATA[0] = 1;
    fdcantx.DATA[1] = 2;
    fdcantx.DATA[2] = 3;
    fdcantx.DATA[3] = 4;
    fdcantx.DATA[4] = 5;
    fdcantx.DATA[5] = 6;
    fdcantx.DATA[6] = 0;
    fdcantx.DATA[7] = 0;

    FDCANMsgRX_TypeDef fdcanrx;
    fdcanrx.HEADER.reg = 0;
    fdcanrx.RX.reg = 0;
    for (u8 j = 0; j < 64; j++) {
        fdcanrx.DATA[j] = 0;
    }

    fdcan_set_fls(FDCAN_RAM, 0, std);

    uint8_t usart_obuf[8] = {0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
    uint8_t fdcan_data[67];

    u32 reg = 0;
    u32 i = 0;
 
    while (1) {
        reg = 0;
        for (u8 j = 0; j < 67; j++) {
            fdcan_data[j] = 0;
        }
        fdcan_data[0] = 2;
        fdcan_data[66] = 0xFF;   

        if (fdcan_read(FDCAN, FDCAN_RAM, &fdcanrx)) {
            fdcan_data[1] += 1;
            for (u8 k = 0; k < 64; k++) {
                fdcan_data[k + 2] = fdcanrx.DATA[k];
            }
            usart_write(USART3, fdcan_data, 67);
        }

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

            if (fdcan_write(FDCAN, FDCAN_RAM, &fdcantx)) {
                usart_obuf[0] = 1;
                usart_obuf[1] = 1;
            } else {
                usart_obuf[0] = 1;
                usart_obuf[1] = 0;
            }

            usart_obuf[3] = fdcanrx.DATA[6];
            usart_obuf[4] = fdcanrx.DATA[7];
            usart_obuf[5] = fdcantx.DATA[6];
            usart_obuf[6] = fdcantx.DATA[7];

            usart_write(USART3, usart_obuf, 8);

            fdcantx.DATA[6] += 1;
            fdcantx.DATA[7] += 1;
            usart_obuf[2] += 1;

            if (fdcantx.DATA[7] >= 25) {
                fdcantx.DATA[6] = 0;
                fdcantx.DATA[7] = 0;
                usart_obuf[2] = 0;
            }

            timer_clr_flag(TIMER2);
        }
    }
}

extern void __aeabi_unwind_cpp_pr0() {
    //loop {}
}
