/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_aht.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:27:23 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/28 13:49:14 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_aht.h"

#include "../utils/uart.h"

void mode_6_aht_temp_c_init(void)
{
	UART_DEBUG("mode_6_aht_temp_c_init\r\n");
}

void mode_6_aht_temp_c_clear(void)
{
	UART_DEBUG("mode_6_aht_temp_c_clear\r\n");
}

void mode_7_aht_temp_f_init(void)
{
	UART_DEBUG("mode_7_aht_temp_f_init\r\n");
}

void mode_7_aht_temp_f_clear(void)
{
	UART_DEBUG("mode_7_aht_temp_f_clear\r\n");
}

void mode_8_aht_humi_init(void)
{
	UART_DEBUG("mode_8_aht_humi_init\r\n");
}

void mode_8_aht_humi_clear(void)
{
	UART_DEBUG("mode_8_aht_humi_clear\r\n");
}
