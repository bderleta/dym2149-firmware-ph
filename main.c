/*
 * main.c
 *
 * Created: 12/3/2022 2:37:34 AM
 *  Author: Bartosz Derleta <bartosz@derleta.com>
 */ 

#include <util/delay.h>
#include "pin_config.h"
#include "dym2149.h"

volatile uint8_t ubuf[2];
volatile uint8_t upos = 0;

/* USART byte received */
ISR(USART_RXC_vect) {
	if (upos < sizeof(ubuf)) {
		ubuf[upos++] = UDR;
		UDR = 0x06; // ACK
	} else {
		UDR; /* UART Receiver Complete Interrupt routine must read UDR in order to clear RXC, otherwise a new interrupt will occur once the interrupt routine terminates. */
		UDR = 0x15; // NAK
	}
}

/* INT2 button pressed */
ISR(INT2_vect) {
	UDR = 0x07; // BEL
}

ISR(TIMER0_COMP_vect)
{
	// noop
}

/* 1s interrupt */
ISR(TIMER2_OVF_vect)
{
	// noop
}

int main(void)
{
	setup();
	
	_delay_ms(50);
	ym_address(0x07);
	ym_write(0b00111111); // enable PA/PB as outputs, disable audio out
	u595_enable();
	for/*ever*/(;;) {
		if (upos == 2) {
			cli();
			if ((ubuf[0] & 0xF0) == 0) { // Send to both
				u595_set(0b00000001);
				ym_set(ubuf[0], ubuf[1]);
			} else if ((ubuf[0] & 0xF0) == 0x10) { // Send to first
				u595_set(0b00000010);
				ym1_set(ubuf[0] & 0x0F, ubuf[1]);
			} else if ((ubuf[0] & 0xF0) == 0x20) { // Send to second
				u595_set(0b00000100);
				ym2_set(ubuf[0] & 0x0F, ubuf[1]);
			}
			upos = 0;
			sei();
			u595_clear();
		}
		nop();
	}
}