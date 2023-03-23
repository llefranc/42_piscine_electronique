/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:12:46 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/23 19:48:46 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 1024

#define R_LED (1 << PD5)
#define G_LED (1 << PD6)
#define B_LED (1 << PD3)

// rouge ff 0 0
// vert 0 ff 0
// bleu 0 0 ff
// yellow ff ff 0
// cyan 0 ff ff
// magenta ff 0 ff
// white ff ff ff

uint8_t colors[7] = {
	R_LED,
	G_LED,
	B_LED,
	R_LED | G_LED,
	G_LED | B_LED,
	R_LED | B_LED,
	R_LED | G_LED | B_LED,
};

ISR(TIMER1_COMPA_vect)
{
	static uint8_t i = 0;

	PORTD &= ~(R_LED | G_LED | B_LED);
	if (++i == sizeof(colors) / sizeof(*colors))
		i = 0;
	PORTD |= colors[i];
}

int main(void)
{
	sei();
	DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
	PORTD |= R_LED;

	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU/TIMER1_PRESCALER;

	while (1);
}