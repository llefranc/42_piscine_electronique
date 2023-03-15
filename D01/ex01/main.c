/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:28:44 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/15 18:53:27 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main(void)
{
	DDRB |= (1 << PB1);

	/*
	 * PWM, Phase and Frequency Correct + ICR1 as TOP value + prescaler set
	 * as 1024
	 */
	TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);

	/*
	 * Clear OC1A/OC1B on Compare Match when up-counting.
	 * Set OC1A/OC1B on Compare Match when downcounting.
	 */
	TCCR1A |= (1 << COM1A1);

	/* TOP value, clock frequency divided by prescaler value */
	ICR1 = F_CPU / 1024;
	/* MATCH value >>>> a verifier */
	OCR1A = F_CPU / 1024 / 20;

	while (1) {}
}