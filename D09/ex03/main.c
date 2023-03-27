/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:52:35 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 14:13:57 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"
#include "i2c_pca.h"
#include "timer.h"

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect)
{
	static uint8_t nb = 0;

	i2c_pca_draw_seg_nb(nb++, (1 << I2C_PCA0_DIG4), 0, 0);
	if (nb >= 10)
		nb = 0;
}

int main(void)
{
	sei();
	i2c_init();

	/* All segments pins in output */
	i2c_pca_write_reg(I2C_PCA_C0, 0xF0);
	i2c_pca_write_reg(I2C_PCA_C1, 0xFF);

	timer1_ms_start(1000);
	while (1);
}