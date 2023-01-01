/*
 * pin_config.c
 *
 * Created: 2022-12-29 15:38:26
 *  Author: Bartosz Derleta <bartosz@derleta.com>
 */ 
#include "pin_config.h"

void setup(void)
{
	/* Initialize PORT/DDR */
	PORTA = PORTA_INIT;
	DDRA = DDRA_INIT;
	PORTB = PORTB_INIT;
	DDRB = DDRB_INIT;
	PORTC = PORTC_INIT;
	DDRC = DDRC_INIT;
	PORTD = PORTD_INIT;
	DDRD = DDRD_INIT;
	
	/* USART setup */
	/* 0.5M speed - according to table 71, page 168 of Doc2503 */
	uint16_t UBRR = 0x03; 
	UBRRL = UBRR & 0xFF;
	/* The UBRRH Register shares the same I/O location as the UCSRC Register, contains 4 high bits */
	UBRRH = (UBRR >> 8) & ~(1 << URSEL) & 0xF;	
	/* 2x mode */
	UCSRA = (1 << U2X);
	/* Set data frame format: asynchronous mode, no parity, 1 stop bit, 8 bit size */
	UCSRC = (1 << URSEL) | (0 << UMSEL) | (0 << UPM1) | (0 << UPM0) | (0 << USBS) | (0 << UCSZ2) | (1 << UCSZ1) | (1 << UCSZ0);
	/* RX complete interrupt, TX complete interrupt, UDR empty interrupt, RX on, TX on, 9th bit enable, received 9th bit, transmitted 9th bit */
	UCSRB = (1 << RXCIE) | (0 << TXCIE) | (0 << UDRIE) | (1 << RXEN) | (1 << TXEN) | (0 << UCSZ2) | (0 << RXB8) | (0 << TXB8); 
	
	/* Initialize SPI */
	SPCR |= (1 << SPE) | (1 << MSTR);
	SPSR |= (1 << SPI2X);
	
	/* Disable 595 clear flag */
	PORTB |= U595_SRCLR;
	
	/* Activate INT2 button on falling edge */
	MCUCSR &= ~(1 << ISC2);
	GICR |= (1 << INT2);
	
	/* Timer setup */
	
	/* Timer0 CTC, OC0 disconnected, 1 divider */
	TCCR0 = (0 << FOC0) | (0 << WGM00) | (0 << COM01) | (0 << COM00) | (1 << WGM01) | (1 << CS02) | (0 << CS01) | (1 << CS00);
	/* Comp0 trigger value */
	OCR0 = 0x9B;
	/* Timer1 CTC, 256 divider */
	TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << FOC1A) | (0 << FOC1B) | (0 << WGM11) | (0 << WGM10);
	TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (1 << WGM12) | (1 << CS12) | (0 << CS11) | (0 << CS10);
	/* Comp1a trigger value */
	OCR1A = 1249;
	/* Activate Timer2 external XTAL - asynchronous operation, requires polling ASSR on TCCR2/OCR2/TCNT2 setting */
	ASSR |= (1 << AS2); 
	/* Timer2 Waveform normal, OC2 disconnected, 128 divider */
	TCCR2 = (0 << FOC2) | (0 < WGM20) | (0 << COM21) | (0 << COM20) | (0 << WGM21) | (1 << CS22) | (0 << CS21) | (1 << CS20); 
	/* Wait for asynchronous set */
	while (ASSR & (1 << TCR2UB)); 
	/* Comp2 trigger value */
	OCR2 = 0xFF;
	/* Wait for asynchronous set */
	while (ASSR & (1 << OCR2UB)); 
	/* Comp2 interrupt, Ovf2 interrupt */
	TIMSK = (0 << OCIE2) | (1 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) | (0 << OCIE1B) | (0 << TOIE1) | (1 << OCIE0) | (0 << TOIE0);
	
	/* Enable global interrupts */
	set_sleep_mode(SLEEP_MODE_IDLE);
	sei();
}

void u595_set(unsigned char byte) {
	SPDR = byte;
	while (!(SPSR & (1<<SPIF)));
	u595_rclk_high();
	// 24ns max
	u595_rclk_low();
}

void u595_clear(void) {
	PORTB &= ~U595_SRCLR;
	// 24ns max
	PORTB |= U595_SRCLR;
	u595_rclk_high();
	// 24ns max
	u595_rclk_low();
}