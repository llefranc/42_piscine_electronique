/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:08:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/17 18:13:32 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define UBRRN F_CPU/8/UART_BAUDRATE-1
#define TIMER1_PRESCALER 1024

static inline void uart_tx(char c);
static inline void uart_printstr(const char* str);

ISR(TIMER1_COMPA_vect)
{
	uart_printstr("Hello world!\r\n"); /* screen program on Linux need \r\n for new line*/
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

static inline void uart_printstr(const char* str)
{
	uint16_t i = 0;

	while (str[i])
		uart_tx(str[i++]);
}

static inline void uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

static inline void set_timer1(void)
{
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); /* CTC mode with OCR1A as TOP value + prescaler 1024 */
	OCR1A = F_CPU / TIMER1_PRESCALER * 2; /* 0,5 Hz frequency */
	TIMSK1 |= (1 << OCIE1A);
}

int main(void)
{
	sei();
	set_timer1();
	uart_init(UBRRN);

	while (1);
}