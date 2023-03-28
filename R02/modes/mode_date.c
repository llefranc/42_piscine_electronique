/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_date.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:29:09 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/28 13:49:20 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_date.h"

#include "../utils/uart.h"

void mode_9_date_hour_init(void)
{
	UART_DEBUG("mode_9_date_hour_init\r\n");
}

void mode_9_date_hour_clear(void)
{
	UART_DEBUG("mode_9_date_hour_clear\r\n");
}

void mode_10_date_day_init(void)
{
	UART_DEBUG("mode_10_date_day_init\r\n");
}

void mode_10_date_day_clear(void)
{
	UART_DEBUG("mode_10_date_day_clear\r\n");
}

void mode_11_date_year_init(void)
{
	UART_DEBUG("mode_11_date_year_init\r\n");
}

void mode_11_date_year_clear(void)
{
	UART_DEBUG("mode_11_date_year_clear\r\n");
}