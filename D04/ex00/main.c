/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:08:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/18 20:38:16 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "uart.h"
#include "i2c.h"

ISR(TWI_vect)
{
	/* Sending TWI status register value after each TWI operation */
	uart_tx((char)(TWSR & TW_STATUS_MASK) + '0');
	uart_tx(' ');
	i2c_stop();
}

int main(void)
{
	sei();
	uart_init(UART_UBRRN, (0 << RXCIE0));
	i2c_init();
	i2c_start();
	while (1);
}