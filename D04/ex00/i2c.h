/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 18:28:42 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/18 20:21:47 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I2C_H
#define I2C_H

#define I2C_TWBR_VAL 18
#define I2C_PRESCALER 4
#define I2C_F_SCL = F_CPU/(16+2*TWBR_VAL*I2C_PRESCALER)

#define I2C_P_ATH20_ADDR 0x38

#include <avr/io.h>
#include <util/twi.h>

static inline void i2c_init(void)
{
	/* Bit rate of 18 and prescaler of 4 gives a F_SCL of 100khz */
	TWSR |= (1 << TWPS0);
	TWBR = I2C_TWBR_VAL;
	/* Enabling TWI and interrupts for each TWI bus event */
	TWCR = (1 << TWEN) | (1 << TWIE); 
}

static inline void i2c_start(void)
{
	/* Writing SLA+W */
	TWDR = (I2C_P_ATH20_ADDR << 1) | TW_WRITE;
	/* Sending START condition */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

static inline void i2c_stop(void)
{
	/* Sending STOP condition */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE);
}

#endif /* I2C_H */