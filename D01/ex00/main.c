/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:17:38 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/15 17:15:59 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/**
	 * --------------------------------------------------------------------
	 * Setting up PB1 without PORTB1
	 *
	TCCR1A |= (1 << COM1A0); // toggle pin PB1
	TCCR1A |= (1 << COM1A1); // clear pin PB1 (LOW)
	TCCR1A |= (1 << COM1A1) | (1 << COM1A0); // set pin PB1 (HIGH)

	TCCR1C |= (1 << FOC1A); // force match
	DDRB |= (1 << PB1);
	 * --------------------------------------------------------------------
	*/

	DDRB |= (1 << PB1);
	TCCR1B |= (1 << CS12); // prescaler 256
	TCCR1A |= (1 << COM1A0); // toggle
	TCCR1B |= (1 << WGM12); // Clear Timer on Compare Match (CTC) Mode
	OCR1A = F_CPU / 256 / 2; // F_CPU / prescaler * 0,5 sec

	while (1) {}
}