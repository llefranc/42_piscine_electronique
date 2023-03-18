/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 18:28:54 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/18 20:33:01 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
#define UART_H

#include <avr/io.h>

void uart_init(uint16_t ubbrn, uint8_t rxcie0);
void uart_tx(char c);
void uart_printstr(const char* str);
char uart_rx(void);

#endif /* UART_H */