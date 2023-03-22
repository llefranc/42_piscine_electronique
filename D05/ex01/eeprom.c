/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 20:18:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/21 14:24:02 by llefranc         ###   ########.fr       */
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

int8_t eeprom_is_m_num(uint16_t addr, uint32_t magic_nb)
{
	uint32_t eeprom_magic_nb_read = 0;

	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++) << 24;
	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++) << 16;
	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++) << 8;
	eeprom_magic_nb_read |= (uint32_t)eeprom_read(addr++);
	return eeprom_magic_nb_read == magic_nb;
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