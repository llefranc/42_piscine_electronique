/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/25 19:57:48 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 256

#define R_LED (1 << PD5)
#define G_LED (1 << PD6)
#define B_LED (1 << PD3)

enum e_timer_dir {
	E_UPCOUNTING,
	E_DOWNCOUNTING
};

enum e_timer_dir timer0_dir = E_UPCOUNTING;
enum e_timer_dir timer2_dir = E_UPCOUNTING;

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	/* Deactivating timers interrupt when changing duty cycle registers */
	cli();

	TCCR0A |= (1 << WGM00); /* PWM phase correct mode with TOP value 0xFF */
	TCCR0B |= (1 << CS02);  /* starting timer0 with prescaler 64 */
	TIMSK0 |= (1 << OCIE0B) | (1 << OCIE0A) | (1 << TOIE0);

	TCCR2A |= (1 << WGM20); /* PWM phase correct mode with TOP value 0xFF */
	TCCR2B |= (1 << CS22);  /* starting timer2 with prescaler 64 */
	TIMSK2 |= (1 << OCIE2A) | (1 << TOIE2);

	OCR0A = r;
	OCR0B = g;
	OCR2A = b;

	TCNT0 = 0;
	TCNT2 = 0;
	sei();
}

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}


/* Occurs at BOTTOM */
ISR(TIMER0_OVF_vect)
{
	timer0_dir = E_UPCOUNTING;
}

/* Occurs at BOTTOM */
ISR(TIMER2_OVF_vect)
{
	timer2_dir = E_UPCOUNTING;
}

/* Red duty cycle */
ISR(TIMER0_COMPA_vect)
{
	if (!OCR0A)
		PORTD &= ~R_LED;
	else if (OCR0A == 0xFF)
		PORTD |= R_LED;
	else if (timer0_dir == E_UPCOUNTING)
		PORTD &= ~R_LED;
	else
		PORTD |= R_LED;
	timer0_dir = E_DOWNCOUNTING;
}

/* Green duty cycle */
ISR(TIMER0_COMPB_vect)
{
	if (!OCR0B)
		PORTD &= ~G_LED;
	else if (OCR0B == 0xFF)
		PORTD |= G_LED;
	else if (timer0_dir == E_UPCOUNTING)
		PORTD &= ~G_LED;
	else
		PORTD |= G_LED;
	timer0_dir = E_DOWNCOUNTING;
}

/* Blue duty cycle */
ISR(TIMER2_COMPA_vect)
{
	if (!OCR2A)
		PORTD &= ~B_LED;
	else if (OCR2A == 0xFF)
		PORTD |= B_LED;
	else if (timer2_dir == E_UPCOUNTING)
		PORTD &= ~B_LED;
	else
		PORTD |= B_LED;
	timer2_dir = E_DOWNCOUNTING;
}

ISR(TIMER1_COMPA_vect)
{
	uint8_t pot_val = adc_read_8bit();

	wheel(pot_val);
	PORTB &= ~((1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0));
	if (pot_val > 0xFF / 4)
		PORTB |= (1 << PB0);
	if (pot_val > 0xFF / 4 * 2)
		PORTB |= (1 << PB1);
	if (pot_val > 0xFF / 4 * 3)
		PORTB |= (1 << PB2);
	if (pot_val == 0xFF)
		PORTB |= (1 << PB4);
}

int main(void)
{
	sei();
	DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
	DDRB |= (1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0);
	uart_init(UART_UBRRN, 0);

	adc_init(ADC_PRESC_128, (1 << ADLAR));
	adc_mux_select(ADC_POT);

	/* Timer 1 match every 20 ms */
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	OCR1A = F_CPU / TIMER1_PRESCALER / 100 * 2;

	while (1);
}