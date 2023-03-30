/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:17:22 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/30 11:05:52 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/io.h"
#include "utils/uart.h"
#include "utils/i2c.h"
#include "utils/i2c_pca.h"
#include "utils/spi.h"
#include "utils/adc.h"
#include "utils/timer.h"
#include "utils/avr_string.h"
#include "modes/mode.h"
#include "modes/mode_date.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DATE_BUFFER_SIZE 25

static inline void button_init(void);

ISR(TIMER0_COMPA_vect)
{
	switch (g_mode) {
	case E_MODE_0_START_SEQ:
		mode_0_start_seq_exec_timer0();
		break;
	case E_MODE_1_ADC_POT:
	case E_MODE_2_ADC_LDR:
	case E_MODE_3_ADC_NTC:
	case E_MODE_4_ADC_TEMP:
		mode_x_adc_xxx_exec_timer0();
		break;
	case E_MODE_5_RGB:
		mode_5_rgb_exec_timer0();
		break;
	case E_MODE_6_AHT_TEMPC:
		mode_x_aht_xxx_exec_timer0('c');
		break;
	case E_MODE_7_AHT_TEMPF:
		mode_x_aht_xxx_exec_timer0('f');
		break;
	case E_MODE_8_AHT_HUMI:
		mode_x_aht_xxx_exec_timer0('p');
		break;
	case E_MODE_9_DATE_HOUR:
		mode_9_date_time_timer0();
		break;
	case E_MODE_10_DATE_DAY:
		mode_10_date_day_timer0();
		break;
	case E_MODE_11_DATE_YEAR:
		mode_11_date_year_timer0();
		break;
	default:
		UART_DEBUG("TIMER0_COMPA_vect error\r\n");
	}
}

ISR(TIMER1_COMPA_vect)
{
	switch (g_mode) {
	case E_MODE_0_START_SEQ:
		switch_mode(MODE_START_NEXT);
		button_init();
		break;
	case E_MODE_1_ADC_POT:
	case E_MODE_2_ADC_LDR:
	case E_MODE_3_ADC_NTC:
		mode_x_adc_xxx_exec_timer1();
		break;
	case E_MODE_4_ADC_TEMP:
		mode_4_adc_temp_exec_timer1();
		break;
	case E_MODE_5_RGB:
		mode_5_rgb_exec_timer1();
		break;
	case E_MODE_6_AHT_TEMPC:
		mode_6_aht_temp_c_exec_timer1();
		break;
	case E_MODE_7_AHT_TEMPF:
		mode_7_aht_temp_f_exec_timer1();
		break;
	case E_MODE_8_AHT_HUMI:
		mode_8_aht_humi_exec_timer1();
		break;
	case E_MODE_9_DATE_HOUR:
	case E_MODE_10_DATE_DAY:
	case E_MODE_11_DATE_YEAR:
		mode_x_date_xxx_timer1();
		break;
	default:
		UART_DEBUG("TIMER1_COMPA_vect error\r\n");
	}
}

/* Checking SW3 input from expander through i2c every 10 ms */
ISR(TIMER2_COMPA_vect)
{
	uint8_t i2c_pca_i0_data;
	static int8_t is_pressed = -1;

	i2c_pca_i0_data = i2c_pca_read_reg(I2C_PCA_I0);

	/* Check pin state after start seq case button is already pressed */
	if (is_pressed == -1 && ~i2c_pca_i0_data & (1 << I2C_PCA0_SW5))
		return;
	else if (is_pressed == -1)
		is_pressed = 0;

	if ((!is_pressed && (~i2c_pca_i0_data & (1 << I2C_PCA0_SW5))) ||
	    (is_pressed && (!(~i2c_pca_i0_data & (1 << I2C_PCA0_SW5))))) {
		is_pressed = !is_pressed;
		i2c_pca_write_regO0(~(1 << I2C_PCA0_D11),
				    (is_pressed << I2C_PCA0_D11));
		_delay_ms(10);
	}
}

ISR(INT0_vect)
{
	static int8_t is_pressed = -1;

	UART_DEBUG("INT0_vect\r\n");
	/* Check pin state after start seq case button is already pressed */
	if (is_pressed == -1)
		is_pressed = !(PIND & (1 << SW1));
	else
		is_pressed = !is_pressed;

	if (is_pressed) {
		i2c_pca_write_regO0(~(1 << I2C_PCA0_D9), (1 << I2C_PCA0_D9));
		switch_mode(MODE_START_NEXT);
	} else {
		i2c_pca_write_regO0(~(1 << I2C_PCA0_D9), 0);
	}
	/* Neutralizing bounce effect */
	_delay_ms(10);
	EIFR |= (1 << INTF0);
}

/* SW2, decrementing mode */
ISR(PCINT2_vect)
{
	static int8_t is_pressed = -1;

	UART_DEBUG("PCINT2_vect\r\n");
	/* Check pin state after start seq case button is already pressed */
	if (is_pressed == -1)
		is_pressed = !(PIND & (1 << SW2));
	else
		is_pressed = !is_pressed;

	if (is_pressed) {
		i2c_pca_write_regO0(~(1 << I2C_PCA0_D10), (1 << I2C_PCA0_D10));
		switch_mode(MODE_START_PREV);
	} else {
		i2c_pca_write_regO0(~(1 << I2C_PCA0_D10), 0);
	}
	/* Neutralizing bounce effect */
	_delay_ms(10);
	PCIFR |= (1 << PCIF2);
}

/* Update date through UART */
ISR(USART_RX_vect)
{
	char c;
	static uint8_t i = 0;
	static char buf[DATE_BUFFER_SIZE] = {};

	c = uart_rx();
	if (c == '\r') {
		if (mode_date_update(buf) != -1)
			uart_printstr("\r\nDate updated!");
		else
			goto err_parsing;
		uart_printstr("\r\nTo update the date, enter a new one with the"
			      " following format \"DD/MM/YYYY HH:MM:SS\":\r\n");
		goto reset_buf;
	} else if (c == 127) {
		if (i > 0) {
			buf[--i] = '\0';
			uart_printstr("\b \b");
		}
	} else if (i < DATE_BUFFER_SIZE - 1) {
		buf[i++] = c;
		uart_tx(c);
	}
	return;

err_parsing:
	uart_printstr("\r\nError: please respect the date format \"DD/MM/YYYY "
		      "HH:MM:SS\"\r\n");
reset_buf:
	i = 0;
	avr_memset(buf, 0, sizeof(buf));
}

/**
 * Init the input/output for AVR pins and PCA expander pins.
 * - LED D1, D2, D3 and D4 as output.
 * - LED D5 with 3 RGB pins as output.
 * - Expander: SW3 as input, LED D9, D10, D11 and segments as output.
*/
static inline void io_init(void)
{
	DDRB |= (1 << LED4) | (1 << LED3) | (1 << LED2) | (1 << LED1);
	DDRD |= (1 << LED5_R) | (1 << LED5_G) | (1 << LED5_B);
	i2c_pca_write_reg(I2C_PCA_C0, 0xFE);
	i2c_pca_write_reg(I2C_PCA_C1, 0xFF);
}

/**
 * Enable INT0 interrupt for SW1, PCINT2 interrupt for SW2 and start
 * timer2 for check SW3 status through I2C every 10 ms.
*/
static inline void button_init(void)
{
	EICRA |= (1 << ISC00);
	EIFR |= (1 << INT0);
	EIMSK |= (1 << INT0);

	PCIFR |= (1 << PCIF2);
	PCMSK2 |= (1 << PCINT20);
	PCICR |= (1 << PCIE2);

	OCR2A = F_CPU / TIMER_PRESCALER_1024 / 100; /* every 10ms */
	TIMSK2 |= (1 << OCIE2A);
	TCCR2A |= (1 << WGM21);
	TCCR2B |= (1 << CS22) | (1 << CS20);
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, (1 << RXCIE0));
	i2c_init();
	io_init();
	switch_mode(MODE_START_ACTUAL);
	uart_printstr("\r\nTo update the date, enter a new one with the"
		      " following format \"DD/MM/YYYY HH:MM:SS\":\r\n");
	while (1);
}
