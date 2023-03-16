/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:22:15 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/16 19:27:38 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER0_PRESCALER 1024
#define TIMER1_PRESCALER 8

static uint8_t period;

ISR(TIMER0_COMPA_vect)
{
	static int8_t duty_cycle = 1;
	static int8_t dir = 1;

	if (!duty_cycle || duty_cycle == 100)
		dir = dir * -1;
	duty_cycle += dir;            /* from 0% to 100% to 0% in 1 sec */
	OCR1A = duty_cycle * period;
}

int main(void)
{
	/*
	 * --------- Timer0 calculation ---------
	 * Using a prescaler of 1024: 16 000 000 / 1024 = 15625 incrementation of TCNT0 in 1 sec
	 * 15625 / 200 = 78,125 >> MATCH VALUE that for TCNT0
	 */

	/*
	 * --------- Timer1 calculation ---------
	 * Using a prescaler of 8: 16 000 000 / 8 = 2 000 000 incrementation of TCNT1 in 1 sec
	 *
	 * We need to reach TOP 200 hundread times per second to update OCR1A:
	 * 2 000 000 / 200 = 10 000 => and since PWM is doing BOTTOM -> TOP -> BOTTOM,
	 * we need to divide it by 2 => TOP VALUE = 5 000
	 *
	 * Then calculating 1% of duty cycle: 5000 / 100 = 50
	 */

	sei();
	DDRB |= (1 << PB1);

	TCCR0A |= (1 << WGM01);                 /* CTC mode */
	TCCR0B |= (1 << CS02) | (1 << CS00);    /* Prescaler 1024 */
	OCR0A = F_CPU / TIMER0_PRESCALER / 200; /* TOP value for CTC mode */
	TIMSK0 |= (1 << OCIE0A);                /* Enabling interrupt when TCNT0 compare OCR0A */

	ICR1 = F_CPU / TIMER1_PRESCALER / 200 / 2;
	period = ICR1 / 100;

	TCCR1B |= (1 << WGM13) | (1 << CS11); /* PWM Phase and Frequency correct + ICR as TOP value + prescaler 8 */
	TCCR1A |= (1 << COM1A1);              /* Output mode */

	while (1) {}
}