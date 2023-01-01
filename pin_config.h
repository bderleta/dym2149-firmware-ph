/*
 * pin_config.h
 *
 * Created: 2022-12-03 02:42:55
 *  Author: Bartosz Derleta <bartosz@derleta.com>
 */ 

#ifndef PIN_CONFIG_H_
#define PIN_CONFIG_H_

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/* Port/Direction config for DA0-DA7 port of YM */
/* PA0 - PA7 are address lines for both YM UC */
#define YM_DA_PORT PORTA
#define YM_DA_DDR DDRA
#define YM_DA_PIN PINA
#define DDRA_INIT 0b11111111
#define PORTA_INIT 0b00000000

/* Port D config */
#define YM1_SEL_BIT (1 << 2) // PD2
#define YM1_BDIR_BIT (1 << 3) // PD3
#define YM1_BC1_BIT (1 << 4) // PD4
#define YM2_SEL_BIT (1 << 5) // PD5
#define YM2_BDIR_BIT (1 << 6) // PD6
#define YM2_BC1_BIT (1 << 7) // PD7
// BC2 = 1 perm. for AY-3-8913 / YM
/* PD0 as input (USART), rest as output, SEL high */
#define DDRD_INIT 0b11111110
#define PORTD_INIT 0b00100100

/* Port/Direction for 595 */
#define U595_SER (1 << 5) // PB5 MOSI
#define U595_RCLK (1 << 0) // PB0
#define U595_SRCLK (1 << 7) // PB7 SCK
#define U595_SRCLR (1 << 3) // PB3
#define U595_OE (1 << 1) // PB1 595_~{OE}
#define u595_rclk_high() (PORTB |= U595_RCLK)
#define u595_rclk_low() (PORTB &= ~U595_RCLK)
#define u595_enable() (PORTB &= ~U595_OE)
#define u595_disable() (PORTB |= U595_OE)
/* 595_X out, MOSI/CS/SCK out, INT2/MISO in */
#define DDRB_INIT 0b10111011
/* ~{CS}/595_~{OE} = 1 (inactive), 595_SRCLR = 0 (active), MOSI/MISO/SCK/595_RCLK = 0 (inactive/no pullup) */
#define PORTB_INIT 0b00010010

/* SCL/SDA, JTAG, TOSC all input */
#define DDRC_INIT 0b00000011
#define PORTC_INIT 0b00000000

#define nop() { asm("nop"); }

void setup(void);

void u595_set(unsigned char byte);
void u595_clear(void);


/* 

UART
	PD0 RXD
	PD1 TXD

External 32768Hz XTAL
	PC6 TOSC1
	PC7 TOSC2

I2C bus
	PC0 SCL
	PC1 SDA

JTAG
	PC2 TCK
	PC3 TMS
	PC4 TDO
	PC5 TDI

External function button
	PB2 INT2 (active low)
	
SRAM socket
	PB5 MOSI
	PB6 MISO
	PB7 SCK
	PC4 ~{CS}

*/

#endif /* PIN_CONFIG_H_ */