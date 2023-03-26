/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 20:17:34 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TIMER1_PRESCALER 256
#define SHIFT_WIDTH 8

static uint8_t led_index;
static int8_t rgb_color_index = 2;

/* SW1 button will change RGB value */
ISR(INT0_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed) {
		if (rgb_color_index >= 0)
			--rgb_color_index;
	}
	_delay_ms(20);        /* Neutralizing bounce effect */
	EIFR |= (1 << INTF0); /* Clearing interrupt flag one occured while ISR was executing*/
}

/* SW2 button will change LED */
ISR(PCINT2_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed) {
		if (++led_index >= 3)
			led_index = 0;
		rgb_color_index = 2;
	}
	_delay_ms(20);         /* Neutralizing bounce effect */
	PCIFR |= (1 << PCIF2); /* Clearing interrupt flag if another one occured while ISR was executing*/
}

ISR(TIMER1_COMPA_vect)
{
	static uint32_t colors[3] = {};
	uint8_t val;

	if (rgb_color_index < 0)
		return;
	val = adc_read();
	colors[led_index] &= ~(0x000000FFL << (SHIFT_WIDTH * rgb_color_index));
	colors[led_index] |= (uint32_t)val << (SHIFT_WIDTH * rgb_color_index);
	spi_set_leds(B_LOW | colors[0], B_LOW | colors[1], B_LOW | colors[2]);
}

static inline void btn_init(void)
{
	EIMSK |= (1 << INT0);  /* Enabling interrupt INT0 for SW1*/
	EICRA |= (1 << ISC00); /* Any logical change on INT0 generates an interrupt request. */

	PCICR |= (1 << PCIE2);    /* Enabling PCINT2 interrupt for PIN16-23 */
	PCMSK2 |= (1 << PCINT20); /* Only pin with PCINT20 can trigger PCINT2 (SW2) */
}

static inline void timer1_init(void)
{
	/* Timer 1 match every 20 ms */
	OCR1A = F_CPU / TIMER1_PRESCALER / 100 * 2;
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
}

int main(void)
{
	sei();
	spi_init();
	btn_init();
	timer1_init();
	adc_init();
	adc_mux_select(ADC_POT);

	while (1) {}
}
