/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:04:31 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 14:08:43 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

#define TIMER1_MS_PRESCALER 256


/**
 * start_timer1_ms() - Starts timer1 that will trigger a match every x ms.
 * @ms: The period between each match.
 *
 * Each match will trigger a TIMER1_COMPA_vect interrupt. Range: 0 - 1000 ms.
 * Precision: 10ms.
 * Return: -1 if ms > 1000. 0 otherwise.
*/
static inline int8_t start_timer1_ms(uint16_t ms)
{
	if (ms > 1000)
		return -1;
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU / TIMER1_MS_PRESCALER / 100 * (ms / 10) +
			F_CPU / TIMER1_MS_PRESCALER / 1000 * (ms % 10);
	return 0;
}

/**
 * stop_timer1() - Stops timer1.
*/
static inline void stop_timer1(void)
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

#endif