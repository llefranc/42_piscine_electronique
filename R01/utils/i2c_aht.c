/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_aht.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 10:23:59 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/30 11:29:55 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c_aht.h"

#include "i2c.h"

#include <util/twi.h>

static uint8_t aht20_val[5];

/**
 * Transmit measurement command to AHT20 through i2c bus. Should wait 80ms
 * before reading the measurement value (cf AHT20 datasheet).
*/
void i2c_aht_start_measurement(void)
{
	i2c_start();
	i2c_sla_wr(I2C_ATH_SLA, TW_WRITE);
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);
	i2c_stop();
}

/**
 * Receive the AHT20 measurement through i2c bus. This function should be call
 * at least 80ms after the measurement command has been transmitted (cf AHT20
 * datasheet).
*/
void i2c_aht_read_measurement(void)
{
	i2c_start();
	i2c_sla_wr(I2C_ATH_SLA, TW_READ);
	i2c_read((1 << TWEA));                      /* Status byte + ACK*/
	for (uint8_t i = 0; i < 5; ++i)
		aht20_val[i] = i2c_read(1 << TWEA); /* 5 data bytes + ACK */
	i2c_read(0);				    /* CRC byte + NACK */
	i2c_stop();
}

/**
 * Calculate the temperature in celcius based on last measurement received from
 * AHT20.
*/
float i2c_aht_calc_temp(void)
{
	float temp;
	uint32_t st = 0;

	st |= ((uint32_t)(aht20_val[2] & 0x0F)) << 16;
	st |= ((uint32_t)aht20_val[3]) << 8;
	st |= (uint32_t)aht20_val[4];
	temp = (float)st / (float)(1L << 20);
	temp *= 200.0;
	temp -= 50.0;
	return temp;
}

/**
 * Calculate the humidity in percent based on last AHT20 received measurement.
*/
float i2c_aht_calc_humi(void)
{
	float humi;
	uint32_t srh = 0;

	srh |= ((uint32_t)(aht20_val[0])) << 12;
	srh |= ((uint32_t)aht20_val[1]) << 4;
	srh |= ((uint32_t)(aht20_val[2] & 0xF0)) >> 4;
	humi = (float)srh / (float)(1L << 20);
	humi *= 100.0;
	return humi;
}