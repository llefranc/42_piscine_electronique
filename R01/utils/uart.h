/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 18:28:54 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/28 13:48:21 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define UART_UBRRN F_CPU/8/UART_BAUDRATE-1

#ifdef DEBUG
#define UART_DEBUG(x) uart_printstr(x)
#else
#define UART_DEBUG(x) 
#endif

void uart_init(uint16_t ubbrn, uint8_t rxcie0);
void uart_print_hex_value(uint8_t c);
void uart_print_u_nb(uint32_t nb);

static inline void uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

static inline void uart_printstr(const char* str)
{
	uint16_t i = 0;

	while (str[i])
		uart_tx(str[i++]);
}

static inline char uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0))); /* Waiting to receive a byte */
	return UDR0;
}

#endif /* UART_H */