/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_pcf.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 09:35:55 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/30 11:10:12 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I2C_PCF_H
#define I2C_PCF_H

#include <avr/io.h>

// https://www.nxp.com/docs/en/data-sheet/PCF8563.pdf

#define I2C_PCF_SLA 0b01010001

struct date {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
};

void i2c_pcf_read_date(struct date *buf);
void i2c_pcf_write_date(struct date *buf);

#endif /* I2C_PCF_H */