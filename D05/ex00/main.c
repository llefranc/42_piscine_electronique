/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:54:11 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 20:39:34 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"

#include <avr/io.h>
#include <util/delay.h>

static inline void update_leds(int8_t nb)
{
	nb %= 16;
	PORTB &= ~(1 << PB4) & (0xFF << 3); /* resetting PB0/1/2/4 */
	PORTB |= nb & ~(0xFF << 3);         /* setting PB0/1/2 */
	PORTB |= (nb & (1 << 3)) << 1;      /* setting PB4 */
}

int main(void)
{
	uint8_t is_pressed = 0;
	static uint8_t nb;

	DDRB |= (1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0);

	/* Initializing EEPROM with our counter only for first run of program */
	if (!is_magic_ok(MAGIC_NUM_ADDR, MAGIC_NUM)) {
		eeprom_write(MAGIC_NUM_ADDR, MAGIC_BYTE);
		eeprom_write(MAGIC_NUM_ADDR + 1, MAGIC_BYTE);
		eeprom_write(MAGIC_NUM_ADDR + 2, MAGIC_BYTE);
		eeprom_write(MAGIC_NUM_ADDR + 3, MAGIC_BYTE);
		eeprom_write(MAGIC_NUM_ADDR + MAGIC_NUM_LEN, nb);
	} else {
		nb = eeprom_read(MAGIC_NUM_ADDR + MAGIC_NUM_LEN);
	}

	update_leds(nb);
	while (1) {
		if (!is_pressed && !(PIND & (1 << PD2))) {
			is_pressed = 1;
			_delay_ms(20);
			nb++;
			eeprom_write(MAGIC_NUM_ADDR + MAGIC_NUM_LEN, nb);
			update_leds(nb);
		} else if (is_pressed && (PIND & (1 << PD2))) {
			is_pressed = 0;
			_delay_ms(20);
		}
	}
}