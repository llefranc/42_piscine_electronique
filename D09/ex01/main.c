/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:52:35 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 10:11:41 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"
#include "i2c_pca.h"

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

void update_leds(uint8_t nb)
{
	i2c_pca_write_reg(I2C_PCA_O0, (nb & 0x07) << 1);
}

int main(void)
{
	uint8_t data;
	uint8_t nb = 0;
	uint8_t is_pressed = 0;

	i2c_init();
	i2c_pca_write_reg(I2C_PCA_C0, (1 << I2C_PCA0_D9) | (1 << I2C_PCA0_D10) |
	                  (1 << I2C_PCA0_D11));
	i2c_pca_write_reg(I2C_PCA_O0, 0);

	while (1) {
		data = i2c_pca_read_reg(I2C_PCA_I0);
		if (!is_pressed && (~data & (1 << I2C_PCA0_SW5))) {
			update_leds(++nb);
			is_pressed = 1;
			_delay_ms(20);
		}
		if (is_pressed && (!(~data & (1 << I2C_PCA0_SW5)))) {
			is_pressed = 0;
			_delay_ms(20);
		}
	}
}