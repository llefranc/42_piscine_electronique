/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:12:46 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/24 16:42:30 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define R_LED (1 << PD5)
#define G_LED (1 << PD6)
#define B_LED (1 << PD3)

#define TIMER1_PRESCALER 1024
#define FREQ_COLOR_CHANGE 64 /* 64Hz, we change color 64 times per sec */

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
	static uint8_t grad = 0;

	wheel(grad++);
}

int main(void)
{
	sei();
	DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);

	/*
	 * Timer 1 will change color 64 times per second, and trigger
	 * timer0/timer2 PWM updates
	 */
	OCR1A = F_CPU / TIMER1_PRESCALER / FREQ_COLOR_CHANGE;
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); /* prescaler 1024 */
	TIMSK1 |= (1 << OCIE1A);

	while (1);
}
