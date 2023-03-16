/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:16:32 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/16 20:01:15 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)                /* Switching LED output only when button is pressed, and not when it is r  */
		PORTB ^= (1 << PB0);

	_delay_ms(20);                 /* Neutralizing bounce effect */
	if (EIFR & (1 << INTF0))       /* Clearing interrupt if another one occured while ISR was executing*/
		EIFR |= (1 << INTF0);
}

int main(void)
{
	sei();                   /* set Global Interrupt Enable bit in SREG*/
	EIMSK |= (1 << INT0);    /* Enable INT0 */
	EICRA |= (1 << ISC00);   /* Any logical change on INT0 generates an interrupt request */
	DDRB |= (1 << PB0);
	while (1) {}
}