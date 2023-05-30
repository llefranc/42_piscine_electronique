/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_pcf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 09:41:56 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/30 17:52:19 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c_pcf.h"

#include "i2c.h"

#include <util/twi.h>

static struct date actual_date = {};

#define I2C_PCF_REG_SEC_ADDR 2

#define MASK_SEC_TENS_PLACE 0x70
#define MASK_SEC_UNIT_PLACE 0x0F
#define MASK_MIN_TENS_PLACE 0x70
#define MASK_MIN_UNIT_PLACE 0x0F
#define MASK_HOUR_TENS_PLACE 0x30
#define MASK_HOUR_UNIT_PLACE 0x0F
#define MASK_DAY_TENS_PLACE 0x30
#define MASK_DAY_UNIT_PLACE 0x0F
#define MASK_MONTH_TENS_PLACE 0x10
#define MASK_MONTH_UNIT_PLACE 0x0F
#define MASK_YEAR_TENS_PLACE 0xF0
#define MASK_YEAR_UNIT_PLACE 0x0F

#define ACTUAL_MILLENIUM 2000

static inline uint8_t date_reg_to_sec(uint8_t sec_reg_data)
{
	return (((sec_reg_data & MASK_SEC_TENS_PLACE) >> 4) * 10) +
	       (sec_reg_data & MASK_SEC_UNIT_PLACE);
}

static inline uint8_t date_reg_to_min(uint8_t min_reg_data)
{
	return (((min_reg_data & MASK_MIN_TENS_PLACE) >> 4) * 10) +
	       (min_reg_data & MASK_MIN_UNIT_PLACE);
}

static inline uint8_t date_reg_to_hour(uint8_t hour_reg_data)
{
	return (((hour_reg_data & MASK_HOUR_TENS_PLACE) >> 4) * 10) +
	       (hour_reg_data & MASK_HOUR_UNIT_PLACE);
}

static inline uint8_t date_reg_to_day(uint8_t day_reg_data)
{
	return (((day_reg_data & MASK_DAY_TENS_PLACE) >> 4) * 10) +
	       (day_reg_data & MASK_DAY_UNIT_PLACE);
}

static inline uint8_t date_reg_to_month(uint8_t month_reg_data)
{
	return (((month_reg_data & MASK_MONTH_TENS_PLACE) >> 4) * 10) +
	       (month_reg_data & MASK_MONTH_UNIT_PLACE);
}

static inline uint16_t date_reg_to_year(uint8_t year_reg_data)
{
	return (((year_reg_data & MASK_YEAR_TENS_PLACE) >> 4) * 10) +
	       (year_reg_data & MASK_YEAR_UNIT_PLACE) + ACTUAL_MILLENIUM;
}

/**
 * Read through i2c the date from real time clock PCF8563T.
*/
void i2c_pcf_read_date(struct date *buf)
{
	i2c_start();
	i2c_sla_wr(I2C_PCF_SLA, TW_WRITE);
	i2c_write(I2C_PCF_REG_SEC_ADDR);
	i2c_start();
	i2c_sla_wr(I2C_PCF_SLA, TW_READ);
	actual_date.sec = date_reg_to_sec(i2c_read(1 << TWEA));
	actual_date.min = date_reg_to_min(i2c_read(1 << TWEA));
	actual_date.hour = date_reg_to_hour(i2c_read(1 << TWEA));
	actual_date.day = date_reg_to_day(i2c_read(1 << TWEA));
	i2c_read(1 << TWEA); /* Weekday value, we do not need it */
	actual_date.month = date_reg_to_month(i2c_read(1 << TWEA));
	actual_date.year = date_reg_to_year(i2c_read(0));
	i2c_stop();
	*buf = actual_date;
}

static inline uint8_t xxx_to_date_reg(uint8_t data)
{
	return ((data / 10) << 4) + data % 10;
}

/**
 * Write the registers of PCF8563T through i2c to update the date.
*/
void i2c_pcf_write_date(struct date *buf)
{
	i2c_start();
	i2c_sla_wr(I2C_PCF_SLA, TW_WRITE);
	i2c_write(I2C_PCF_REG_SEC_ADDR);
	i2c_write(xxx_to_date_reg(buf->sec));
	i2c_write(xxx_to_date_reg(buf->min));
	i2c_write(xxx_to_date_reg(buf->hour));
	i2c_write(xxx_to_date_reg(buf->day));
	i2c_write(0); /* Weekday value, we do not need it */
	i2c_write(xxx_to_date_reg(buf->month));
	i2c_write(xxx_to_date_reg((uint8_t)(buf->year - ACTUAL_MILLENIUM)));
	i2c_stop();
	actual_date = *buf;
}