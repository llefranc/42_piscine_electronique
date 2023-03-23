/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:54:11 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/23 16:55:31 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"
#include "uart.h"
#include "utils.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUFFER_SIZE 50

#define NB_CMDS 3

#define R_NB_ARGS 1
#define W_NB_ARGS 2
#define F_NB_ARGS 1

#define R_CMD_SIZE 4
#define W_CMD_SIZE 5
#define F_CMD_SIZE 6

enum {
	E_READ,
	E_WRITE,
	E_FORGET,
};

static const char *cmd_token[3] = { "READ ", "WRITE ", "FORGET "};
static const uint8_t cmd_size[3] = { R_CMD_SIZE, W_CMD_SIZE, F_CMD_SIZE };
static const uint8_t cmd_nb_args[3] = { R_NB_ARGS, W_NB_ARGS, F_NB_ARGS };

void avr_memset(void *ptr, char c, uint16_t len);
int8_t avr_strncmp(const char *s1, const char *s2, uint16_t n);

/**
 * Count the number of args in a command.
*/
uint8_t count_args(const char *entry)
{
	char next;
	uint8_t i = 0;
	uint8_t nb_args = 0;

	while (entry[i]) {
		if (entry[i] == ' ') {
			next = entry[i + 1];
			if (next != ' ' && next !=  '\0')
				nb_args++;
		}
		i++;
		if (nb_args >= W_NB_ARGS)
			break;
	}
	return nb_args;
}

/**
 * Remove whitespaces at the end of a string.
*/
void r_trim_ws(char *buf)
{
	int8_t i = 0;
	while (buf[i] != '\0')
		i++;
	i--;
	while (i >= 0 && buf[i] == ' ')
		buf[i--] = '\0';
}

/**
 * Read from EEPROM a data associated to a specific id.
*/
void exec_read(char *buf, uint16_t id_size)
{
	if (eeprom_malloc_read(buf, id_size, BUFFER_SIZE) == -1)
		uart_printstr("error: unknow id");
	else
		uart_printstr(buf);
}

/**
 * Write in EEPROM a data associated to a specific id.
*/
void exec_write(char *buf, uint16_t id_size)
{
	int8_t ret = eeprom_malloc_write(buf, id_size);

	if (ret == -1)
		uart_printstr("error: id already stored in EEPROM");
	else if (ret == -2)
		uart_printstr("error: not enough storage");
	else
		uart_printstr("Done");
}

/**
 * Free in EEPROM a data associated to a specific id.
*/
void exec_forget(char *buf, uint16_t id_size)
{
	if (eeprom_malloc_free(buf, id_size) == -1)
		uart_printstr("error: unknow id");
	else
		uart_printstr("Done");
}

/**
 * Execute based on entry READ, WRITE or FORGET command.
*/
static inline void exec_cmd(char *buf)
{
	uint8_t i = 0;
	uint8_t nb_args;
	uint16_t id_size = 0;
	static void (*exec_fptr[3])(char *, uint16_t) = {
		&exec_read,
		&exec_write,
		&exec_forget,
	};

	r_trim_ws(buf);
	nb_args = count_args(buf);
	for (; i < NB_CMDS; ++i) {
		if (nb_args == cmd_nb_args[i] &&
		    !avr_strncmp(buf, cmd_token[i], cmd_size[i])) {
			buf = get_next_token(buf);
			id_size = count_chars_until_sep(buf, ' ');
			break;
		}
	}
	uart_printstr("\r\n");
	if (i < NB_CMDS)
		exec_fptr[i](buf, id_size);
	else
		uart_printstr("error: wrong command");
	uart_printstr("\r\n");
	eeprom_debug_print_range(0, 0x60);
}

ISR(USART_RX_vect)
{
	char c;
	static char buf[BUFFER_SIZE] = {};
	static uint16_t i = 0;

	c = uart_rx();

	if (c == '\r') {
		exec_cmd(buf);
		avr_memset(buf, 0, sizeof(buf));
		uart_printstr("> ");
		i = 0;
	} else if (c == 127) {
		if (i > 0) {
			buf[--i] = '\0';
			uart_printstr("\b \b");
		}
	} else if (i < BUFFER_SIZE - 1) {
		buf[i++] = c;
		uart_tx(c);
	}
}

void avr_memset(void *ptr, char c, uint16_t len)
{
	uint16_t i = 0;

	while (i < len)
		((char *)ptr)[i++] = c;
}

int8_t avr_strncmp(const char *s1, const char *s2, uint16_t n)
{
	uint16_t i = 0;

	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		++i;
	if (i == n)
		return 0;
	return s1[i] - s2[i];
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, (1 << RXCIE0));

	/*
	 * See https://arduino.stackexchange.com/questions/86403/unos-internal-eeprom-read-returns-incorrect-values-when-it-precedes-write
	 * to understand the delay
	 */
	_delay_ms(3000);
	eeprom_debug_print_range(0, 0x60);
	uart_printstr("Welcome!\r\n> ");
	while (1) {}
}