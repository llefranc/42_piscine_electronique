/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:52:35 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 13:56:48 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"
#include "i2c_pca.h"
#include "timer.h"

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define TIMER0_PRESCALER 1024

static uint16_t nb = 0;

ISR(TIMER1_COMPA_vect)
{
	if (++nb > I2C_PCA_MAX_VALUE_DRAWABLE)
		nb = 0;
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t dx[4] = { 0, 0, 0, 0 };

	i2c_pca_draw_seg_nb(nb, dx, 0);
}

static inline void start_timer0_5ms(void)
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

	/* All segments pins in output */
	i2c_pca_write_reg(I2C_PCA_C0, 0xF0);
	i2c_pca_write_reg(I2C_PCA_C1, 0xFF);

	start_timer1_ms(1000);
	start_timer0_5ms();
	while (1);
}