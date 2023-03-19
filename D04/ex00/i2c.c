/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 13:51:18 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/19 14:04:36 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"

#include "uart.h"

void i2c_print_status(void)
{
	static const char *hex = "0123456789ABCDEF";
	char buf[6] = { [0] = '0', 'x', [4] = ' ' };
	char c;

	while (!(TWCR & (1 << TWINT)));
	c = TWSR & TW_STATUS_MASK;
	buf[2] = hex[c >> 4];
	buf[3] = hex[c & 0x0F];
	uart_printstr(buf);
}

void i2c_start(void)
{
	/* Sending START condition */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	i2c_print_status();

	/* Writing SLA+W */
	TWDR = (I2C_P_ATH20_ADDR << 1) | TW_WRITE;
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_print_status();
}