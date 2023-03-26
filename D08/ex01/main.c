/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 16:38:31 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 1024

ISR(TIMER1_COMPA_vect)
{
	static uint8_t i = 1;
	static const uint32_t col[] = { RED, GREEN, BLUE, YELLOW, CYAN,
					MAGENTA, WHITE };

	spi_set_leds(B_LOW | col[i], LED_OFF, LED_OFF);
	if (++i >= sizeof(col) / sizeof(*col))
		i = 0;
}

int main(void)
{
	sei();
	spi_init();
	spi_set_leds(B_LOW | RED, LED_OFF, LED_OFF);

	OCR1A = F_CPU / TIMER1_PRESCALER;
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS00);

	while (1);
}