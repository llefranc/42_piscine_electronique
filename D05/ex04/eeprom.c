/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 20:18:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/23 16:52:18 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"
#include "uart.h"
#include "utils.h"

#include <avr/interrupt.h>

/**
 * Read a byte at a specified address in EEPROM.
*/
uint8_t eeprom_read_byte(uint16_t addr)
{
	/* Waiting if there is a write eeprom in progress */
	while (EECR & (1 << EEPE));
	EEAR = addr;
	EECR |= (1 << EERE);
	return EEDR;
}

/**
 * Read a word at a specified address in EEPROM (little endian).
*/
uint16_t eeprom_read_word(uint16_t addr)
{
	uint16_t word = 0;

	word |= (uint16_t)eeprom_read_byte(addr);
	word |= (uint16_t)eeprom_read_byte(addr + 1) << 8;
	return word;
}

/**
 * Read a dword at a specified address in EEPROM (little endian).
*/
uint32_t eeprom_read_dword(uint16_t addr)
{
	uint32_t dword = 0;

	dword |= (uint32_t)eeprom_read_byte(addr);
	dword |= (uint32_t)eeprom_read_byte(addr + 1) << 8;
	dword |= (uint32_t)eeprom_read_byte(addr + 2) << 16;
	dword |= (uint32_t)eeprom_read_byte(addr + 3) << 24;
	return dword;
}

/**
 * Write a byte at a specified address in EEPROM.
*/
void eeprom_write_byte(uint16_t addr, uint8_t data)
{
	/* Avoiding to write byte if already stored in EEEPROM */
	if (eeprom_read_byte(addr) != data) {

		/* Waiting if there is already a write eeprom in progress */
		while (EECR & (1 << EEPE));

		/* Deactivating interrupts to protect eeprom write */
		cli();
		EEAR = addr;
		EEDR = data;
		EECR |= (1 << EEMPE);
		EECR |= (1 << EEPE);
		sei();
	}
}

/**
 * Write a word at a specified address in EEPROM (little endian).
*/
void eeprom_write_word(uint16_t addr, uint16_t data)
{
	eeprom_write_byte(addr, (uint8_t)data);
	eeprom_write_byte(addr + 1, (uint8_t)(data >> 8));
}

/**
 * Write a dword at a specified address in EEPROM (little endian).
*/
void eeprom_write_dword(uint16_t addr, uint32_t data)
{
	eeprom_write_byte(addr, (uint8_t)data);
	eeprom_write_byte(addr + 1, (uint8_t)(data >> 8));
	eeprom_write_byte(addr + 2, (uint8_t)(data >> 16));
	eeprom_write_byte(addr + 3, (uint8_t)(data >> 24));
}

static inline void eeprom_write_m_num(uint16_t addr)
{
	eeprom_write_dword(addr, M_NUM_DWORD);
}

/**
 * Read the size of the current block and jump after it.
*/
uint16_t eeprom_malloc_jump_after_block(uint16_t addr)
{
	addr += M_NUM_SIZE;
	/* Reading size of id */
	addr += eeprom_read_word(addr) + sizeof(uint16_t);
	/* Reading size of data */
	return addr + sizeof(uint16_t) + eeprom_read_word(addr);
}

/**
 * Return the address of the next block allocated, or EEPROM_SIZE if there
 * is not other block allocated.
*/
uint16_t eeprom_find_next_block(uint16_t addr)
{
	uint16_t start = addr;

	while (addr < EEPROM_SIZE) {
		if (eeprom_read_byte(addr) == M_NUM_BYTE) {
			while (eeprom_read_byte(addr - 1) == M_NUM_BYTE)
				--addr;
			if (start < addr && eeprom_is_m_num(addr))
				return addr;
		}
		addr += M_NUM_SIZE;
	}
	return EEPROM_SIZE;
}

/**
 * Print to a computer a range of bytes stored in the EEPROM using UART
 * protocol through USB.
*/
void eeprom_debug_print_range(uint16_t start, uint16_t end)
{
	uint16_t nb_print = 0;

	if (end > EEPROM_SIZE)
		end = EEPROM_SIZE;
	uart_printstr("------------- EEPROM -------------");
	while (start < end) {
		if (nb_print % 16 == 0) {
			uart_printstr("\r\n0x");
			uart_print_hex_value((uint8_t)(start >> 8));
			uart_print_hex_value((uint8_t)(start));
			uart_printstr(": ");
		}
		uart_print_hex_value(eeprom_read_byte(start++));
		uart_tx(' ');
		nb_print++;
	}
	uart_printstr("\r\n------------- EEPROM -------------\r\n");
}

/**
 * Check if an id corresponds to another one stored at a specific address in
 * EEPROM.
*/
int8_t eeprom_is_same_id(char *buf, uint16_t addr, uint16_t id_size)
{
	uint8_t i = 0;
	uint16_t eeprom_id_size =  eeprom_read_word(addr);

	if (id_size != eeprom_id_size)
		return 0;
	addr += 2;
	while (buf[i] && i < eeprom_id_size) {
		if (buf[i] != eeprom_read_byte(addr + i))
			return 0;
		++i;
	}
	return i == eeprom_id_size;
}

/**
 * Write to a specific address in EEPROM a word corresponding to the size of
 * a buffer + the buffer data.
*/
void eeprom_write_with_size(uint16_t addr, char *buf, uint16_t size)
{
	eeprom_write_word(addr, size);
	addr += sizeof(size);
	for (uint16_t i = 0; i < size; ++i)
		eeprom_write_byte(addr + i, buf[i]);
}

/**
 * Write data associated to an id to the EEPROM. Return an error if a data
 * with this id is already existing in EEPROM or if there is not enough storage.
*/
int8_t eeprom_malloc_write(char *buf, uint16_t id_size)
{
	char *data = get_next_token(buf);
	/* + 1 To store '\0' */
	uint16_t data_size = count_chars_until_sep(data, '\0') + 1;
	uint16_t total_size = M_NUM_SIZE + sizeof(id_size) + id_size +
			      sizeof(data_size) + data_size;
	uint16_t addr = 0;
	uint16_t next_blk = 0;
	uint16_t write_addr = EEPROM_SIZE;

	while ((addr + total_size) <= EEPROM_SIZE) {
		if (eeprom_is_m_num(addr)) {
			if (eeprom_is_same_id(buf, addr + M_NUM_SIZE, id_size))
				return -1;
			addr = eeprom_malloc_jump_after_block(addr);
		} else {
			next_blk = eeprom_find_next_block(addr);
			/*
			* Not breaking the loop because even if we find a
			* correct spot to write, we still need to check if id
			* already exists in EEPROM
			*/
			if (write_addr == EEPROM_SIZE &&
			   (next_blk - addr) >= total_size)
				write_addr = addr;
			addr = next_blk;
		}
	}
	if (write_addr == EEPROM_SIZE)
		return -2;

	eeprom_write_m_num(write_addr);
	write_addr += M_NUM_SIZE;
	eeprom_write_with_size(write_addr, buf, id_size);
	write_addr += sizeof(id_size) + id_size;
	eeprom_write_with_size(write_addr, data, data_size);
	return 0;
}

/**
 * Read data associated to an id from the EEPROM and store it in a buffer.
 * Return an error if the id matches no data in EEPROM.
*/
int8_t eeprom_malloc_read(char *buf, uint16_t id_size, uint16_t len)
{
	uint16_t addr = 0;
	uint16_t block_len = 0;

	while (addr != EEPROM_SIZE) {
		if (eeprom_is_m_num(addr)) {
			if (eeprom_is_same_id(buf, addr + M_NUM_SIZE, id_size))
				break;
			addr = eeprom_malloc_jump_after_block(addr);
		} else {
			addr = eeprom_find_next_block(addr);
		}
	}
	if (addr >= EEPROM_SIZE)
		return -1;

	addr += M_NUM_SIZE;
	addr +=  sizeof(id_size) + eeprom_read_word(addr);
	block_len = eeprom_read_word(addr);
	if (len > block_len)
		len = block_len;
	addr += sizeof(id_size); /* Correspond to data size word*/

	for (uint16_t i = 0; i < len; ++i)
		buf[i] = eeprom_read_byte(addr + i);
	return block_len;
}

/**
 * Free data associated to an id in EEPROM.
*/
int8_t eeprom_malloc_free(char *buf, uint16_t id_size)
{
	uint16_t addr = 0;

	while (1) {
		if (!eeprom_is_m_num(addr))
			addr = eeprom_find_next_block(addr);
		if (addr >= EEPROM_SIZE)
			break;
		if (eeprom_is_same_id(buf, addr + M_NUM_SIZE, id_size)) {
			eeprom_memset(addr, M_NUM_SIZE);
			return 0;
		} else {
			addr = eeprom_malloc_jump_after_block(addr);
		}
	}
	return -1;
}