/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:22:15 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/16 18:33:35 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>

#define PRESCALER 1024
#define PERIOD 78

ISR(TIMER0_COMPA_vect)
{
	static uint8_t duty_cycle = 1;
	static int8_t dir = 1;

	if (!duty_cycle || duty_cycle == 100)
		dir = dir * -1;
	duty_cycle += dir;
	OCR1A = duty_cycle * PERIOD;
	// PORTB ^= (1 << PB0);
	// OCR1A = PERIOD * 50;

}

int main(void)
{
	// TCNT0 > match 200 fois / sec pour faire 0% > 100% > 0% en 1 sec
	// 16 000 000 / 1024 = 15625 clock cycle in 1 sec
	// 15625 / 200 = 78,125 >> MATCH VALUE for TCNT0, on doit la match 200 fois

	// TCNT1
	// 16 000 000 / 1024 = 15625 clock cycle in 1 sec
	// 15625 / 2 = 7812,5 >> TOP value, on l'atteint au bout de 0,5 sec
	// 1% = 78,125 >> MATCH VALUE pour un duty cycle de 1%

	sei();
	DDRB |= (1 << PB1);

	TCCR0B |= (1 << CS02) | (1 << CS00); // presacler 1024
	OCR0A = F_CPU / PRESCALER / 200; // TOP value, matches every 0,005 sec
	TIMSK0 |= (1 << OCIE0A); // interrupt value for ocr0A match

	ICR1 = F_CPU / PRESCALER / 2;
	TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);
	TCCR1A |= (1 << COM1A1);
	// OCR1A = PERIOD * 50;
	// OCR1A = PERIOD * 50;

	while (1) {}
}