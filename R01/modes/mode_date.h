/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_date.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:29:37 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/29 15:55:41 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_DATE_H
#define MODE_DATE_H

#include <avr/io.h>

int8_t mode_date_update(char *buf);
void mode_9_date_hour_init(void);
void mode_9_date_hour_clear(void);
void mode_10_date_day_init(void);
void mode_10_date_day_clear(void);
void mode_11_date_year_init(void);
void mode_11_date_year_clear(void);

#endif /* MODE_DATE_H */