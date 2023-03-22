/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:54:11 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/21 14:24:02 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define NB_TOTAL_COUNTERS 4
#define COUNT_NB_MAX_VALUE 8

static uint8_t count_index = 1;
static uint8_t count_nb = 0;

static void update_leds(uint8_t nb);
static void uart_print_counters(void);

ISR(INT0_vect) /* sw1 */
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed) {
		count_nb++;
		if (count_nb == COUNT_NB_MAX_VALUE)
			count_nb = 0;

		eeprom_write(COUNTERS_ADDR + M_NUM_SIZE + count_index, count_nb);
		update_leds(count_nb);
		uart_print_counters();
	}
	_delay_ms(20);                 /* Neutralizing bounce effect */
	if (EIFR & (1 << INTF0))       /* Clearing interrupt flag if another one occured while ISR was executing*/
		EIFR |= (1 << INTF0);

}

ISR(PCINT2_vect) /* sw2 */
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed) {
		count_index++;
		if (count_index == NB_TOTAL_COUNTERS + 1)
			count_index = 1;

		eeprom_write(COUNTERS_ADDR + M_NUM_SIZE, count_index);
		count_nb = eeprom_read(COUNTERS_ADDR + M_NUM_SIZE + count_index);
		update_leds(count_nb);
		uart_print_counters();
	}
	_delay_ms(20);                 /* Neutralizing bounce effect */
	if (PCIFR & (1 << PCIF2))      /* Clearing interrupt flag if another one occured while ISR was executing*/
		PCIFR |= (1 << PCIF2);
}

static inline void update_leds(uint8_t nb)
{
	PORTB &= ~((1 << PB2) | (1 << PB1) | (1 << PB0));
	PORTB |= nb;
}

static inline void init_buttons_intrq(void)
{
	sei();
	EIMSK |= (1 << INT0);  /* Enabling interrupt INT0 for SW1*/
	EICRA |= (1 << ISC00); /* Any logical change on INT0 generates an interrupt request. */

	PCICR |= (1 << PCIE2);    /* Enabling PCINT2 interrupt for PIN16-23 */
	PCMSK2 |= (1 << PCINT20); /* Only pin with PCINT20 can trigger PCINT2 (SW2) */
}

static void uart_print_counters(void)
{
	uart_printstr("counter ");
	uart_tx(count_index + '0');
	uart_printstr(" selected, value=");
	uart_tx(count_nb + '0');
	uart_printstr("\r\n");
}

int main(void)
{
	init_buttons_intrq();
	uart_init(UART_UBRRN, (0 << RXCIE0));
	DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB0);

	/* Initializing EEPROM only for first run of program */
	if (!eeprom_is_m_num(COUNTERS_ADDR, M_NUM)) {
		uart_printstr("Initializing EEPROM\r\n");
		eeprom_write_m_num(COUNTERS_ADDR);

		/* +1 for count_index byte*/
		eeprom_memset(COUNTERS_ADDR + M_NUM_SIZE, NB_TOTAL_COUNTERS + 1);
		eeprom_write(COUNTERS_ADDR + M_NUM_SIZE, count_index);
	} else {
		uart_printstr("EEPROM already initialized\r\n");

		count_index = eeprom_read(COUNTERS_ADDR + M_NUM_SIZE);
		count_nb = eeprom_read(COUNTERS_ADDR + M_NUM_SIZE + count_index);
	}

	uart_print_counters();
	update_leds(count_nb);
	while (1) {}
}