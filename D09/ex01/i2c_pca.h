/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_pca.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:51:58 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 20:58:11 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I2C_PCA_H
#define I2C_PCA_H

#include "i2c.h"
#include <avr/io.h>

#define I2C_SLA_PCA 0b0100000

/* Command byte */
#define I2C_PCA_I0 0
#define I2C_PCA_I1 1
#define I2C_PCA_O0 2
#define I2C_PCA_O1 3
#define I2C_PCA_N0 4
#define I2C_PCA_N1 5
#define I2C_PCA_C0 6
#define I2C_PCA_C1 7

/* IO pins mapping for register 0 */
#define I2C_PCA0_SW5 0
#define I2C_PCA0_D11 1
#define I2C_PCA0_D10 2
#define I2C_PCA0_D9 3
#define I2C_PCA0_DIG1 4
#define I2C_PCA0_DIG2 5
#define I2C_PCA0_DIG3 6
#define I2C_PCA0_DIG4 7

/* IO pins mapping for register 1 */
#define I2C_PCA1_A 0
#define I2C_PCA1_B 1
#define I2C_PCA1_C 2
#define I2C_PCA1_D 3
#define I2C_PCA1_E 4
#define I2C_PCA1_F 5
#define I2C_PCA1_G 6
#define I2C_PCA1_DPX 7

void i2c_pca_write_reg(uint8_t reg, uint8_t data);
uint8_t i2c_pca_read_reg(uint8_t reg);

#endif /* I2C_PCA_H */