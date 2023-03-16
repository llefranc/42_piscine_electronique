/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:28:44 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/16 11:04:26 by llefranc         ###   ########.fr       */
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

	/*
	 * TOP value, clock frequency divided by prescaler value. Dividing by 2
	 * because of double slopes of PWM. So the timer will increment and
	 * reach TOP value in 0,5 sec, and then decrement and reach BOTTOM value
	 * (0) in 0,5 sec giving a cycle of 1 sec.
	 */
	ICR1 = F_CPU / 1024 / 2;

	/*
	 * MATCH value, clock frequency divided by prescaler value. Dividing by
	 * 2 because of double slopes of PWM, and dividing by 10 because we want
	 * a duty cycle of 10%. So the LED will be on during 5% of 0,5 sec for
	 * the first slope, and 5% of 0,5 sec of second slope == 10% of 1 cycle
	 */
	OCR1A = F_CPU / 1024 / 2 / 10;

	while (1) {}
}