/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 20:18:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 22:37:11 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"

#include <avr/interrupt.h>

uint8_t eeprom_read(uint16_t addr)
{
	while (EECR & (1 << EEPE)); /* waiting if there is write eeprom op */
	EEAR = addr;
	EECR |= (1 << EERE); /* triggers eeprom read */
	return EEDR;
}

int8_t eeprom_is_m_num_ok(uint16_t addr)
{
	uint32_t eeprom_magic_nb_read = 0;

	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++) << 24;
	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++) << 16;
	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++) << 8;
	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++);
	return eeprom_magic_nb_read == M_NUM;
}

void eeprom_write(uint16_t addr, uint8_t data)
{
	while (EECR & (1 << EEPE)); /* waiting if there is write eeprom op */
	cli();                     /* deactivates interrupt to protect eeprom write */
	EEAR = addr;
	EEDR = data;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
	sei();
}

void eeprom_write_m_num(uint16_t addr)
{
	eeprom_write(addr, M_NUM_BYTE);
	eeprom_write(addr + 1, M_NUM_BYTE);
	eeprom_write(addr + 2, M_NUM_BYTE);
	eeprom_write(addr + 3, M_NUM_BYTE);
}

int8_t eeprom_safe_read(void *buf, uint16_t addr, uint16_t len)
{
	uint8_t *tmp = buf;

	if (!eeprom_is_m_num_ok(addr))
		return -1;
	addr += M_NUM_LEN;
	for (uint16_t i = 0; i < len; ++i)
		tmp[i] = eeprom_read(addr + i);
	return 0;
}

int8_t eeprom_safe_write(void *buf, uint16_t addr, uint16_t len)
{
	uint8_t *tmp = buf;

	eeprom_write_m_num(addr);
	addr += M_NUM_LEN;
	for (uint16_t i = 0; i < len; ++i) {
		if (eeprom_read(addr + i) == tmp[i])
			continue;
		eeprom_write(addr + i, tmp[i]);
	}
	return 0;
}