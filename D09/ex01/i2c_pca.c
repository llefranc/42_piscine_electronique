/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_pca.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:14:21 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 21:33:50 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c_pca.h"

#include <util/twi.h>

/**
 * Write to PCA expander's register a byte of data. Reverse for more simplicity
 * the bits in the byte of data using binary not operator because PCA registers
 * work the opposite way of AVR (0 in PCA output register => pin will be HIGH).
*/
void i2c_pca_write_byte(uint8_t reg, uint8_t data)
{
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA, TW_WRITE);
	i2c_write(reg);
	i2c_write(~data);
	i2c_stop();
}

void i2c_pca_read_byte(uint8_t reg)
{
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA, TW_WRITE);
	i2c_write(reg);
	i2c_start();
	i2c_read(0); /* NACK */
	i2c_stop();
}
