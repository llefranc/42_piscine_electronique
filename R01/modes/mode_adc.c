/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_adc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:20:43 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/28 21:31:21 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_adc.h"

#include "../utils/uart.h"
#include "../utils/adc.h"
#include "../utils/i2c_pca.h"
#include "../utils/timer.h"
#include "mode.h"

static uint16_t adc_val;

/**
 * Init the ADC to a specific input through multiplexer, timer1 to match every
 * 100ms (1 sec for internal temp sensor) in order to read ADC value, and
 * timer0 to match every 2.5ms to display ADC value on segments.
*/
void mode_x_adc_xxx_init(void)
{
	static uint8_t inputs[4] = { ADC_POT, ADC_LDR, ADC_NTC, ADC_TEMP };

	UART_DEBUG("mode_1_adc_pot_init\r\n");
	if (g_mode == E_MODE_4_ADC_TEMP) {
		ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) |
			  (1 << ADPS0);

		/* Internal 1.1V voltage reference for internal temp sensor */
		ADMUX |= (1 << REFS1) | (1 << REFS0);
		OCR1A = F_CPU / TIMER_PRESCALER_1024; /* Every 1 sec */
	} else {
		adc_init((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0), 0);
		OCR1A = F_CPU / TIMER_PRESCALER_1024 / 10; /* Every 100ms */
	}
	adc_mux_select(inputs[g_mode - 1]);
	adc_val = 0;

	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 = (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 400; /* Every 2.5ms */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);
}

/**
 * Read the value of ADC internal temperature sensor in 10bits mode and
 * calibrate it.
*/
void mode_4_adc_temp_exec_timer1(void)
{
	int16_t tmp = adc_read_10bit() - ADC_INTERNAL_SENSOR_TEMP_CAL_OFFSET;

	if (tmp < 0)
		tmp = 0;
	adc_val = tmp;

}

/**
 * Display on segment ADC 10bits value.
*/
void mode_x_adc_xxx_exec_timer0(void)
{
	static uint8_t dx[4] = { 0, 0, 0, 0 };

	i2c_pca_draw_seg_nb(adc_val, dx, 4);
}

/**
 * Read the value of ADC in 10bits mode.
*/
void mode_x_adc_xxx_exec_timer1(void)
{
	adc_val = adc_read_10bit();
}

/**
 * Reset timer0, timer1, adc registers and turn off segments.
*/
void mode_x_adc_xxx_clear(void)
{
	UART_DEBUG("mode_1_adc_pot_clear\r\n");
	timer0_clear();
	timer1_clear();
	adc_clear();
	i2c_pca_reset_seg();
}