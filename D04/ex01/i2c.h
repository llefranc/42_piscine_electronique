/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 18:28:42 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 14:24:49 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I2C_H
#define I2C_H

#define I2C_TWBR_VAL 18
#define I2C_PRESCALER 4
#define I2C_F_SCL = F_CPU/(16+2*TWBR_VAL*I2C_PRESCALER)

#define I2C_SLA_ATH20 0x38

#include <avr/io.h>

void i2c_start(void);
uint8_t i2c_is_aht20_calibrated(void);
void i2c_read(void);
void i2c_write(uint8_t data);

static inline void i2c_init(void)
{
	/* Bit rate of 18 and prescaler of 4 gives a F_SCL of 100khz */
	TWSR |= (1 << TWPS0);
	TWBR = I2C_TWBR_VAL;
	/* Enabling TWI */
	TWCR = (1 << TWEN);
}

static inline void i2c_stop(void)
{
	/* Sending STOP condition */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

#endif /* I2C_H */