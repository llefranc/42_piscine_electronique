/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 20:18:59 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 21:56:29 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#define M_NUM 0xF9F9F9F9L
#define M_NUM_BYTE 0xF9
#define M_NUM_LEN 4

#define COUNTERS_ADDR 0x0000

uint8_t eeprom_read(uint16_t addr);
int8_t eeprom_is_m_num_ok(uint16_t addr, uint32_t magic_nb);
void eeprom_write(uint16_t addr, uint8_t data);
void eeprom_write_m_num(uint16_t addr);

static inline void eeprom_erase(uint16_t addr, uint16_t len)
{
	for (uint16_t i = 0; i < len; ++i)
		eeprom_write(addr + i, 0);
}

#endif /* EEPROM)H */