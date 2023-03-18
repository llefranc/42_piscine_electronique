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

#include "uart.h"

#define UBRRN F_CPU/8/UART_BAUDRATE-1

#define TWBR_VAL 18
#define I2C_PRESCALER 4
#define F_SCL = F_CPU/(16+2*TWBR_VAL*I2C_PRESCALER)

// 16 000 000 / 100 000 = 160

// 160 =  16 + 2 * (TWBR) * PRESCALER
// 144 =  2 * (TWBR) * PRESCALER
// 72 = (TWBR) * PRESCALER
// 72 = TWBR * 4
// 18 = TWBR

int main(void)
{
	sei();
	uart_init(UBRRN, (0 << RXCIE0));
	uart_printstr("Salut\r\n");

	while (1);
}