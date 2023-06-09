/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:12:27 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/30 11:28:24 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.h"

#include "mode_adc.h"
#include "mode_rgb.h"
#include "mode_aht.h"
#include "mode_date.h"

#include "../utils/uart.h"
#include "../utils/timer.h"
#include "../utils/i2c_pca.h"
#include "../utils/io.h"

#include <util/delay.h>

volatile int8_t g_mode = E_MODE_0_START_SEQ;

static void (*g_fptr_mode_init[LAST_MODE_NB + 1])(void) = {
	&mode_0_start_seq_init,
	&mode_x_adc_xxx_init,
	&mode_x_adc_xxx_init,
	&mode_x_adc_xxx_init,
	&mode_x_adc_xxx_init,
	&mode_5_rgb_init,
	&mode_x_aht_xxx_init,
	&mode_x_aht_xxx_init,
	&mode_x_aht_xxx_init,
	&mode_x_date_xxx_init,
	&mode_x_date_xxx_init,
	&mode_x_date_xxx_init
};

static void (*g_fptr_mode_clear[LAST_MODE_NB + 1])(void) = {
	&mode_0_start_seq_clear,
	&mode_x_adc_xxx_clear,
	&mode_x_adc_xxx_clear,
	&mode_x_adc_xxx_clear,
	&mode_x_adc_xxx_clear,
	&mode_5_rgb_clear,
	&mode_x_aht_xxx_clear,
	&mode_x_aht_xxx_clear,
	&mode_x_aht_xxx_clear,
	&mode_x_date_xxx_clear,
	&mode_x_date_xxx_clear,
	&mode_x_date_xxx_clear
};

/**
 * Switch mode by stopping the previous one and starting another and updating
 * the LEDs accordingly.
 * @dir: If negative/positive number, stop actual mode and start another mode by
 *       going backward/forward in the modes number. If dir is 0, just start the
 *       mode referenced by global variable g_mode (usefull for first mode to
 *       launch at system start).
*/
void switch_mode(int8_t dir)
{
	uint8_t tmp;

	if (dir != 0) {
		g_fptr_mode_clear[g_mode]();
		g_mode += dir;
		if (g_mode < FIRST_MODE_NB)
			g_mode = LAST_MODE_NB;
		else if (g_mode > LAST_MODE_NB)
			g_mode = FIRST_MODE_NB;
	}
	tmp = g_mode;
	PORTB &= ~((1 << LED4) | (1 << LED3) | (1 << LED2) | (1 << LED1));
	if (tmp & (1 << 3)) {
		tmp |= (1 << PB4);
		tmp &= ~(1 << 3);
	}
	PORTB |= tmp;
	g_fptr_mode_init[g_mode]();
}

/**
 * Light on all segments and LED D1,D2,D3 and D4 for 3 sec at start.
*/
void mode_0_start_seq_init(void)
{
	UART_DEBUG("mode_0_start_seq_init\r\n");
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU / TIMER_PRESCALER_1024 * 3; /* Every 3sec */
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 |= (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 200; /* Every 5ms*/
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);

	PORTB |= (1 << LED4) | (1 << LED3) | (1 << LED2) | (1 << LED1);
}

void mode_0_start_seq_exec_timer0(void)
{
	static uint8_t dx[4] = {};

	i2c_pca_draw_seg_nb(8888, dx, 4);
}

void mode_0_start_seq_clear(void)
{
	UART_DEBUG("mode_0_start_seq_clear\r\n");
	timer0_clear();
	timer1_clear();
	i2c_pca_reset_seg();
	PORTB &= ~((1 << LED4) | (1 << LED3) | (1 << LED2) |
			(1 << LED1));
	_delay_ms(1000);
}