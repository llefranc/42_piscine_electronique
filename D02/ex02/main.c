/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:22:15 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/21 14:13:58 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static uint8_t nb = 0;

static void update_led(uint8_t nb);

ISR(INT0_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed) {
		nb++;
		update_led(nb);
	}

	_delay_ms(20);        /* Neutralizing bounce effect */
	EIFR |= (1 << INTF0); /* Clearing interrupt flag one occured while ISR was executing*/
}

ISR(PCINT2_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed) {
		nb--;
		update_led(nb);
	}

	_delay_ms(20);         /* Neutralizing bounce effect */
	PCIFR |= (1 << PCIF2); /* Clearing interrupt flag if another one occured while ISR was executing*/
}

static void update_led(uint8_t nb)
{
	uint8_t tmp = nb % 16;

	PORTB &= ~((1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0));
	if (tmp & (1 << 3)) {
		tmp |= (1 << PB4);
		tmp &= ~(1 << 3);
	}
	PORTB |= tmp;
}

int main(void)
{
	sei();
	EIMSK |= (1 << INT0);  /* Enabling interrupt INT0 for SW1*/
	EICRA |= (1 << ISC00); /* Any logical change on INT0 generates an interrupt request. */

	PCICR |= (1 << PCIE2);    /* Enabling PCINT2 interrupt for PIN16-23 */
	PCMSK2 |= (1 << PCINT20); /* Only pin with PCINT20 can trigger PCINT2 (SW2) */

	DDRB |= (1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0);

	while (1) {}
}
