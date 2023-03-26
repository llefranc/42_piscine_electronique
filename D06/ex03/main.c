/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:12:46 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 18:46:50 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define R_LED (1 << PD5)
#define G_LED (1 << PD6)
#define B_LED (1 << PD3)

#define BUFFER_SIZE 8
#define SHIFT_WIDTH 4

enum e_timer_dir {
	E_UPCOUNTING,
	E_DOWNCOUNTING
};

enum e_timer_dir timer0_dir = E_UPCOUNTING;
enum e_timer_dir timer2_dir = E_UPCOUNTING;

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	/* Deactivating timers interrupt when changing duty cycle registers */
	cli();

	TCCR0A |= (1 << WGM00); /* PWM phase correct mode with TOP value 0xFF */
	TCCR0B |= (1 << CS02);  /* starting timer0 with prescaler 64 */
	TIMSK0 |= (1 << OCIE0B) | (1 << OCIE0A) | (1 << TOIE0);

	TCCR2A |= (1 << WGM20); /* PWM phase correct mode with TOP value 0xFF */
	TCCR2B |= (1 << CS22);  /* starting timer2 with prescaler 64 */
	TIMSK2 |= (1 << OCIE2A) | (1 << TOIE2);

	OCR0A = r;
	OCR0B = g;
	OCR2A = b;

	TCNT0 = 0;
	TCNT2 = 0;
	sei();
}

/* Occurs at BOTTOM */
ISR(TIMER0_OVF_vect)
{
	timer0_dir = E_UPCOUNTING;
}

/* Occurs at BOTTOM */
ISR(TIMER2_OVF_vect)
{
	timer2_dir = E_UPCOUNTING;
}

/* Red duty cycle */
ISR(TIMER0_COMPA_vect)
{
	if (!OCR0A)
		PORTD &= ~R_LED;
	else if (OCR0A == 0xFF)
		PORTD |= R_LED;
	else if (timer0_dir == E_UPCOUNTING)
		PORTD &= ~R_LED;
	else
		PORTD |= R_LED;
	timer0_dir = E_DOWNCOUNTING;
}

/* Green duty cycle */
ISR(TIMER0_COMPB_vect)
{
	if (!OCR0B)
		PORTD &= ~G_LED;
	else if (OCR0B == 0xFF)
		PORTD |= G_LED;
	else if (timer0_dir == E_UPCOUNTING)
		PORTD &= ~G_LED;
	else
		PORTD |= G_LED;
	timer0_dir = E_DOWNCOUNTING;
}

/* Blue duty cycle */
ISR(TIMER2_COMPA_vect)
{
	if (!OCR2A)
		PORTD &= ~B_LED;
	else if (OCR2A == 0xFF)
		PORTD |= B_LED;
	else if (timer2_dir == E_UPCOUNTING)
		PORTD &= ~B_LED;
	else
		PORTD |= B_LED;
	timer2_dir = E_DOWNCOUNTING;
}

static inline uint8_t is_alnum(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

int8_t char_to_hex(char c)
{
	static const char *hex = "0123456789ABCDEF";
	uint8_t i = 0;

	while (hex[i] && hex[i] != c)
		++i;
	if (!hex[i])
		return -1;
	return i;
}

void avr_memset(void *ptr, char c, uint16_t len)
{
	uint16_t i = 0;

	while (i < len)
		((char *)ptr)[i++] = c;
}

static inline uint8_t is_color_code_ok(char *buf)
{
	uint8_t i = 0;

	if (buf[i++] != '#')
		return 0;
	while (buf[i]) {
		if (!is_alnum(buf[i]))
			return 0;
		++i;
	}
	return i == BUFFER_SIZE - 1;
}

static inline uint32_t get_rgb_code(char *buf)
{
	uint32_t nb_hex;
	uint32_t rgb = 0;

	for (uint8_t i = 1; i < BUFFER_SIZE - 1; ++i) {
		nb_hex = char_to_hex(buf[i]);
		rgb |= nb_hex << ((BUFFER_SIZE - 1 - i) * SHIFT_WIDTH);
	}
	return rgb;
}

ISR(USART_RX_vect)
{
	static char buf[BUFFER_SIZE] = {};
	static uint32_t rgb;
	static uint8_t i = 0;
	char c;

	c = uart_rx();
	if (c == '\r') {
		if (!is_color_code_ok(buf))
			goto err_parsing;
		rgb = get_rgb_code(buf);
		set_rgb((uint8_t)(rgb >> 16), (uint8_t)(rgb >> 8), (uint8_t)rgb);
		uart_printstr("\r\nRGB LED updated!\r\n");
		uart_printstr("Enter a hexadecimal color code to modify the "
			      "RGB LED:\r\n");
		goto reset_buf;
	} else if (c == 127) {
		if (i > 0) {
			buf[--i] = '\0';
			uart_printstr("\b \b");
		}
	} else if (i < BUFFER_SIZE - 1) {
		buf[i++] = c;
		uart_tx(c);
	}
	return;

err_parsing:
	uart_printstr("\r\nError: please respect the color code format "
		      "#RRGGBB (caps only)\r\n");
reset_buf:
	i = 0;
	rgb = 0;
	avr_memset(buf, 0, sizeof(buf));
}

int main(void)
{
	sei();
	DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
	uart_init(UART_UBRRN, (1 << RXCIE0));
	uart_printstr("Welcome!");
	uart_printstr("\r\nEnter a hexadecimal color code (#RRGGBB) to modify "
		      "the RGB LED:\r\n");
	while (1);
}
