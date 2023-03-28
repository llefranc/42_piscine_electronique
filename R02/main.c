/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:17:22 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/28 13:52:04 by lucaslefran      ###   ########.fr       */
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

ISR(TIMER1_COMPA_vect)
{
	switch (g_mode) {
	case E_MODE_0_START_SEQ:
		switch_mode(MODE_START_NEXT);
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

/* SW1, incrementing mode */
ISR(INT0_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)
		switch_mode(MODE_START_NEXT);
		
	/* Neutralizing bounce effect */
	_delay_ms(20);
	EIFR |= (1 << INTF0);
}

/* SW2, decrementing mode */
ISR(PCINT2_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)
		switch_mode(MODE_START_PREV);
		
	/* Neutralizing bounce effect */
	_delay_ms(20);
	PCIFR |= (1 << PCIF2);
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
 * Enable INT0 interrupt for SW1 and PCIN2 interrupt for SW2.
*/
static inline void button_init(void)
{
	EIMSK |= (1 << INT0);  
	EICRA |= (1 << ISC00);
	
	PCICR |= (1 << PCIE2);    
	PCMSK2 |= (1 << PCINT20);
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, 0);
	i2c_init();
	io_init();
	switch_mode(MODE_START_ACTUAL);

	while (g_mode == E_MODE_0_START_SEQ);
	button_init();

	while (1) {

	}
}
