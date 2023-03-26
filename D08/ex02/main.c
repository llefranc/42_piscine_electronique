/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 16:48:55 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 256

#define NB_RGB_LEDS 3

ISR(TIMER1_COMPA_vect)
{
	static uint8_t i = 1;

	if (i == 0)
		spi_set_leds(B_LOW | RED, LED_OFF, LED_OFF);
	else if (i == 1)
		spi_set_leds(LED_OFF, B_LOW | RED, LED_OFF);
	else if (i == 2)
		spi_set_leds(LED_OFF, LED_OFF, B_LOW | RED);
	else
		spi_set_leds(LED_OFF, LED_OFF, LED_OFF);
	if (++i >= NB_RGB_LEDS + 1)
		i = 0;
}

int main(void)
{
	sei();
	spi_init();
	spi_set_leds(B_LOW | RED, LED_OFF, LED_OFF);

	OCR1A = F_CPU / TIMER1_PRESCALER / 4;
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12);

	while (1);
}