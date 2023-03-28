/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_adc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:20:43 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/28 21:02:40 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode_adc.h"

#include "../utils/uart.h"
#include "../utils/adc.h"
#include "../utils/i2c_pca.h"
#include "../utils/timer.h"

static uint16_t adc_val;
static uint8_t dx[4] = { 0, 0, 0, 0 };

/**
 * Init the ADC to potentiometer, timer1 to match every 20ms in order to read
 * ADC potentiometer value, and timer0 to match every 2,5ms to display ADC
 * potentiometer value on segments.
*/
void mode_1_adc_pot_init(void)
{
	UART_DEBUG("mode_1_adc_pot_init\r\n");
	adc_init((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0), 0);
	adc_mux_select(ADC_POT);
	adc_val = 0;

	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU / TIMER_PRESCALER_1024 / 10; /* match every 20ms */
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 = (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 400; /* match every 2,5ms */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);
}

/**
 * Init the ADC to ldr, timer1 to match every 20ms in order to read ADC ldr
 * value, and timer0 to match every 2,5ms to display ADC ldr value on segments.
*/
void mode_2_adc_ldr_init(void)
{
	UART_DEBUG("mode_2_adc_ldr_init\r\n");
	adc_init((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0), 0);
	adc_mux_select(ADC_LDR);
	adc_val = 0;

	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU / TIMER_PRESCALER_1024 / 10; /* match every 20ms */
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 = (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 400; /* match every 2,5ms */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);
}


void mode_3_adc_ntc_init(void)
{
	UART_DEBUG("mode_3_adc_ntc_init\r\n");
	adc_init((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0), 0);
	adc_mux_select(ADC_NTC);
	adc_val = 0;

	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU / TIMER_PRESCALER_1024 / 50; /* match every 20ms */
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 = (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 400; /* match every 2,5ms */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);
}

void mode_4_adc_temp_init(void)
{
	UART_DEBUG("mode_4_adc_temp_init\r\n");
}

/**
 * Display on segment ADC 10bits value.
*/
void mode_x_adc_xxx_exec_timer0(void)
{
	i2c_pca_draw_seg_nb(adc_val, dx, 4);
}

/**
 * Read the value of ADC in 10bits mode.
*/
void mode_x_adc_xxx_exec_timer1(void)
{
	adc_val = adc_read_10bit();
}

void mode_x_adc_xxx_clear(void)
{
	UART_DEBUG("mode_1_adc_pot_clear\r\n");
	timer0_clear();
	timer1_clear();
	adc_clear();
	i2c_pca_reset_seg();
}