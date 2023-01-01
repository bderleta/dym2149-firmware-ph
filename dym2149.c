/*
 * dym2149.c
 *
 * Created: 2022-12-29 17:04:13
 *  Author: Bartosz Derleta <bartosz@derleta.com>
 */ 

#include "dym2149.h"

void ym_set(uint8_t addr, uint8_t data) {
	ym_address(addr);
	ym_write(data);
}

void ym1_set(uint8_t addr, uint8_t data) {
	ym1_address(addr);
	ym1_write(data);
}

void ym2_set(uint8_t addr, uint8_t data) {
	ym2_address(addr);
	ym2_write(data);
}

void ym12_set(uint8_t addr, uint8_t data1, uint8_t data2) {
	ym_address(addr);
	ym1_write(data1);
	ym2_write(data2);
}

uint8_t ym1_read(void) {
	uint8_t val;
	DDRA = 0b00000000; /* PA as input */
	PORTD |= (YM1_BC1_BIT);
	nop(); // Tda 400ns
	val = PINA;
	PORTD &= (~YM1_BC1_BIT);
	nop(); // Tts 100ns
	DDRA = 0b11111111; /* PA as output */
	return val;
}

uint8_t ym2_read(void) {
	uint8_t val;
	DDRA = 0b00000000; /* PA as input */
	PORTD |= (YM2_BC1_BIT);
	nop(); // Tda 400ns
	val = PINA;
	PORTD &= (~YM2_BC1_BIT);
	nop(); // Tts 100ns
	DDRA = 0b11111111; /* PA as output */
	return val;
}

void ym_address(uint8_t addr) {
	PORTD |= (YM1_BDIR_BIT | YM1_BC1_BIT | YM2_BDIR_BIT | YM2_BC1_BIT);
	PORTA = addr;
	nop(); // 300ns
	PORTD &= (~YM1_BDIR_BIT & ~YM1_BC1_BIT & ~YM2_BDIR_BIT & ~YM2_BC1_BIT);
	// 80ns
}

void ym1_address(uint8_t addr) {
	PORTD |= (YM1_BDIR_BIT | YM1_BC1_BIT);
	PORTA = addr;
	nop(); // 300ns
	PORTD &= (~YM1_BDIR_BIT & ~YM1_BC1_BIT);
	// 80ns
}

void ym2_address(uint8_t addr) {
	PORTD |= (YM2_BDIR_BIT | YM2_BC1_BIT);
	PORTA = addr;
	nop(); // Tas 300ns
	PORTD &= (~YM2_BDIR_BIT & ~YM2_BC1_BIT);
	// Tah 80ns
}

void ym_write(uint8_t data) {
	PORTA = data;
	// Tds 0ns
	PORTD |= (YM1_BDIR_BIT | YM2_BDIR_BIT);
	nop(); nop(); nop(); // Tdw 300 - 10000ns
	// Tdh 80ns
	PORTD &= (~YM1_BDIR_BIT & ~YM2_BDIR_BIT);
}

void ym1_write(uint8_t data) {
	PORTA = data;
	// Tds 0ns
	PORTD |= (YM1_BDIR_BIT);
	nop(); nop(); nop(); // Tdw 300 - 10000ns
	// Tdh 80ns
	PORTD &= (~YM1_BDIR_BIT);
}

void ym2_write(uint8_t data) {
	PORTA = data;
	// Tds 0ns
	PORTD |= (YM2_BDIR_BIT);
	nop(); nop(); nop(); // Tdw 300 - 10000ns
	// Tdh 80ns
	PORTD &= (~YM2_BDIR_BIT);
}