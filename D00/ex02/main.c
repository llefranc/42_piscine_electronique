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

void sleep_ms(long ms)
{
	/*
	 * Multiplying per 1000 because 1 million of instruction per second is executed
	 * Dividing by 2 because 2 instructions during the while loop: add and cmp
	 */
	long timer = ms * 1000 / 2;
	long i = 0;

	while (i < timer)
		i++;
}

int main(void)
{
	DDRB |= (1 << PIN0);
	while (1) {
		sleep_ms(500);
		PORTB ^= (1 << PIN0); /* switching between HIGH and LOW for PB0 */
	}
}