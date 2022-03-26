#ifndef RCC_H_
#define RCC_H_

#include "common.h"

// RCC DRIVER
// ARM RCC MODULE
// RCC Description - is on pg 327

/* Reset and Clock Control (RCC) */

/* Register Base */
/* Defined In The Board Specific Folder */

/* Register Pointer Structure */
typedef struct {
    volatile u32 CR;                // Control Register
    volatile u32 ICSCR;             // Internal Clock Sources Calibration Register
    volatile u32 CFGR;              // Clock Configuration Register    
    volatile u32 PLL_CFGR;          // PLL Configuration Register
    volatile u32 PLL_SAI1_CFGR;     // PLL SAI1 Configuration Register
    volatile u32 PLL_SAI2_CFGR;     // PLL SAI2 Configuration Register
    volatile u32 CIER;              // Clock Interrupt Enable Register
    volatile u32 CIFR;              // Clock Interrupt Flag Status Register
    volatile u32 CICR;              // Clock Interrupt Clear Register
    volatile u32 RESERVED_2;        // Reserved Area
    volatile u32 AHB1_RSTR;         // AHB1 Peripheral Reset Register
    volatile u32 AHB2_RSTR;         // AHB2 Peripheral Reset Register
    volatile u32 AHB3_RSTR;         // AHB3 Peripheral Reset Register
    volatile u32 RESERVED_3;        // Reserved Area
    volatile u32 APB1_RSTR1;        // APB1 Peripheral Reset Register 1
    volatile u32 APB1_RSTR2;        // APB1 Peripheral Reset Register 2
    volatile u32 APB2_RSTR;         // APB2 Peripheral Reset Register
    volatile u32 RESERVED_4;        // Reserved Area
    volatile u32 AHB1_ENR;          // AHB1 Peripheral Enable Register
    volatile u32 AHB2_ENR;          // AHB2 Peripheral Enable Register
    volatile u32 AHB3_ENR;          // AHB3 Peripheral Enable Register
    volatile u32 RESERVED_5;        // Reserved Area
    volatile u32 APB1_ENR1;         // APB1 Peripheral Enable Register 1
    volatile u32 APB1_ENR2;         // APB1 Peripheral Enable Register 2
    volatile u32 APB2_ENR;          // APB2 Peripheral Enable Register
    volatile u32 RESERVED_6;        // Reserved Area
    volatile u32 AHB1_SM_ENR;       // AHB1 Peripheral Sleep And Stop Modes Enable Register
    volatile u32 AHB2_SM_ENR;       // AHB2 Peripheral Sleep And Stop Modes Enable Register
    volatile u32 AHB3_SM_ENR;       // AHB3 Peripheral Sleep And Stop Modes Enable Register
    volatile u32 RESERVED_7;        // Reserved Area
    volatile u32 APB1_SM_ENR1;      // APB1 Peripheral Sleep And Stop Modes Enable Register 1
    volatile u32 APB1_SM_ENR2;      // APB1 Peripheral Sleep And Stop Modes Enable Register 2
    volatile u32 APB2_SM_ENR;       // APB2 Peripheral Sleep And Stop Modes Enable Register
    volatile u32 RESERVED_8;        // Reserved Area
    volatile u32 CCIPR1;            // Peripherals Independent Clock Configuration Register
    volatile u32 BDCR;              // Backup Domain Control Register
    volatile u32 CSR;               // Control Status Register
    volatile u32 CRRCR;             // Clock Recovery RC Register
    volatile u32 CCIPR2;            // Peripherals Independent Clock Configuration Register
    volatile u32 RESERVED_9;        // Reserved Area
    volatile u32 DLYCFGR;           // OCTOSPI delay configuration register
} RCC_TypeDef;

/* Public Functions */
void rcc_write_msi_range(RCC_TypeDef *ptr, MSI_Range freq_khz);
void rcc_write_ahb1_enr(RCC_TypeDef *ptr, u32 bit_num);
void rcc_write_ahb2_enr(RCC_TypeDef *ptr, u32 bit_num);
void rcc_write_ahb3_enr(RCC_TypeDef *ptr, u32 bit_num);
void rcc_write_apb1_enr1(RCC_TypeDef *ptr, u32 bit_num);
void rcc_write_apb1_enr2(RCC_TypeDef *ptr, u32 bit_num);
void rcc_write_apb2_enr(RCC_TypeDef *ptr, u32 bit_num);
void rcc_write_ccipr1(RCC_TypeDef *ptr, u32 offset, u32 value);

#endif /* RCC_H_ */