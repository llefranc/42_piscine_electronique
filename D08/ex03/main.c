/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 17:06:19 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 256

enum {
	E_THIRD_1 = 1,
	E_THIRD_2,
	E_THIRD_3,
};

ISR(TIMER1_COMPA_vect)
{
	static uint8_t prev_val = 0;
	uint8_t pot_val = adc_read() / (0xFF / 3);

	/* Avoiding to uselessly update LEDs */
	if (prev_val != pot_val) {
		switch (pot_val) {
		case E_THIRD_1:
			spi_set_leds(B_LOW | RED, LED_OFF, LED_OFF);
			break;
		case E_THIRD_2:
			spi_set_leds(B_LOW | RED, B_LOW | RED, LED_OFF);
			break;
		case E_THIRD_3:
			spi_set_leds(B_LOW | RED, B_LOW | RED, B_LOW | RED);
			break;
		default:
			spi_set_leds(LED_OFF, LED_OFF, LED_OFF);
		}
	}
	prev_val = pot_val;
}

int main(void)
{
	sei();
	spi_init();
	adc_init();
	adc_mux_select(ADC_POT);

	/* Timer 1 match every 20 ms */
	OCR1A = F_CPU / TIMER1_PRESCALER / 100 * 2;
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12);

	while (1);
}