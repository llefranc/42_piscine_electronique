/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 13:51:18 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/25 17:42:47 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"

#include "uart.h"

#include <util/delay.h>
#include <util/twi.h>

void print_hex_value(uint8_t c)
{
	static const char *hex = "0123456789ABCDEF";

	uart_tx(hex[(c >> 4)]);
	uart_tx(hex[(c & 0x0F)]);
}

static inline void i2c_m_wait_for_op_to_proceed(void)
{
	/* Wait until TWINT flag is set */
	while (!(TWCR & (1 << TWINT)));
}

void i2c_start(void)
{
	/* Sending START condition */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();

	/* Writing SLA+W */
	TWDR = (I2C_P_ATH20_ADDR << 1) | TW_WRITE;
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();
}

uint8_t i2c_is_aht20_calibrated(void)
{
	uint8_t is_calibrated;
	i2c_start();
	i2c_write(0x71); /* command to ask status byte */

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); /* start */
	i2c_m_wait_for_op_to_proceed();

	TWDR = (I2C_P_ATH20_ADDR << 1) | TW_READ; /* sla+r */
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();

	TWCR = (1 << TWINT) | (1 << TWEN); /* recv byte + NACK */
	i2c_m_wait_for_op_to_proceed();
	is_calibrated = TWDR & (1 << 3);

	i2c_stop();
	return is_calibrated;
}

void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); /* send data */
	i2c_m_wait_for_op_to_proceed();
}

void i2c_read(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); /* start */
	i2c_m_wait_for_op_to_proceed();

	TWDR = (I2C_P_ATH20_ADDR << 1) | TW_READ; /* sla+r */
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();

	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); /* recv byte + ACK */
	i2c_m_wait_for_op_to_proceed();
	print_hex_value(TWDR); /* printing status byte */
	uart_tx(' ');

	/* 5 times recv byte + ACK and print */
	for (uint8_t i = 0; i < 5; ++i) {
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
		i2c_m_wait_for_op_to_proceed();
		print_hex_value(TWDR);
		uart_tx(' ');
	}

	/* recv last byte + NACK and print */
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();
	print_hex_value(TWDR);
	uart_printstr("\r\n");
}