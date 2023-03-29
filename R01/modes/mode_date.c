/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_date.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:29:09 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/29 17:59:31 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_date.h"

#include "../utils/uart.h"
#include "../utils/avr_string.h"
#include "../utils/timer.h"
#include "../utils/i2c_pca.h"

#define START_INDEX_DAY 0
#define START_INDEX_MONTH 3
#define START_INDEX_YEAR 6
#define START_INDEX_HOUR 11
#define START_INDEX_MIN 14
#define START_INDEX_SEC 17

#define END_INDEX_DAY_MONTH 6
#define END_INDEX_YEAR 10
#define END_INDEX_TIME 19

#define DAY_LEN 2
#define MONTH_LEN 2
#define YEAR_LEN 4
#define HOUR_LEN 2
#define MIN_LEN 2
#define SEC_LEN 2

struct date {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
};

static const uint8_t digs[4] = {
	(1 << I2C_PCA0_DIG1),
	(1 << I2C_PCA0_DIG2),
	(1 << I2C_PCA0_DIG3),
	(1 << I2C_PCA0_DIG4)
};

/* Number of day in each month, starting from January to December */
static const uint8_t nb_days_in_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31,
					      30, 31, 30, 31 };

/*
 * Temporary copy used after a read of date registers of real time clock
 * through i2c.
 */
static struct date actual_date = {
	.year = 2023,
	.month = 11,
	.day = 16,
	.hour = 18,
	.min = 33,
	.sec = 5
};

/**
 * Parse day and month of date entry : "dd/mm/".
*/
static inline int8_t check_day_month(char *buf, uint8_t *i)
{
	for (; *i < END_INDEX_DAY_MONTH; ++(*i)) {
		if (*i % 3 == 2 && buf[*i] != '/')
			return -1;
		else if (*i % 3 != 2 &&!avr_isnum(buf[*i]))
			return -1;
	}
	return 0;
}

/**
 * Parse year of date entry, including space after year : "yyyy ".
*/
static inline int8_t check_year(char *buf, uint8_t *i)
{
	for (; *i < END_INDEX_YEAR; ++(*i)) {
		if (!avr_isnum(buf[*i]))
			return -1;
	}
	if (buf[(*i)++] != ' ')
		return -1;
	return 0;
}

/**
 * Parse time of date entry : "hh:mm:ss" (ignore the spaces after time).
*/
static inline int8_t check_time(char *buf, uint8_t *i)
{
	for (; *i < END_INDEX_TIME; ++(*i)) {
		if (*i % 3 == 1 && buf[*i] != ':')
			return -1;
		else if (*i % 3 != 1 && !avr_isnum(buf[*i]))
			return -1;
	}
	while (buf[*i]) {
		if (buf[(*i)++] != ' ')
			return -1;
	}
	return 0;
}

/**
 * Check if the new date is a correct and valid date.
*/
static inline int8_t is_correct_date(struct date *tmp)
{
	if (!(tmp->month >= 1 && tmp->month <= 12) || !(tmp->hour <= 23) ||
	    !(tmp->min <= 59) || !(tmp->sec <= 59))
		return -1;

	/* Special case bisextile year */
	if (tmp->year % 4 == 0 && tmp->month == 2 && tmp->day > 0 &&
	    tmp->day <= 29)
		return 0;

	if (tmp->day == 0 || nb_days_in_month[tmp->month - 1] < tmp->day)
		return -1;
	return 0;
}

/**
 * Allow to update the date set in the real time clock through i2c bus.
*/
int8_t mode_date_update(char *buf)
{
	struct date tmp = {};
	uint8_t i = 0;

	if (check_day_month(buf, &i) == -1 || check_year(buf, &i) == -1 ||
	    check_time(buf, &i) == -1)
		return -1;

	tmp.day = avr_atoui(&buf[START_INDEX_DAY], DAY_LEN);
	tmp.month = avr_atoui(&buf[START_INDEX_MONTH], MONTH_LEN);
	tmp.year = avr_atoui(&buf[START_INDEX_YEAR], YEAR_LEN);
	tmp.hour = avr_atoui(&buf[START_INDEX_HOUR], HOUR_LEN);
	tmp.min = avr_atoui(&buf[START_INDEX_MIN], MIN_LEN);
	tmp.sec = avr_atoui(&buf[START_INDEX_SEC], SEC_LEN);

	if (is_correct_date(&tmp) == -1)
		return -1;
	actual_date = tmp;

	// add here i2c comm to set timer

	return 0;
}

void mode_x_date_xxx_init(void)
{
	UART_DEBUG("mode_x_date_xxx_init\r\n");
	OCR1A = F_CPU / TIMER_PRESCALER_1024; /* Every 1sec */
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 = (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 400; /* Every 2.5ms */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);
}

void mode_x_date_xxx_timer1(void)
{
	// request here i2c real time clock data
}

void mode_9_date_time_timer0(void)
{
	static uint8_t dx[4] = { 0, 1, 0, 1 };
	static uint8_t i = 0;

	if (i == 0)
		i2c_pca_draw_seg_dig(actual_date.hour / 10, digs[i], dx[i]);
	else if (i == 1)
		i2c_pca_draw_seg_dig(actual_date.hour % 10, digs[i], dx[i]);
	else if (i == 2)
		i2c_pca_draw_seg_dig(actual_date.min / 10, digs[i], dx[i]);
	else
		i2c_pca_draw_seg_dig(actual_date.min % 10, digs[i], dx[i]);
	if (++i >= I2C_PCA_MAX_DIG)
		i = 0;
}

void mode_x_date_xxx_clear(void)
{
	UART_DEBUG("mode_x_date_xxx_clear\r\n");
	timer0_clear();
	timer1_clear();
	i2c_pca_reset_seg();
}