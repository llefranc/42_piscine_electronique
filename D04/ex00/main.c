/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:08:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/19 14:05:48 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "uart.h"
#include "i2c.h"

int main(void)
{
	sei();
	uart_init(UART_UBRRN, (0 << RXCIE0));
	i2c_init();
	i2c_start();
	i2c_stop();
	while (1);
}