/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:54:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/14 11:19:35 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main(void)
{
	/* PB0 = LED D1 | PD2 = SW1 */

	DDRB |= (1 << PB0);			/* setting PB0 as output */
	while (1) {
		if (PIND & (1 << PD2))		/* if true, means there is current >> sw1 is not pressed */
			PORTB &= ~(1 << PB0);   /* set LED OFF */
		else
			PORTB |= (1 << PB0);    /* set LED ON */
	}
}