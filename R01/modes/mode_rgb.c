/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_rgb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:26:08 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/30 11:22:23 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_rgb.h"

#include "../utils/uart.h"
#include "../utils/timer.h"
#include "../utils/spi.h"
#include "../utils/i2c_pca.h"
#include "../utils/io.h"

#define NB_PRIMARY_COLOR 3

static uint8_t col_index = 1;

static const uint32_t rgb_pins[3] = { LED5_R, LED5_G, LED5_B };
static const uint32_t spi_col[3] = { RED, GREEN, BLUE };
static const uint8_t digs[4] = {
	(1 << I2C_PCA0_DIG1),
	(1 << I2C_PCA0_DIG2),
	(1 << I2C_PCA0_DIG3),
	(1 << I2C_PCA0_DIG4)
};

void mode_5_rgb_init(void)
{
	UART_DEBUG("mode_5_rgb_init\r\n");
	spi_init();

	TIMSK0 = (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 400; /* Every 2.5ms */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);

	OCR1A = F_CPU / TIMER_PRESCALER_1024; /* Every 1 sec */
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	spi_set_leds(B_MED | RED, B_MED | RED, B_MED | RED);
	PORTD |= (1 << LED5_R);
	col_index = 1;
}

void mode_5_rgb_exec_timer0(void)
{
	static uint8_t i = 0;

	if (i == 0 || i == 3)
		i2c_pca_draw_seg_line(digs[i]);
	else if (i == 1)
		i2c_pca_draw_seg_dig(4, digs[i], 0);
	else
		i2c_pca_draw_seg_dig(2, digs[i], 0);
	if (++i >= I2C_PCA_MAX_DIG)
		i = 0;
}

void mode_5_rgb_exec_timer1(void)
{
	PORTD &= ~((1 << LED5_R) | (1 << LED5_G) | (1 << LED5_B));
	PORTD |= (1 << rgb_pins[col_index]);
	spi_set_leds(B_MED | spi_col[col_index], B_MED | spi_col[col_index],
		     B_MED | spi_col[col_index]);
	if (++col_index >= NB_PRIMARY_COLOR)
		col_index = 0;
}

void mode_5_rgb_clear(void)
{
	UART_DEBUG("mode_5_rgb_clear\r\n");
	timer0_clear();
	timer1_clear();
	spi_set_leds(0, 0, 0);
	PORTD &= ~((1 << LED5_R) | (1 << LED5_G) | (1 << LED5_B));
	spi_clear();
	i2c_pca_reset_seg();
}