/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/25 19:20:24 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 256

ISR(TIMER1_COMPA_vect)
{
	uint16_t temp = adc_read_10bit() - ADC_INTERNAL_SENSOR_TEMP_CAL_OFFSET;

	uart_print_u_nb(temp);
	uart_printstr(".C\r\n");
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, 0);

	/* Enabling adc + prescaler */
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	/* Internal 1.1V voltage reference needed for internal temp sensor */
	ADMUX |= (1 << REFS1) | (1 << REFS0);
	// ADMUX |= (1 << REFS1) | (1 << REFS0) | (1 << ADLAR);

	/* Selecting internal temperature sensor as input */
	adc_mux_select(ADC_TEMP);

	/* Timer 1 match every 20 ms */
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	OCR1A = F_CPU / TIMER1_PRESCALER / 100 * 2;

	while (1);
}