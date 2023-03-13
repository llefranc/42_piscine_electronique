/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:54:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/13 15:57:50 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main(void)
{
	/*
	 * DDRB0 = 1   (binary: 0000001)
	 * Sets the direction of the pin to output
	 */
	DDRB |= (1 << PB0);

	/*
	 * PORTB0 = 1   (binary: 0000001)
	 * The pin configured as output is set to HIGH
	 */
	PORTB |= (1 << PB0);
	return 0;
}