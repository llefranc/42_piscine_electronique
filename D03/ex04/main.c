/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:08:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/17 22:41:08 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define UBRRN F_CPU/8/UART_BAUDRATE-1
#define TIMER1_PRESCALER 1024

#define BUFFER_SIZE 20

enum e_step {
	E_USERNAME,
	E_PASSWD,
	E_CHECK,
	E_END
};

static const char *creds[] = {"llefranc", "password"};

void avr_memset(void *ptr, char c, uint16_t len);	
int8_t avr_strcmp(const char *s1, const char *s2);

static inline void uart_tx(char c);
static inline char uart_rx(void);
static inline void uart_printstr(const char* str);

ISR(USART_RX_vect)
{
	char c;
	static uint8_t wrong_creds = 0;
	static uint8_t step = E_USERNAME;
	static char buf[BUFFER_SIZE] = {};
	static uint8_t i = 0;

	c = uart_rx();
	if (step == E_END)
		return ;
	if (c == '\r') {
		if (avr_strcmp(buf, creds[step]))
			wrong_creds = 1;
		step++;
		if (step == E_PASSWD) {
			uart_printstr("\r\n\tPassword: ");
		} else if (step == E_CHECK) {
			if (wrong_creds) {
				uart_printstr("\r\nBad combination username/password\r\n");
				goto err_reset_login;
			}
			uart_printstr("\r\nHello ");
			uart_printstr(creds[E_USERNAME]);
			uart_printstr("!\r\nShall we play a game ?\r\n");
			step = E_END;
		}
		goto reset_buff;
	} else if (c == 127) {
		if (i > 0) {
			buf[i--] = '\0';
			uart_printstr("\b \b");
		}
	} else if (i >= BUFFER_SIZE - 1) {
		uart_printstr("\r\nPlease enter less than 20 characters\r\n");
		goto err_reset_login;
	} else {
		buf[i++] = c;
		if (step == E_PASSWD)
			c = '*';
		uart_tx(c);
	}
	return ;

err_reset_login:
	step = E_USERNAME;
	uart_printstr("\r\nEnter your login:\r\n\tUsername: ");
	wrong_creds = 0;
reset_buff:
	avr_memset(buf, 0, sizeof(buf));
	i = 0;
}

void avr_memset(void *ptr, char c, uint16_t len)
{
	uint16_t i = 0;

	while (i < len)
		((char *)ptr)[i++] = c;
}

int8_t avr_strcmp(const char *s1, const char *s2)
{
	uint16_t i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		++i;
	return s1[i] - s2[i];
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

int main(void)
{
	sei();
	uart_init(UBRRN);
	uart_printstr("Enter your login:\r\n\tUsername: ");
	while (1) {};
}