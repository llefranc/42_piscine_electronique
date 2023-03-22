/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:54:11 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/22 21:12:25 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	uart_init(UART_UBRRN, (0 << RXCIE0));

	/*
	 * To handle a weird behavior of EEPROM. Tests doesn't work without
	 * this delay, data in EEPROM seems correct the read operations followed
	 * by a UART transmission to computer outputs weird stuff, like the
	 * read/write operation were desynchronized. A delay of 1 sec fix this
	 * problem, it's some kind of black magic apparently
	 */
	// _delay_ms(1000);

	/**
	 * To trigger the bug :
	 * 1. Flash the AVR with only the memset instruction (no other write)
	 * 2. Comment the memset instruction, and do the write/read in a row
	 * 3. First eeprom_debug_print_range() should ouput an empty EEPROM
	 *    but EEPROM is already filled with data from the writes.
	*/
	// eeprom_memset(0, 0x40);
	uart_printstr("Starting tests\r\n");

	eeprom_debug_print_range(0, 0x40);

	char buf[] = "AAA asdasdaskudaksdjak  sd ad  \r\n";
	if (eeprom_malloc_write(4, buf, sizeof(buf)) == 0) {
		uart_printstr("write success\r\n");
	} else {
		uart_printstr("write failed\r\n");
	}
	eeprom_debug_print_range(0, 0x40);

	char buf2[] = "BBBBB dakudhjasdhahj adsdas\r\n";
	if (eeprom_malloc_write(5, buf2, sizeof(buf2)) == 0) {
		uart_printstr("write success\r\n");
	} else {
		uart_printstr("write failed\r\n");
	}
	eeprom_debug_print_range(0, 0x40);

	char buf3[] = "Cdasjkldalsdhjasd as das d\r\n";
	if (eeprom_malloc_write(6, buf3, sizeof(buf3)) == 0) {
		uart_printstr("write success\r\n");
	} else {
		uart_printstr("write failed\r\n");
	}
	eeprom_debug_print_range(0, 0x40);

	if (eeprom_malloc_free(4) == -1)
		uart_printstr("free failed\r\n");
	// if (eeprom_malloc_free(5) == -1)
	// 	uart_printstr("free failed\r\n");
	if (eeprom_malloc_free(6) == -1)
		uart_printstr("free failed\r\n");

	char bufr[30] = {};
	if (eeprom_malloc_read(4, bufr, EEPROM_SIZE) >= 0)
		uart_printstr(bufr);
	else
		uart_printstr("read failed\r\n");

	if (eeprom_malloc_read(5, bufr, EEPROM_SIZE) >= 0)
		uart_printstr(bufr);
	else
		uart_printstr("read failed\r\n");

	if (eeprom_malloc_read(6, bufr, EEPROM_SIZE) >= 0)
		uart_printstr(bufr);
	else
		uart_printstr("read failed\r\n");

	eeprom_debug_print_range(0, 0x40);
	// eeprom_debug_print_all();

	// TESTS MALLOC_GET_NEXT_BLOCK
	// int i = 0;
	// eeprom_write_dword(0, M_NUM);
	// eeprom_write_word(4, 12);
	// eeprom_write_word(6, 30);
	// eeprom_write_dword(38, M_NUM);

	// uart_tx((char)eeprom_malloc_get_find_block(0));
	// if (eeprom_read_dword(eeprom_malloc_get_find_block(0)) == M_NUM)
	// 	uart_printstr(" ok\r\n");
	// else
	// 	uart_printstr(" not ok\r\n");

	// if (eeprom_read_dword(eeprom_malloc_get_find_block(0)) == eeprom_read_dword(38))
	// 	uart_printstr(" ok\r\n");
	// else
	// 	uart_printstr(" not ok\r\n");



	// TESTS IS_FREE_SPACE
	// int i = 0;
	// eeprom_write_dword(0, M_NUM);
	// uart_tx(i++ + '0');
	// if (!eeprom_is_free_space(0, 4))
	// 	uart_printstr(" not enough space\r\n");
	// else
	// 	uart_printstr(" enough space\r\n");
	// eeprom_memset(0, 50);

	// eeprom_write_dword(1, M_NUM);
	// uart_tx(i++ + '0');
	// if (!eeprom_is_free_space(0, 4))
	// 	uart_printstr(" not enough space\r\n");
	// else
	// 	uart_printstr(" enough space\r\n");
	// eeprom_memset(0, 50);

	// eeprom_write_dword(2, M_NUM);
	// uart_tx(i++ + '0');
	// if (!eeprom_is_free_space(0, 4))
	// 	uart_printstr(" not enough space\r\n");
	// else
	// 	uart_printstr(" enough space\r\n");
	// eeprom_memset(0, 50)

	// eeprom_write_dword(3, M_NUM);
	// uart_tx(i++ + '0');
	// if (!eeprom_is_free_space(0, 4))
	// 	uart_printstr(" not enough space\r\n");
	// else
	// 	uart_printstr(" enough space\r\n");
	// eeprom_memset(0, 50);

	// eeprom_write_dword(4, M_NUM);
	// uart_tx(i++ + '0');
	// if (!eeprom_is_free_space(0, 4))
	// 	uart_printstr(" not enough space\r\n");
	// else
	// 	uart_printstr(" enough space\r\n");
	// eeprom_memset(0, 50);

	// eeprom_write_dword(7, M_NUM);
	// uart_tx(i++ + '0');
	// if (!eeprom_is_free_space(0, 8))
	// 	uart_printstr(" not enough space\r\n");
	// else
	// 	uart_printstr(" enough space\r\n");
	// eeprom_memset(0, 50);

	// eeprom_write_dword(8, M_NUM);
	// uart_tx(i++ + '0');
	// if (!eeprom_is_free_space(0, 8))
	// 	uart_printstr(" not enough space\r\n");
	// else
	// 	uart_printstr(" enough space\r\n");
	// eeprom_memset(0, 50);





	// TESTS READ WRITE WORDS
	// uint8_t wbyte = 8;
	// uint16_t wword = 1000;
	// uint32_t wdword = 125000;

	// uint8_t rbyte = 0;
	// uint16_t rword = 0;
	// uint32_t rdword = 0;
	// eeprom_write_byte(0, wbyte);
	// eeprom_write_word(1, wword);
	// eeprom_write_dword(3, wdword);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");

	// eeprom_write_byte(0, wbyte++);
	// eeprom_write_word(1, wword++);
	// eeprom_write_dword(3, wdword++);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");


	// eeprom_write_byte(0, wbyte++);
	// eeprom_write_word(1, wword++);
	// eeprom_write_dword(3, wdword++);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");

	// eeprom_write_byte(0, wbyte++);
	// eeprom_write_word(1, wword++);
	// eeprom_write_dword(3, wdword++);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");


	// eeprom_write_byte(0, wbyte++);
	// eeprom_write_word(1, wword++);
	// eeprom_write_dword(3, wdword++);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");


	// eeprom_write_byte(0, wbyte++);
	// eeprom_write_word(1, wword++);
	// eeprom_write_dword(3, wdword++);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");


	// eeprom_write_byte(0, wbyte++);
	// eeprom_write_word(1, wword++);
	// eeprom_write_dword(3, wdword++);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");


	// eeprom_write_byte(0, wbyte++);
	// eeprom_write_word(1, wword++);
	// eeprom_write_dword(3, wdword++);

	// rbyte = eeprom_read_byte(0);
	// rword = eeprom_read_word(1);
	// rdword = eeprom_read_dword(3);

	// if (rbyte == wbyte)
	// 	uart_printstr("eeprom byte ok\r\n");
	// if (rword == wword)
	// 	uart_printstr("eeprom word ok\r\n");
	// if (rdword == wdword)
	// 	uart_printstr("eeprom dword ok\r\n");
	eeprom_debug_print_range(0, 0x40);

	while (1) {}
}