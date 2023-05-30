/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_aht.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 10:24:16 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/29 13:21:19 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I2C_AHT_H
#define I2C_AHT_H

#define I2C_ATH_SLA 0x38

void i2c_aht_start_measurement(void);
void i2c_aht_read_measurement(void);
float i2c_aht_calc_temp(void);
float i2c_aht_calc_humi(void);

#endif