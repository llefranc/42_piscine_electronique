/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 20:18:59 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 20:34:51 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#define MAGIC_NUM 0xF9F9F9F9L
#define MAGIC_BYTE 0xF9
#define MAGIC_NUM_ADDR 0
#define MAGIC_NUM_LEN 4

uint8_t eeprom_read(uint16_t addr);
int8_t is_magic_ok(uint16_t addr, uint32_t real_magic);
void eeprom_write(uint16_t addr, uint8_t data);

#endif /* EEPROM)H */