/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_aht.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:27:23 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/29 13:27:24 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_aht.h"

#include "../utils/uart.h"
#include "../utils/i2c_aht.h"
#include "../utils/i2c_pca.h"
#include "../utils/timer.h"

static uint8_t wait_first_measurement;
static uint16_t aht_val;
static const uint8_t digs[4] = {
	(1 << I2C_PCA0_DIG4),
	(1 << I2C_PCA0_DIG3),
	(1 << I2C_PCA0_DIG2),
	(1 << I2C_PCA0_DIG1)
};

void mode_x_aht_xxx_init(void)
{
	UART_DEBUG("mode_x_aht_xxx_init\r\n");
	OCR1A = F_CPU / TIMER_PRESCALER_1024; /* Every 1sec */
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 = (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 400; /* Every 2.5ms */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);
	i2c_aht_start_measurement();
	wait_first_measurement = 1;
}

void mode_x_aht_xxx_exec_timer0(char unit)
{
	static uint8_t i = 0;

	++i;
	if (wait_first_measurement) {
		if (i >= I2C_PCA_MAX_DIG)
			i = 0;
		i2c_pca_draw_seg_line(digs[i]);
	} else {
		if (i >= I2C_PCA_MAX_DIG - 1)
			i = 0;
		if (i == 0)
			i2c_pca_draw_seg_char(unit, digs[i], 0);
		else if (i == 1)
			i2c_pca_draw_seg_dig(aht_val % 10, digs[i], 1);
		else
			i2c_pca_draw_seg_dig(aht_val / 10, digs[i], 0);
	}
}

void mode_6_aht_temp_c_exec_timer1(void)
{
	i2c_aht_read_measurement();
	wait_first_measurement = 0;
	aht_val = (uint16_t)(i2c_aht_calc_temp());
	i2c_aht_start_measurement();
}

void mode_7_aht_temp_f_exec_timer1(void)
{
	i2c_aht_read_measurement();
	wait_first_measurement = 0;
	aht_val = (uint16_t)(i2c_aht_calc_temp() * 1.8 + 32.0); /* Fahrenheit */
	i2c_aht_start_measurement();
}

void mode_8_aht_humi_exec_timer1(void)
{
	i2c_aht_read_measurement();
	wait_first_measurement = 0;
	aht_val = (uint16_t)(i2c_aht_calc_humi());
	i2c_aht_start_measurement();
}

void mode_x_aht_xxx_clear(void)
{
	UART_DEBUG("mode_x_aht_xxx_clear\r\n");
	timer0_clear();
	timer1_clear();
	i2c_pca_reset_seg();
}