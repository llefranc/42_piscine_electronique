/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 18:28:42 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/25 19:14:47 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

void uart_init(uint16_t ubbrn, uint8_t rxcie0)
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

	/* Enabling Rx complete interrupt if necessary + Rx and Tx units */
	UCSR0B |= rxcie0 | (1 << RXEN0) | (1 << TXEN0);

	/*
	 * Enabling Double Speed Operation, it gives less error for UBRRn
	 * calculation with our F_CPU and baudrate.
	 */
	UCSR0A |= (1 << U2X0);
}

void uart_print_hex_value(uint8_t c)
{
	static const char *hex = "0123456789ABCDEF";

	uart_tx(hex[(c >> 4)]);
	uart_tx(hex[(c & 0x0F)]);
}

/**
 * Print an unsigned number.
*/
void uart_print_u_nb(uint32_t nb)
{
	if (nb >= 10)
		uart_print_u_nb(nb / 10);
	uart_tx(nb % 10 + 48);
}