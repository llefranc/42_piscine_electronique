/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avr_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:32:50 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/29 17:40:50 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "avr_string.h"

int8_t avr_strcmp(const char *s1, const char *s2)
{
	uint16_t i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		++i;
	return s1[i] - s2[i];
}

void avr_memset(void *ptr, char c, uint16_t len)
{
	uint16_t i = 0;

	while (i < len)
		((char *)ptr)[i++] = c;
}

/**
 * Ascii to unsigned integer.
*/
int16_t avr_atoui(char *buf, uint8_t n)
{
	int16_t tmp = 0;

	for (uint8_t i = 0; buf[i] && i < n; ++i) {
		if (!avr_isnum(buf[i]))
			return -1;
		tmp *= 10;
		tmp += buf[i] - '0';
	}
	return tmp;
}