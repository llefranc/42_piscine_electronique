/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:08:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/17 18:58:42 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define UBRRN F_CPU/8/UART_BAUDRATE-1
#define TIMER1_PRESCALER 1024

static inline void uart_tx(char c);
static inline char uart_rx(void);

ISR(USART_RX_vect)
{
	char c = uart_rx();

	if (c >= 'a' && c <= 'z')
		c -= 32;
	else if (c >= 'A' && c <= 'Z')
		c += 32;
	uart_tx(c);
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

	/* Enabling Rx complete interrupt + Rx and Tx units */
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

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

static inline char uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0))); /* Waiting to receive a byte */
	return UDR0;
}

int main(void)
{
	sei();
	uart_init(UBRRN);
	while (1) {};
}