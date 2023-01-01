/*
 * dym2149.h
 *
 * Created: 2022-12-29 17:03:21
 *  Author: Bartosz Derleta <bartosz@derleta.com>
 */ 


#ifndef DYM2149_H_
#define DYM2149_H_

#include "pin_config.h"

void ym_address(uint8_t addr);
void ym1_address(uint8_t addr);
void ym2_address(uint8_t addr);

void ym_write(uint8_t data);
void ym1_write(uint8_t data);
void ym2_write(uint8_t data);

void ym_set(uint8_t addr, uint8_t data);
void ym1_set(uint8_t addr, uint8_t data);
void ym2_set(uint8_t addr, uint8_t data);
void ym12_set(uint8_t addr, uint8_t data1, uint8_t data2);

uint8_t ym1_read(void);
uint8_t ym2_read(void);

#endif /* DYM2149_H_ */