/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:52:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/14 13:44:57 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define LED_TIME_UP 200

static inline void set_bit(volatile uint8_t *reg, uint8_t n)
{
	*reg |= (1 << n);
}

static inline void unset_bit(volatile uint8_t *reg, uint8_t n)
{
	*reg &= ~(1 << n);
}

int main(void)
{
	uint16_t i;
	uint8_t leds[] = { PB0, PB1, PB2, PB4 };

	DDRB |= (1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0);
	set_bit(&PORTB, PB0);
	_delay_ms(LED_TIME_UP);

	while (1) {
		for (i = 1; i < sizeof(leds) / sizeof(*leds); ++i) {
			unset_bit(&PORTB, leds[i - 1]);
			set_bit(&PORTB, leds[i]);
			_delay_ms(LED_TIME_UP);
		}
		for (i = sizeof(leds) - 1; i > 0; --i) {
			unset_bit(&PORTB, leds[i]);
			set_bit(&PORTB, leds[i - 1]);
			_delay_ms(LED_TIME_UP);
		}
	}
}