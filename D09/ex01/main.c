/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:52:35 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 21:33:39 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"
#include "i2c_pca.h"
#include "uart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

void update_leds(uint8_t nb)
{
	i2c_pca_write_reg
}

int main(void)
{
	uint8_t nb;
	uint8_t data;
	uint8_t is_pressed = 0;

	uart_init(UART_UBRRN, 0);
	i2c_init();
	while (1) {
		data = i2c_pca_read_reg(I2C_PCA_I0);
		if (!is_pressed && (~data & (1 << I2C_PCA_IO0_SW5))) {
			if (++nb == 8)
				nb = 0;
			update_leds(nb);
			is_pressed = 1;
			_delay_ms(20);
		}
		if (is_pressed && (~data && (1 << I2C_PCA_IO0_SW5))) {
			is_pressed = 0;
			_delay_ms(20);
		}
		// uart_print_hex_value(data);
		// uart_printstr("\r\n");

		// _delay_ms(1000);
	}
}