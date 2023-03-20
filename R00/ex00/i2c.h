/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 21:30:10 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 15:00:40 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

/* Bit rate of 18 and prescaler of 4 gives a F_SCL of 100khz */
#define I2C_TWBR_VAL 18
#define I2C_PRESCALER 4
#define I2C_F_SCL = F_CPU/(16+2*TWBR_VAL*I2C_PRESCALER)

#define I2C_SLA_ATH20 0x38

static inline void i2c_init()
{
	/* Enabling TWI and interrupt at each bus event */
	TWCR = (1 << TWEN) | (1 << TWIE);
	TWSR |= (1 << TWPS0);
	TWBR = I2C_TWBR_VAL;
}

static inline void i2c_m_wait_for_op_to_proceed(void)
{
	/* Wait until TWINT flag is set */
	while (!(TWCR & (1 << TWINT)));
}

static inline void i2c_start(void)
{
	/* Deactivate interrupts */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();
}

static inline void i2c_sla_wr(uint8_t sla, uint8_t wr_bit)
{
	TWDR = (sla << 1) | wr_bit;
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();
}

static inline void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();
}

static inline uint8_t i2c_read(uint8_t twea_bit)
{
	uint8_t data = TWDR;

	TWCR = (1 << TWINT) | twea_bit | (1 << TWEN);
	i2c_m_wait_for_op_to_proceed();
	return data;
}

static inline void i2c_reset_bus_error(void)
{
	TWCR = (1 << TWINT) |  (1 << TWSTO) | (1 <<TWEN) ;
}

static inline void i2c_stop(void)
{
	/* Activate interrupts (case we're addressed as slave) */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE);
}

#endif /* I2C_H */