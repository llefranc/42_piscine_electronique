/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:52:35 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 21:25:27 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"
#include "i2c_pca.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect)
{
	static uint8_t led_d9 = (1 << I2C_PCA_IO0_D9);

	i2c_pca_write(I2C_PCA_O0, led_d9);
	led_d9 ^= (1 << I2C_PCA_IO0_D9);
}

int main(void)
{
	sei();
	i2c_init();
	uart_init(UART_UBRRN, 0);
	i2c_pca_write(I2C_PCA_C0, (1 << I2C_PCA_IO0_D9));
	while (1);
}