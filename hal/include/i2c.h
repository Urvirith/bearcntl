#ifndef I2C_H_
#define I2C_H_

#include "common.h"

/* Register Base */

#define ADDR_7_BIT_ACT      false
#define ADDR_10_BIT_ACT     true


/* Register Pointer Structure */
typedef struct {
    volatile u32 CR1;       // Control Register 1
    volatile u32 CR2;       // Control Register 2
    volatile u32 OAR1;      // Own Address Register 1
    volatile u32 OAR2;      // Own Address Register 2
    volatile u32 TIMINGR;   // Timing Register
    volatile u32 TIMEOUTR;  // Timeout Register
    volatile u32 ISR;       // Interrupt And Status Register
    volatile u32 ICR;       // Interrupt Flag Clear Register
    volatile u32 PECR;      // PEC Register
    volatile u32 RXDR;      // Receive Data Register
    volatile u32 TXDR;      // Transmit Data Register
} I2C_TypeDef;

/* Enumerations */
/* Speed of I2C bus, 10KHz, 100KHz 400KHz or Plus mode */
typedef enum i2c_timing_mode {
    I2C_Sm_10KHz,
    I2C_Sm_100KHz,
    I2C_Fm_400KHz,
    I2C_Fm_Plus
} I2C_TimingMode;

/* Public Functions */
void i2c_open(I2C_TypeDef *ptr, u32 sclk_mhz, u32 mode);
void i2c_start_bus(I2C_TypeDef *ptr);
void i2c_stop_bus(I2C_TypeDef *ptr);
void i2c_setup(I2C_TypeDef *ptr, u32 slave_addr, bool addr_10bit, bool req_10bit, u32 byte_cnt, bool write);
bool i2c_start(I2C_TypeDef *ptr);
bool i2c_stop(I2C_TypeDef *ptr);
bool i2c_tc(I2C_TypeDef *ptr);
bool i2c_read(I2C_TypeDef *ptr, u8* buf, int len);
u8 i2c_read_u8(I2C_TypeDef *ptr);
bool i2c_write(I2C_TypeDef *ptr, u8* buf, int len);
bool i2c_write_u8(I2C_TypeDef *ptr, u8 byte);

/* Public Combined Functions */
void i2c_std_read(I2C_TypeDef *ptr, u32 slave_addr, bool addr_10bit, bool req_10bit, u8* buf_write, u32 len_write,  u8* buf_read, u32 len_read);
u8 i2c_std_write(I2C_TypeDef *ptr, u32 slave_addr, bool addr_10bit, bool req_10bit, u8* buf, u32 len);
u8 i2c_std_read_u8(I2C_TypeDef *ptr, u32 slave_addr, bool addr_10bit, bool req_10bit, u8 byte_write);
#endif /* I2C_H_ */