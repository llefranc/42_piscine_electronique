/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:40:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/23 15:00:34 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>

char* get_next_token(char *buf);

/**
 * Count characters from beginning until a separator or '\0' character.
*/
static inline uint8_t count_chars_until_sep(char *buf, char sep)
{
	uint8_t i = 0;

	while (buf[i] && buf[i] != sep)
		++i;
	return i;
}

#endif /* UTILS_H */