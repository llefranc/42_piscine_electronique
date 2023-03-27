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

#include "uart.h"
#include "i2c.h"
#include "i2c_pca.h"
#include "spi.h"
#include "adc.h"
#include "timer.h"
#include "define.h"
#include "mode.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static volatile int8_t mode = E_MODE_INIT;

static void (*start_mode[11])(void) = {
	&mode_1_adc_pot_start,
	&mode_2_adc_ldr_start,
	&mode_3_adc_ntc_start,
	&mode_4_adc_temp_start,
	&mode_5_rgb_start,
	&mode_6_aht_temp_c_start,
	&mode_7_aht_temp_f_start,
	&mode_8_aht_humi_start,
	&mode_9_time_start,
	&mode_10_date_day_start,
	&mode_11_date_year_start
};

static void (*stop_mode[11])(void) = {
	&mode_1_adc_pot_stop,
	&mode_2_adc_ldr_stop,
	&mode_3_adc_ntc_stop,
	&mode_4_adc_temp_stop,
	&mode_5_rgb_stop,
	&mode_6_aht_temp_c_stop,
	&mode_7_aht_temp_f_stop,
	&mode_8_aht_humi_stop,
	&mode_9_time_stop,
	&mode_10_date_day_stop,
	&mode_11_date_year_stop
};

ISR(TIMER1_COMPA_vect)
{
	switch (mode) {
	case E_MODE_INIT:
		timer0_clear();
		timer1_clear();
		i2c_pca_reset_seg(0);
		PORTB &= ~((1 << LED4) | (1 << LED3) | (1 << LED2) |
			   (1 << LED1));
		_delay_ms(1000);
		mode = FIRST_MODE_NB;
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

static inline void update_mode(int8_t dir)
{
	uint8_t tmp;

	if (dir != 0) {
		stop_mode[mode - 1]();
		mode += dir;
		if (mode < FIRST_MODE_NB)
			mode = LAST_MODE_NB;
		else if (mode > LAST_MODE_NB)
			mode = FIRST_MODE_NB;
	}
	tmp = mode;
	PORTB &= ~((1 << LED4) | (1 << LED3) | (1 << LED2) | (1 << LED1));
	if (tmp & (1 << 3)) {
		tmp |= (1 << PB4);
		tmp &= ~(1 << 3);
	}
	PORTB |= tmp;
	start_mode[mode - 1]();
}

/* SW1, incrementing mode */
ISR(INT0_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)
		update_mode(1);
	_delay_ms(20);        /* Neutralizing bounce effect */
	EIFR |= (1 << INTF0); /* Clearing int flag if one occured while ISR */
}

/* SW2, decrementing mode */
ISR(PCINT2_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)
		update_mode(-1);
	_delay_ms(20);         /* Neutralizing bounce effect */
	PCIFR |= (1 << PCIF2); /* Clearing int flag if one occured while ISR */
}

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

static inline void io_init(void)
{
	/* LED D1, D2, D3 and D4 as output */
	DDRB |= (1 << LED4) | (1 << LED3) | (1 << LED2) | (1 << LED1);

	/* LED D5 with 3 RGB pins as output */
	DDRD |= (1 << LED5_R) | (1 << LED5_G) | (1 << LED5_B);

	/* Button SW3 as input, LED D9, D10, D11 and segments as output */
	i2c_pca_write_reg(I2C_PCA_C0, 0xFE);
	i2c_pca_write_reg(I2C_PCA_C1, 0xFF);
}

static inline void button_init(void)
{
	EIMSK |= (1 << INT0);  /* Enabling interrupt INT0 for SW1*/
	EICRA |= (1 << ISC00); /* Any logical change on INT0 generates an interrupt request. */

	PCICR |= (1 << PCIE2);    /* Enabling PCINT2 interrupt for PIN16-23 */
	PCMSK2 |= (1 << PCINT20); /* Only pin with PCINT20 can trigger PCINT2 (SW2) */
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, 0);
	i2c_init();
	io_init();
	start_sequence_init();

	while (mode == E_MODE_INIT);
	button_init();
	update_mode(0);

	while (1) {

	}
}
