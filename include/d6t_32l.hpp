#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdbool.h>
#include <time.h>
#include <linux/i2c.h> //add

/* defines */
#define D6T_ADDR 0x0A  // for I2C 7bit address
#define D6T_CMD 0x4D  // for D6T-32L-01A, compensated output.
#define D6T_SET_ADD 0x01

#define N_ROW 32
#define N_PIXEL (32 * 32)
#define N_READ ((N_PIXEL + 1) * 2 + 1)
#define RASPBERRY_PI_I2C    "/dev/i2c-1"
#define I2CDEV              RASPBERRY_PI_I2C

/******* setting parameter *******/
#define D6T_IIR 0x00 
#define D6T_AVERAGE 0x04  
/*********************************/

void initialSetting(void);
void delay(int);
uint32_t i2c_read_reg8(uint8_t, uint8_t,
	uint8_t *, int
);
uint32_t i2c_write_reg8(uint8_t,
                        uint8_t *, int);
uint8_t calc_crc(uint8_t) ;
bool D6T_checkPEC(uint8_t *, int);
int16_t conv8us_s16_le(uint8_t* , int);

