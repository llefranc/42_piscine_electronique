/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/25 19:20:42 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 256

ISR(TIMER1_COMPA_vect)
{
	uart_print_hex_value(adc_read());
	uart_printstr("\r\n");
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, 0);
	adc_init();
	adc_mux_select(ADC_POT);

	/* Timer 1 match every 20 ms */
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	OCR1A = F_CPU / TIMER1_PRESCALER / 100 * 2;

	while (1);
}