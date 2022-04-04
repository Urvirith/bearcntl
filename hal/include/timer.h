#ifndef TIMER_H_
#define TIMER_H_

#include "common.h"
#include "pointer.h"

// TIMER DRIVER
// ARM TIMER MODULE - FREE RUNNING COUNTERS -32kHz or 25MHz
// Timer Description - is on pg 1181

/* Timers */
typedef struct {
    volatile u32 CR1;               // Control Register 1
    volatile u32 CR2;               // Control Register 2
    volatile u32 SMCR;              // Slave Mode Control Register
    volatile u32 DIER;              // DMA/ Interrupt Enable Register
    volatile u32 SR;                // Status Register
    volatile u32 EGR;               // Event Generation Register
    volatile u32 CCMR1;             // Capture/Compare Mode Register 1
    volatile u32 CCMR2;             // Capture/Compare Mode Register 2
    volatile u32 CCER;              // Capture/Compare Enable Register
    volatile u32 CNT;               // Counter Register
    volatile u32 PSC;               // Prescaler Register
    volatile u32 ARR;               // Auto Reload Register
    volatile u32 RCR;               // Repetition Counter Register
    volatile u32 CCR1;              // Capture/Compare Register 1
    volatile u32 CCR2;              // Capture/Compare Register 2
    volatile u32 CCR3;              // Capture/Compare Register 3
    volatile u32 CCR4;              // Capture/Compare Register 4
    volatile u32 BDTR;              // Break and Dead-Time Register
    volatile u32 DCR;               // DMA Control Register
    volatile u32 DMAR;              // DMA Address Register
    volatile u32 OR1;               // Option Register 1
} TIMER_TypeDef;

/* Enumerations */
// 0 = Continous, 1 = ONS (clears en bit)
typedef enum timer_type {
    Timer_Cont, 
    Timer_Ons
} TIMER_Type;

// 0 = Upcounter, 1 = Downcounter
typedef enum timer_direction {
    Timer_Upcount,
    Timer_Down
} TIMER_Direction;

/* Public Functions */
void timer_open(TIMER_TypeDef *ptr, TIMER_Type ons, TIMER_Direction dir);
bool timer_get_flag(TIMER_TypeDef *ptr);
void timer_clr_flag(TIMER_TypeDef *ptr);
void timer_set_interrupt(TIMER_TypeDef *ptr);
u32 timer_read_count(TIMER_TypeDef *ptr);
void timer_clr_count(TIMER_TypeDef *ptr);
void timer_start(TIMER_TypeDef *ptr);
void timer_stop(TIMER_TypeDef *ptr);
void timer_set_time(TIMER_TypeDef *ptr, u32 time, u32 clock_speed, u32 prescale);
void timer_set_pwm_ccr1(TIMER_TypeDef *ptr, u32 cnt);
void timer_set_pwm_ccr2(TIMER_TypeDef *ptr, u32 cnt);
void timer_set_pwm_ccr3(TIMER_TypeDef *ptr, u32 cnt);
void timer_set_pwm_ccr4(TIMER_TypeDef *ptr, u32 cnt);
void timer_set_pwm_ch1(TIMER_TypeDef *ptr);
void timer_set_pwm_ch2(TIMER_TypeDef *ptr);
void timer_set_pwm_ch3(TIMER_TypeDef *ptr);
void timer_set_pwm_ch4(TIMER_TypeDef *ptr);

#endif /* TIMER_H_ */
