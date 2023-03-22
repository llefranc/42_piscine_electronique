/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 20:18:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/22 21:15:37 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"
#include "uart.h"

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
	addr += M_NUM_SIZE + sizeof(uint16_t);
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
 * Write data associated to an id to the EEPROM. Return an error if a data
 * with this id is already present in EEPROM or if there is not enough storage.
*/
int8_t eeprom_malloc_write(uint16_t id, void *buf, uint16_t len)
{
	uint16_t next_blk = 0;
	uint16_t write_addr = EEPROM_SIZE;
	uint16_t addr = 0;
	uint8_t *tmp = (uint8_t *)buf;
	uint16_t total_size = M_NUM_SIZE + sizeof(id) + sizeof(len) + len;

	if (!len)
		return -1;
	while ((addr + total_size) <= EEPROM_SIZE) {
		if (eeprom_is_m_num(addr)) {
			if (eeprom_read_word(addr + M_NUM_SIZE) == id)
				return -3;
			addr = eeprom_malloc_jump_after_block(addr);
		} else {
			next_blk = eeprom_find_next_block(addr);
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
	eeprom_write_word(write_addr, id);
	write_addr += sizeof(id);
	eeprom_write_word(write_addr, len);
	write_addr += sizeof(len);

	for (uint16_t i = 0; i < len; ++i)
		eeprom_write_byte(write_addr + i, tmp[i]);
	return 0;
}

/**
 * Read data associated to an id from the EEPROM and store it in a buffer.
 * Return an error if the id matches no data in EEPROM.
*/
int8_t eeprom_malloc_read(uint16_t id, void *buf, uint16_t len)
{
	uint16_t addr = 0;
	uint16_t block_len = 0;
	uint8_t *tmp = (uint8_t *)buf;

	while (addr != EEPROM_SIZE) {
		if (eeprom_is_m_num(addr)) {
			if (eeprom_read_word(addr + M_NUM_SIZE) == id)
				break;
			addr = eeprom_malloc_jump_after_block(addr);
		} else {
			addr = eeprom_find_next_block(addr);
		}
	}
	if (addr >= EEPROM_SIZE)
		return -1;

	addr += M_NUM_SIZE + sizeof(id);
	block_len = eeprom_read_word(addr);
	if (len > block_len)
		len = block_len;
	addr += sizeof(len);

	for (uint16_t i = 0; i < len; ++i)
		tmp[i] = eeprom_read_byte(addr + i);
	return block_len;
}

/**
 * Free data associated to an id in EEPROM.
*/
int8_t eeprom_malloc_free(uint16_t id)
{
	uint16_t addr = 0;

	while (1) {
		if (!eeprom_is_m_num(addr))
			addr = eeprom_find_next_block(addr);
		if (addr >= EEPROM_SIZE)
			break;
		if (eeprom_read_word(addr + M_NUM_SIZE) == id) {
			eeprom_memset(addr, M_NUM_SIZE);
			return 0;
		} else {
			addr = eeprom_malloc_jump_after_block(addr);
		}
	}
	return -1;
}