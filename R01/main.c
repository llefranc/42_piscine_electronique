/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:17:22 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/28 19:09:03 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/io.h"
#include "utils/uart.h"
#include "utils/i2c.h"
#include "utils/i2c_pca.h"
#include "utils/spi.h"
#include "utils/adc.h"
#include "utils/timer.h"
#include "modes/mode.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static inline void button_init(void);

ISR(TIMER1_COMPA_vect)
{
	switch (g_mode) {
	case E_MODE_0_START_SEQ:
		switch_mode(MODE_START_NEXT);
		button_init();
		break;
	default:
		uart_printstr("Mode error\r\n");
	}
}

ISR(TIMER0_COMPA_vect)
{
	switch (g_mode) {
	case E_MODE_0_START_SEQ:
		mode_0_start_seq_exec_timer0();
		break;
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
	uart_init(UART_UBRRN, 0);
	i2c_init();
	io_init();
	switch_mode(MODE_START_ACTUAL);

	while (1);
}
