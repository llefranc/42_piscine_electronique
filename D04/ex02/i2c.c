/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 13:51:18 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/19 21:24:57 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"

#include "uart.h"

#include <util/twi.h>
#include <stdlib.h>

void print_hex_value(uint8_t c)
{
	static const char *hex = "0123456789ABCDEF";
	char buf[3] = { hex[(c >> 4)], hex[(c & 0x0F)]};

	uart_printstr(buf);
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

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); /* repeated start */
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

uint32_t moy_last_3_s(uint32_t new_s, uint32_t *last_3_s,
				    uint8_t i, uint8_t nb_s)
{
	last_3_s[i] = new_s;
	return (last_3_s[0] + last_3_s[1] + last_3_s[2]) / (uint32_t)(nb_s);
}

static inline void print_temp(uint8_t *buf, uint8_t i, uint8_t nb_s)
{
	float temp;
	char temp_str[7] = {};
	uint32_t new_st = 0;
	static uint32_t last_3_st[3] = {};

	new_st |= ((uint32_t)(buf[2] & 0x0F)) << 16;
	new_st |= ((uint32_t)buf[3]) << 8;
	new_st |= (uint32_t)buf[4];
	temp = moy_last_3_s(new_st, last_3_st, i, nb_s);
	temp /= (float)(1L << 20);
	temp *= 200.0;
	temp -= 50.0;
	uart_printstr("Temperature: ");
	dtostrf(temp, 4, 2, temp_str);
	uart_printstr(temp_str);
}

static inline void print_humi(uint8_t *buf, uint8_t i, uint8_t nb_s)
{
	float humi;
	char humi_str[6] = {};
	uint32_t new_srh = 0;
	static uint32_t last_3_st[3] = {};

	new_srh |= (uint32_t)(buf[0]) << 12;
	new_srh |= ((uint32_t)buf[1]) << 4;
	new_srh |= (uint32_t)(buf[2] & 0xF0) >> 4;
	humi = moy_last_3_s(new_srh, last_3_st, i, nb_s);
	humi /= (float)(1L << 20);
	humi *= 100.0;
	uart_printstr(".C, Humidity: ");
	dtostrf(humi, 3, 1, humi_str);
	uart_printstr(humi_str);
	uart_printstr("%\r\n");
}

static inline void i2c_print_aht20(uint8_t *buf)
{
	static uint8_t i = 0;
	static uint8_t nb_s = 0;

	/* To handle moy calculation during first and second measurement */
	if (nb_s != 3)
		++nb_s;
	print_temp(buf, i, nb_s);
	print_humi(buf, i, nb_s);

	/*
	 * To go back to beginning of st/srh arrays to erase the oldest of 3
	 * last values
	 */
	if (++i == 3)
		i = 0;
}

void i2c_read(void)
{
	uint8_t buf[5];

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); /* start */
	i2c_m_wait_for_op_to_proceed();

	TWDR = (I2C_P_ATH20_ADDR << 1) | TW_READ; /* sla+r */
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();

	/* recv status byte + ACK */
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();

	/* 5 times recv measurement data byte + ACK */
	for (uint8_t i = 0; i < 5; ++i) {
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
		i2c_m_wait_for_op_to_proceed();
		buf[i] = TWDR;
	}

	/* recv last byte (CRC) + NACK */
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();
	i2c_print_aht20(buf);
}