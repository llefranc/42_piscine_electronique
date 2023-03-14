/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 09:56:17 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/14 12:08:30 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

void update_leds(int8_t nb)
{
	PORTB &= ~(1 << PB4) & (0xFF << 3); /* resetting PB0/1/2/4 */
	PORTB |= nb & ~(0xFF << 3);         /* setting PB0/1/2 */
	PORTB |= (nb & (1 << 3)) << 1;      /* setting PB4 */
}

uint8_t update_nb(int8_t *nb, int8_t op, uint8_t is_pressed, uint16_t pin_x,
                  uint8_t pin_n)
{
	if (!is_pressed && !(pin_x & (1 << pin_n))) {
		is_pressed = 1;
		_delay_ms(20);
		*nb += op;           /* adding +1 or -1 */
	} else if (is_pressed && (pin_x & (1 << pin_n))) {
		is_pressed = 0;
		_delay_ms(20);
	}
	return is_pressed;
}

int main(void)
{
	/*
	 * LED D1 -> PBO, LED D2 -> PB1, LED D3 -> PB2, LED D4 -> PB4
	 * SW1 -> PD2, SW2 -> PD4
	 */

	uint8_t sw1_is_pressed = 0;
	uint8_t sw2_is_pressed = 0;
	int8_t nb = 0;

	DDRB |= (1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0);
	while (1) {

		sw1_is_pressed = update_nb(&nb, 1, sw1_is_pressed, PIND, PD2);
		sw2_is_pressed = update_nb(&nb, -1, sw2_is_pressed, PIND, PD4);

		if (nb > 0b1111)
			nb = 0;
		else if (nb < 0)
			nb = 0b1111;

		update_leds(nb);
	}
}