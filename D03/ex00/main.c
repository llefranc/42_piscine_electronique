/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:08:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/17 18:03:13 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Transmission du caractere S (ASCII val = 83) en 7 bits: 0 1 0 1 0 1 1
// -> on transmet en reverse 1 1 0 1 0 1 0

#include <avr/io.h>
#include <avr/interrupt.h>

#define UBRRN F_CPU/8/UART_BAUDRATE-1
#define TIMER1_PRESCALER 256

static inline void uart_tx(char c);

ISR(TIMER1_COMPA_vect)
{
	uart_tx('Z');
}

static inline void uart_init(uint16_t ubbrn)
{
	UBRR0H = (unsigned char)(ubbrn >> 8); /* Setting baudrate */
	UBRR0L = (unsigned char)ubbrn;

	/*
	 * Setting USART to:
	 * 	- asynchronous mode
	 * 	- No bit parity
	 * 	- 1 stop bit
	 * 	- 8 data bits
	 */
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); /* Enabling Rx and Tx units */

	/*
	 * Enabling Double Speed Operation, it gives less error for UBRRn
	 * calculation with our F_CPU and baudrate.
	 */
	UCSR0A |= (1 << U2X0);
}

static inline void uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

static inline void set_timer1(void)
{
	TCCR1B |= (1 << WGM12) | (1 << CS12); /* CTC mode with OCR1A as TOP value + prescaler 256 */
	OCR1A = F_CPU / TIMER1_PRESCALER;     /* 1 Hz frequency */
	TIMSK1 |= (1 << OCIE1A);
}

int main(void)
{
	sei();
	set_timer1();
	uart_init(UBRRN);

	while (1);
}