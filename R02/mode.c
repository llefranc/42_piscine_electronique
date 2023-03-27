/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:12:27 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 17:28:20 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.h"

#include "uart.h"

void mode_1_adc_pot_start(void)
{
	uart_printstr("mode_1_adc_pot_start\r\n");
}

void mode_1_adc_pot_stop(void)
{
	uart_printstr("mode_1_adc_pot_stop\r\n");
}

void mode_2_adc_ldr_start(void)
{
	uart_printstr("mode_2_adc_ldr_start\r\n");
}

void mode_2_adc_ldr_stop(void)
{
	uart_printstr("mode_2_adc_ldr_stop\r\n");
}

void mode_3_adc_ntc_start(void)
{
	uart_printstr("mode_3_adc_ntc_start\r\n");
}

void mode_3_adc_ntc_stop(void)
{
	uart_printstr("mode_3_adc_ntc_stop\r\n");
}

void mode_4_adc_temp_start(void)
{
	uart_printstr("mode_4_adc_temp_start\r\n");
}

void mode_4_adc_temp_stop(void)
{
	uart_printstr("mode_4_adc_temp_stop\r\n");
}

void mode_5_rgb_start(void)
{
	uart_printstr("mode_5_rgb_start\r\n");
}

void mode_5_rgb_stop(void)
{
	uart_printstr("mode_5_rgb_stop\r\n");
}

void mode_6_aht_temp_c_start(void)
{
	uart_printstr("mode_6_aht_temp_c_start\r\n");
}

void mode_6_aht_temp_c_stop(void)
{
	uart_printstr("mode_6_aht_temp_c_stop\r\n");
}

void mode_7_aht_temp_f_start(void)
{
	uart_printstr("mode_7_aht_temp_f_start\r\n");
}

void mode_7_aht_temp_f_stop(void)
{
	uart_printstr("mode_7_aht_temp_f_stop\r\n");
}

void mode_8_aht_humi_start(void)
{
	uart_printstr("mode_8_aht_humi_start\r\n");
}

void mode_8_aht_humi_stop(void)
{
	uart_printstr("mode_8_aht_humi_stop\r\n");
}

void mode_9_time_start(void)
{
	uart_printstr("mode_9_time_start\r\n");
}

void mode_9_time_stop(void)
{
	uart_printstr("mode_9_time_stop\r\n");
}

void mode_10_date_day_start(void)
{
	uart_printstr("mode_10_date_day_start\r\n");
}

void mode_10_date_day_stop(void)
{
	uart_printstr("mode_10_date_day_stop\r\n");
}

void mode_11_date_year_start(void)
{
	uart_printstr("mode_11_date_year_start\r\n");
}

void mode_11_date_year_stop(void)
{
	uart_printstr("mode_11_date_year_stop\r\n");
}