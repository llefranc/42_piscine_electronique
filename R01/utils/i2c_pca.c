/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_pca.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:14:21 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/30 11:30:35 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i2c_pca.h"

#include "i2c.h"

#include <util/twi.h>

#define I2C_PCA_MAX_DIGIT_DRAWABLE 9

#define I2C_PCAO0_MASK_DIGS 0x0F

/*
 * Copy of PCA output0 register to avoid useless read when updating certain bits
 * of this register, in order to save the state of the other bits
 */
static uint8_t i2c_pca_regO0_data;

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
 * Write to PCA expander's register output0 one or several bits using a mask.
 * This function use a static buffer which is the copy of PCAO0 register to
 * avoid an useless read in order to save the other bits already set in PCAO0
 * before writing.
*/
void i2c_pca_write_regO0(uint8_t mask, uint8_t data)
{
	i2c_pca_regO0_data &= mask;
	i2c_pca_regO0_data |= data;
	i2c_pca_write_reg(I2C_PCA_O0, i2c_pca_regO0_data);
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

static inline uint8_t i2c_pca_get_seg_line(void)
{
	return ~(1 << I2C_PCA1_G);
}

static inline uint8_t i2c_pca_get_seg_c(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_D) | (1 << I2C_PCA1_E) |
	         (1 << I2C_PCA1_F));
}

static inline uint8_t i2c_pca_get_seg_f(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_E) | (1 << I2C_PCA1_F) |
		 (1 << I2C_PCA1_G));
}

static inline uint8_t i2c_pca_get_seg_p(void)
{
	return ~((1 << I2C_PCA1_A) | (1 << I2C_PCA1_B) | (1 << I2C_PCA1_E) |
	         (1 << I2C_PCA1_F) | (1 << I2C_PCA1_G));
}

/**
 * Draw a line on the LED segment display (ex: -);
*/
int8_t i2c_pca_draw_seg_line(uint8_t dig)
{
	if (dig < (1 << I2C_PCA0_DIG1))
		return -1;
	i2c_pca_write_reg(I2C_PCA_O1, 0xFF);
	i2c_pca_regO0_data &= I2C_PCAO0_MASK_DIGS;
	i2c_pca_regO0_data |= dig;
	i2c_pca_write_reg(I2C_PCA_O0, i2c_pca_regO0_data);
	i2c_pca_write_reg(I2C_PCA_O1, ~(1 << I2C_PCA1_G));
	return 0;
}

/**
 * Draw a character on the LED segment display, or draw a line if the get_seg_x
 * corresponding function is not implemented.
*/
int8_t i2c_pca_draw_seg_char(char c, uint8_t dig, uint8_t dx)
{
	static uint8_t (*get_seg_char[])(void) = {
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_c,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_f,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_p,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line,
		&i2c_pca_get_seg_line
	};
	uint8_t segs;

	if (dig < (1 << I2C_PCA0_DIG1))
		return -1;
	if (c >= 'a' && c <= 'z')
		c -= 'a';
	else if (c >= 'A' && c <= 'Z')
		c -= 'A';
	else
		return -1;
	i2c_pca_write_reg(I2C_PCA_O1, 0xFF);
	i2c_pca_regO0_data &= I2C_PCAO0_MASK_DIGS;
	i2c_pca_regO0_data |= dig;
	i2c_pca_write_reg(I2C_PCA_O0, i2c_pca_regO0_data);
	segs = get_seg_char[(uint8_t)c]();
	if (dx)
		segs &= ~(1 << I2C_PCA1_DPX);
	i2c_pca_write_reg(I2C_PCA_O1, segs);
	return 0;
}

/**
 * Draw a digit on the LED segment display.
*/
int8_t i2c_pca_draw_seg_dig(uint8_t nb, uint8_t dig, uint8_t dx)
{
	static uint8_t (*get_seg_dig[])(void) = {
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
	i2c_pca_regO0_data &= I2C_PCAO0_MASK_DIGS;
	i2c_pca_regO0_data |= dig;
	i2c_pca_write_reg(I2C_PCA_O0, i2c_pca_regO0_data);
	segs = get_seg_dig[nb]();
	if (dx)
		segs &= ~(1 << I2C_PCA1_DPX);
	i2c_pca_write_reg(I2C_PCA_O1, segs);
	return 0;
}

/**
 * Draw a number with several digits on the LED segment display.
 *
 * @width: The minimum width of the number displayed on segment screen. 0 and 1
 * 	   will both output a width of 1(ex: 0 with a width of 2 will produce
 * 	   '00').
*/
int8_t i2c_pca_draw_seg_nb(uint16_t nb, const uint8_t *dx, uint8_t width)
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
	if (print_nb_dig > 3 || (print_nb_dig >= width && nb / divider == 0)) {
		print_nb_dig = 0;
		divider = 1;
	}
	nb /= divider;
	i2c_pca_draw_seg_dig(nb % 10, dig[print_nb_dig], dx[print_nb_dig]);
	++print_nb_dig;
	return 0;
}

void i2c_pca_reset_seg(void)
{
	i2c_pca_regO0_data &= I2C_PCAO0_MASK_DIGS;
	i2c_pca_write_reg(I2C_PCA_O0, i2c_pca_regO0_data);
	i2c_pca_write_reg(I2C_PCA_O1, 0xFF);
}