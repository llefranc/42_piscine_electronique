/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:08:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/19 17:00:22 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>

#include "uart.h"
#include "i2c.h"

int main(void)
{
	uart_init(UART_UBRRN, (0 << RXCIE0));

	/* Wait 40ms after power-on for allowing sensor to initialize */
	_delay_ms(40);

	i2c_init();
	if (!i2c_is_aht20_calibrated())
		uart_printstr("Error: aht20 is not calibrated\r\n");

	while (1) {
		/* Triggering sensor measurement */
		i2c_start();
		i2c_write(0xAC);
		i2c_write(0x33);
		i2c_write(0x00);
		i2c_stop();

		/* Wait 80ms to give time to sensor for measurement (cf datasheet) */
		_delay_ms(80);

		/* Reading sensor */
		i2c_read();
		i2c_stop();

		/* Measurement should be done every sec (cf datasheet) */
		_delay_ms(1000);
	}
}