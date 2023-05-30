/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_date.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:29:37 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/30 10:46:18 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_DATE_H
#define MODE_DATE_H

#include <avr/io.h>

int8_t mode_date_update(char *buf);

void mode_x_date_xxx_init(void);
void mode_x_date_xxx_timer1(void);
void mode_x_date_xxx_clear(void);

void mode_9_date_time_timer0(void);
void mode_10_date_day_timer0(void);
void mode_11_date_year_timer0(void);

#endif /* MODE_DATE_H */