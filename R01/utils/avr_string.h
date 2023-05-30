/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avr_string.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:33:01 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/29 17:33:49 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVR_STRING_H
#define AVR_STRING_H

#include <avr/io.h>

static inline uint8_t avr_isnum(char c)
{
	return (c >= '0' && c <= '9');
}

int8_t avr_strcmp(const char *s1, const char *s2);
void avr_memset(void *ptr, char c, uint16_t len);
int16_t avr_atoui(char *buf, uint8_t n);

#endif /* AVR_STRING_H */