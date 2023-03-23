/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:56:55 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/23 14:58:06 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/**
 * Return a pointer to the next token in a string.
*/
char* get_next_token(char *buf)
{
	uint8_t i = 0;

	while (buf[i] && buf[i] != ' ')
		++i;
	while (buf[i] && buf[i] == ' ')
		++i;
	return &buf[i];
}