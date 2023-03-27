/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 14:54:08 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "adc.h"
#include "timer.h"
#include "i2c.h"
#include "i2c_pca.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER0_PRESCALER 1024

static uint16_t pot_val;

ISR(TIMER1_COMPA_vect)
{
	static uint16_t tmp;

	tmp = adc_read_10bit();

	/*
	 * To neutralize the fact that potentiometer values changes a little
	 * bit even if the potentiometer isn't moving
	 */
	if ((tmp > pot_val && tmp - pot_val > 2) ||
	    (tmp < pot_val && pot_val - tmp > 2))
		pot_val = tmp;
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t dx[4] = { 0, 0, 0, 0 };

	i2c_pca_draw_seg_nb(pot_val, dx, 4, 0);
}

static inline void timer0_5ms_start(void)
{
	OCR0A = F_CPU / TIMER0_PRESCALER / 200;
	TCCR0A = (1 << WGM01);
	TIMSK0 = (1 << OCIE0A);
	TCCR0B = (1 << CS02) | (1 << CS00);
}

int main(void)
{
	sei();
	i2c_init();
	adc_init((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) ,0);
	adc_mux_select(ADC_POT);
	pot_val = adc_read_10bit();

	/* All segments pins in output */
	i2c_pca_write_reg(I2C_PCA_C0, 0xF0);
	i2c_pca_write_reg(I2C_PCA_C1, 0xFF);
	timer1_ms_start(20);
	timer0_5ms_start();

	while (1);
}