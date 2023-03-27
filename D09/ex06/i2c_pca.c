/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_pca.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:14:21 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 14:08:08 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c_pca.h"

#include <util/twi.h>

#define I2C_PCA_MAX_DIG 4
#define I2C_PCA_MAX_DIGIT_DRAWABLE 9

/**
 * Write to PCA expander's register a byte of data. Reverse for more simplicity
 * the bits in the byte of data using binary not operator because PCA registers
 * work the opposite way of AVR (0 in PCA output register => pin will be HIGH).
*/
void i2c_pca_write_reg(uint8_t reg, uint8_t data)
{
	i2c_start();
	i2c_sla_wr(I2C_PCA_SLA, TW_WRITE);
	i2c_write(reg);
	i2c_write(~data);
	i2c_stop();
}

/**
 * Read from PCA expander's register a single byte of data.
*/
uint8_t i2c_pca_read_reg(uint8_t reg)
{
	uint8_t data;

	i2c_start();
	i2c_sla_wr(I2C_PCA_SLA, TW_WRITE);
	i2c_write(reg);
	i2c_start();
	i2c_sla_wr(I2C_PCA_SLA, TW_READ);
	data = i2c_read(0); /* NACK */
	i2c_stop();
	return data;
}

static inline uint8_t i2c_pca_get_seg_0(void)
{
	/* Not gate for the segment so we need to use binary not operator */
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_B) | (1 << I2C_PCA1_C) |
	         (1 << I2C_PCA1_D) | (1 << I2C_PCA1_E) | (1 << I2C_PCA1_F));
}

static inline uint8_t i2c_pca_get_seg_1(void)
{
	return ~((1 << I2C_PCA1_B) | (1 << I2C_PCA1_C));
}

static inline uint8_t i2c_pca_get_seg_2(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_B) | (1 << I2C_PCA1_D) |
		 (1 << I2C_PCA1_E) | (1 << I2C_PCA1_G));
}

static inline uint8_t i2c_pca_get_seg_3(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_B) | (1 << I2C_PCA1_C) |
		 (1 << I2C_PCA1_D) | (1 << I2C_PCA1_G));
}

static inline uint8_t i2c_pca_get_seg_4(void)
{
	return ~((1 << I2C_PCA1_B) | (1 << I2C_PCA1_C) | (1 << I2C_PCA1_G) |
		 (1 << I2C_PCA1_F));
}

static inline uint8_t i2c_pca_get_seg_5(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_C) | (1 << I2C_PCA1_D) |
		 (1 << I2C_PCA1_F) | (1 << I2C_PCA1_G));
}

static inline uint8_t i2c_pca_get_seg_6(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_C) | (1 << I2C_PCA1_D) |
		 (1 << I2C_PCA1_E) | (1 << I2C_PCA1_F) | (1 << I2C_PCA1_G));
}

static inline uint8_t i2c_pca_get_seg_7(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_B) | (1 << I2C_PCA1_C));
}

static inline uint8_t i2c_pca_get_seg_8(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_B) | (1 << I2C_PCA1_C) |
	         (1 << I2C_PCA1_D) | (1 << I2C_PCA1_E) | (1 << I2C_PCA1_F) |
		 (1 << I2C_PCA1_G));
}

static inline uint8_t i2c_pca_get_seg_9(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_B) | (1 << I2C_PCA1_C) |
	         (1 << I2C_PCA1_D) | (1 << I2C_PCA1_F) | (1 << I2C_PCA1_G));
}

/**
 * Draw a digit on the LED segment display.
 *
 * @pcaO0_bits_save: correspond to the first 4 bits of PCA output 0 register,
 * 		     which are not used for the segments.
*/
int8_t i2c_pca_draw_seg_dig(uint8_t nb, uint8_t dig,  uint8_t dx,
			    uint8_t pcaO0_bits_save)
{
	static uint8_t (*get_seg[])(void) = {
		&i2c_pca_get_seg_0,
		&i2c_pca_get_seg_1,
		&i2c_pca_get_seg_2,
		&i2c_pca_get_seg_3,
		&i2c_pca_get_seg_4,
		&i2c_pca_get_seg_5,
		&i2c_pca_get_seg_6,
		&i2c_pca_get_seg_7,
		&i2c_pca_get_seg_8,
		&i2c_pca_get_seg_9
	};
	uint8_t segs;

	if (dig < (1 << I2C_PCA0_DIG1) || nb > I2C_PCA_MAX_DIGIT_DRAWABLE)
		return -1;
	i2c_pca_write_reg(I2C_PCA_O1, 0xFF);
	i2c_pca_write_reg(I2C_PCA_O0, pcaO0_bits_save | dig);
	segs = get_seg[nb]();
	if (dx)
		segs &= ~(1 << I2C_PCA1_DPX);
	i2c_pca_write_reg(I2C_PCA_O1, segs);
	return 0;
}

/**
 * Draw a number with several digits on the LED segment display.
 *
 * @pcaO0_bits_save: correspond to the first 4 bits of PCA output 0 register,
 * 		     which are not used for the segments.
*/
int8_t i2c_pca_draw_seg_nb(uint16_t nb, uint8_t *dx, uint8_t pcaO0_bits_save)
{
	static uint8_t dig[4] = {
		(1 << I2C_PCA0_DIG4),
		(1 << I2C_PCA0_DIG3),
		(1 << I2C_PCA0_DIG2),
		(1 << I2C_PCA0_DIG1)
	};
	static uint8_t print_nb_dig = 0;
	uint16_t divider = 1;

	if (nb > I2C_PCA_MAX_VALUE_DRAWABLE)
		return -1;
	for (uint8_t i = 0; i < print_nb_dig; ++i)
		divider *= 10;
	if (print_nb_dig > 3 || nb / divider == 0) {
		print_nb_dig = 0;
		divider = 1;
	}
	nb /= divider;
	i2c_pca_draw_seg_dig(nb % 10, dig[print_nb_dig], dx[print_nb_dig],
			     pcaO0_bits_save);
	++print_nb_dig;
	return 0;
}