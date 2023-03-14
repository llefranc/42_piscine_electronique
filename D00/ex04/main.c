/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:54:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/13 17:48:18 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/* PB0 = LED D1 | PD2 = SW1 */
	char is_pressed = 0;

	DDRB |= (1 << PB0);
	PORTD |= (1 << PD2);
	while (1) {
		if (!is_pressed && !(PIND & (1 << PD2))) {	/* button from state RELEASE > PRESS */
			is_pressed = 1;		
			_delay_ms(50);				/* neutralizing switch bounce effect */
			PORTB ^= (1 << PB0);			/* inversing LED ON/OFF */
		}
		if (is_pressed && (PIND & (1 << PD2))) {	/* button from state RELEASE > PRESS */
			is_pressed = 0;
			_delay_ms(50);
		}
	}
}

