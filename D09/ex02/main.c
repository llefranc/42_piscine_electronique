/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:52:35 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 11:05:49 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c.h"
#include "i2c_pca.h"

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

int main(void)
{
	i2c_init();

	/* All segments pins in output */
	i2c_pca_write_reg(I2C_PCA_C0, 0xF0);
	i2c_pca_write_reg(I2C_PCA_C1, 0xFF);

	i2c_pca_draw_seg_nb(2, (1 << I2C_PCA0_DIG4), 1, 0);

	while (1);
}