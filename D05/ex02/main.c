/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:54:11 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 22:45:08 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	char buf[] = "salut ca va\r\n";
	char buf2[] = "";
	char buf3[] = "yes ca va bien\r\n";
	char testbuf[30] = {};

	uart_init(UART_UBRRN, (0 << RXCIE0));

	/* -------------- writing -------------- */
	if (eeprom_safe_write(buf, 0, sizeof(buf)) == -1)
		uart_printstr("eeprom_safe_write buf failed\r\n");
	if (eeprom_safe_write(buf2, sizeof(buf) + 4, sizeof(buf2)) == -1)
		uart_printstr("eeprom_safe_write buf2 failed\r\n");
	if (eeprom_safe_write(buf3, sizeof(buf) + 4 + sizeof(buf2) + 4, sizeof(buf3)) == -1)
		uart_printstr("eeprom_safe_write buf3 failed\r\n");

	/* Write without magic number, safe_read at this address will fail */
	eeprom_write(sizeof(buf) + 4 + sizeof(buf2) + 4 + sizeof(buf3) + 4, 10);


	/* -------------- reading -------------- */
	if (eeprom_safe_read(testbuf, 0, sizeof(buf)) == -1)
		uart_printstr("eeprom_safe_read failed\r\n");
	uart_printstr("\r\n-> Printing buffer: ");
	uart_printstr(testbuf);

	if (eeprom_safe_read(testbuf, sizeof(buf) + 4, sizeof(buf2)) == -1)
			uart_printstr("eeprom_safe_read failed\r\n");
	uart_printstr("\r\n-> Printing buffer: ");
	uart_printstr(testbuf);

	if (eeprom_safe_read(testbuf, sizeof(buf) + 4 + sizeof(buf2) + 4, sizeof(buf3)) == -1)
			uart_printstr("eeprom_safe_read failed\r\n");
	uart_printstr("\r\n-> Printing buffer: ");
	uart_printstr(testbuf);

	if (eeprom_safe_read(testbuf, sizeof(buf) + 4 + sizeof(buf2) + 4 + sizeof(buf3) + 4, 1) == -1)
		uart_printstr("eeprom_safe_read failed because wrong magic number\r\n");

	while (1) {}
}