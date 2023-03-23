/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 20:18:59 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/23 16:43:38 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#define M_NUM_DWORD 0xF9F9F9F9L
#define M_NUM_BYTE 0xF9
#define M_NUM_SIZE 4

#define EEPROM_SIZE 1024

uint8_t eeprom_read_byte(uint16_t addr);
uint16_t eeprom_read_word(uint16_t addr);
uint32_t eeprom_read_dword(uint16_t addr);
void eeprom_write_byte(uint16_t addr, uint8_t data);
void eeprom_write_word(uint16_t addr, uint16_t data);
void eeprom_write_dword(uint16_t addr, uint32_t data);

uint16_t eeprom_malloc_jump_after_block(uint16_t addr);
uint16_t eeprom_find_next_block(uint16_t addr);

int8_t eeprom_malloc_write(char *buf, uint16_t id_size);
int8_t eeprom_malloc_read(char *buf, uint16_t id_size, uint16_t len);
int8_t eeprom_malloc_free(char *buf, uint16_t id_size);

void eeprom_debug_print_range(uint16_t start, uint16_t end);

static inline void eeprom_memset(uint16_t addr, uint16_t len)
{
	for (uint16_t i = 0; i < len; ++i)
		eeprom_write_byte(addr + i, 0);
}

static inline int8_t eeprom_is_m_num(uint16_t addr)
{
	return eeprom_read_dword(addr) == M_NUM_DWORD;
}

static inline void eeprom_debug_print_all(void)
{
	eeprom_debug_print_range(0, EEPROM_SIZE);
}

#endif /* EEPROM_H */