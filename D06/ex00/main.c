/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:12:46 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/23 19:31:42 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 1024

#define BLUE (1 << PD3)
#define RED (1 << PD5)
#define GREEN (1 << PD6)

ISR(TIMER1_COMPA_vect)
{
	static uint8_t led_rgb = (1 << PD3);

	PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD3));
	if (led_rgb & BLUE)
		led_rgb = RED;
	else if (led_rgb & RED)
		led_rgb = GREEN;
	else
		led_rgb = BLUE;
	PORTD |= led_rgb;
}

int main(void)
{
	sei();
	DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
	PORTD |= (1 << PD3);

	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU/TIMER1_PRESCALER;

	while (1);
}