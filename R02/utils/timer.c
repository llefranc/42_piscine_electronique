/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:50:09 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 15:50:59 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timer.h"

#include <avr/io.h>

void timer0_clear(void)
{
	TIMSK0 = 0;
	OCR0A = 0;
	OCR0B = 0;
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0 = 0;
}

void timer1_clear(void)
{
	TIMSK1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
}
