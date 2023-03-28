/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_adc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:20:43 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/28 13:49:08 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_adc.h"

#include "../utils/uart.h"

void mode_1_adc_pot_init(void)
{
	UART_DEBUG("mode_1_adc_pot_init\r\n");
}

void mode_1_adc_pot_clear(void)
{
	UART_DEBUG("mode_1_adc_pot_clear\r\n");
}

void mode_2_adc_ldr_init(void)
{
	UART_DEBUG("mode_2_adc_ldr_init\r\n");
}

void mode_2_adc_ldr_clear(void)
{
	UART_DEBUG("mode_2_adc_ldr_clear\r\n");
}

void mode_3_adc_ntc_init(void)
{
	UART_DEBUG("mode_3_adc_ntc_init\r\n");
}

void mode_3_adc_ntc_clear(void)
{
	UART_DEBUG("mode_3_adc_ntc_clear\r\n");
}

void mode_4_adc_temp_init(void)
{
	UART_DEBUG("mode_4_adc_temp_init\r\n");
}

void mode_4_adc_temp_clear(void)
{
	UART_DEBUG("mode_4_adc_temp_clear\r\n");
}
