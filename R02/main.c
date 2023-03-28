/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:17:22 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 18:01:12 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "define.h"
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

static volatile int8_t g_mode = E_MODE_INIT;

static void (*g_fptr_mode_init[11])(void) = {
	&mode_1_adc_pot_init,
	&mode_2_adc_ldr_init,
	&mode_3_adc_ntc_init,
	&mode_4_adc_temp_init,
	&mode_5_rgb_init,
	&mode_6_aht_temp_c_init,
	&mode_7_aht_temp_f_init,
	&mode_8_aht_humi_init,
	&mode_9_time_init,
	&mode_10_date_day_init,
	&mode_11_date_year_init
};

static void (*g_fptr_mode_clear[11])(void) = {
	&mode_1_adc_pot_clear,
	&mode_2_adc_ldr_clear,
	&mode_3_adc_ntc_clear,
	&mode_4_adc_temp_clear,
	&mode_5_rgb_clear,
	&mode_6_aht_temp_c_clear,
	&mode_7_aht_temp_f_clear,
	&mode_8_aht_humi_clear,
	&mode_9_time_clear,
	&mode_10_date_day_clear,
	&mode_11_date_year_clear
};

ISR(TIMER1_COMPA_vect)
{
	switch (g_mode) {
	case E_MODE_INIT:
		timer0_clear();
		timer1_clear();
		i2c_pca_reset_seg(0);
		PORTB &= ~((1 << LED4) | (1 << LED3) | (1 << LED2) |
			 (1 << LED1));
		_delay_ms(1000);
		g_mode = FIRST_MODE_NB;
		break;
	default:
		uart_printstr("Mode error\r\n");
	}
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t dx[4] = {};

	i2c_pca_draw_seg_nb(8888, dx, 4, 0);
}

/**
 * Switch mode by stopping the previous one and starting another and updating 
 * the LEDs accordingly.
 * @dir: If negative/positive number, stop actual mode and start another mode by 
 *       going backward/forward in the modes number. If dir is 0, just start the 
 *       mode referenced by global variable g_mode (usefull for first mode to
 *       launch at start).
*/
static inline void update_mode(int8_t dir)
{
	uint8_t tmp;

	if (dir != 0) {
		g_fptr_mode_clear[g_mode - 1]();
		g_mode += dir;
		if (g_mode < FIRST_MODE_NB)
			g_mode = LAST_MODE_NB;
		else if (g_mode > LAST_MODE_NB)
			g_mode = FIRST_MODE_NB;
	}
	tmp = g_mode;
	PORTB &= ~((1 << LED4) | (1 << LED3) | (1 << LED2) | (1 << LED1));
	if (tmp & (1 << 3)) {
		tmp |= (1 << PB4);
		tmp &= ~(1 << 3);
	}
	PORTB |= tmp;
	g_fptr_mode_init[g_mode - 1]();
}

/* SW1, incrementing mode */
ISR(INT0_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)
		update_mode(MODE_START_NEXT);
	_delay_ms(20);        /* Neutralizing bounce effect */
	EIFR |= (1 << INTF0); /* Clearing int flag if one occured while ISR */
}

/* SW2, decrementing mode */
ISR(PCINT2_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)
		update_mode(MODE_START_PREV);
	_delay_ms(20);         /* Neutralizing bounce effect */
	PCIFR |= (1 << PCIF2); /* Clearing int flag if one occured while ISR */
}

/**
 * Light on all segments and LED D1,D2,D3 and D4 for 3 sec at start.
*/
static inline void start_sequence_init(void)
{
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = F_CPU / TIMER_PRESCALER_1024 * 3; /* match every 3sec */
	TCCR1B |= (1 < WGM12) | (1 << CS12) | (1 << CS10);

	TIMSK0 |= (1 << OCIE0A);
	OCR0A = F_CPU / TIMER_PRESCALER_1024 / 200; /* match every 5ms*/
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);

	PORTB |= (1 << LED4) | (1 << LED3) | (1 << LED2) | (1 << LED1);
}

/**
 * Init the input/output for AVR pins and PCA expander pins.
 * - LED D1, D2, D3 and D4 as output.
 * - LED D5 with 3 RGB pins as output.
 * - Expander: button SW3 as input, LED D9, D10, D11 and segments as output.
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
	start_sequence_init();

	while (g_mode == E_MODE_INIT);
	button_init();
	update_mode(0);

	while (1) {

	}
}
