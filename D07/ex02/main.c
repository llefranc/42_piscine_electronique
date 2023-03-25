/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/25 18:37:11 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"
#include "adc.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 256

ISR(TIMER1_COMPA_vect)
{
	static const uint8_t adc_input[3] = { ADC_POT, ADC_LDR, ADC_NTC };
	uint16_t tmp;

	for (uint8_t i = 0; i < sizeof(adc_input) / sizeof(*adc_input); ++i) {
		adc_mux_select(adc_input[i]);
		tmp = adc_read_10bit();
		uart_print_hex_value((uint8_t)((tmp & 0xFF00) >> 8));
		uart_print_hex_value((uint8_t)tmp);
		if (i < sizeof(adc_input) / sizeof(*adc_input) - 1)
			uart_printstr(", ");
	}
	uart_printstr("\r\n");
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, 0);
	adc_init((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) ,0);

	/* Timer 1 match every 20 ms */
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	OCR1A = F_CPU / TIMER1_PRESCALER / 100 * 2;

	while (1);
}