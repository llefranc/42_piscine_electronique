/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:21:50 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/26 18:47:12 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1_PRESCALER 1024
#define FREQ_COLOR_CHANGE 64 /* 64Hz, we change color 64 times per sec */

#define BUFFER_SIZE 13
#define SHIFT_WIDTH 4

#define RGB_START_INDEX 1
#define RGB_END_INDEX 7

void wheel(uint8_t pos)
{
	uint32_t color = 0;

	pos = 255 - pos;
	if (pos < 85) {
		color |= (uint32_t)(255 - pos * 3) << 16;
		color |= (uint32_t)(pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		color |= (uint32_t)(pos * 3) << 8;
		color |= (uint32_t)(255 - pos * 3);
	} else {
		pos = pos - 170;
		color |= (uint32_t)(pos * 3) << 16;
		color |= (uint32_t)(255 - pos * 3) << 8;
	}
	spi_set_leds(B_LOW | color, B_LOW | color, B_LOW | color);
}

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

static inline uint8_t retrieve_led_index(char *buf)
{
	return buf[RGB_END_INDEX + 1] - '6';
}

static inline void update_color(char *buf, uint32_t *colors)
{
	uint32_t nb_hex;
	uint32_t rgb = 0;

	for (uint8_t i = RGB_START_INDEX; i < RGB_END_INDEX; ++i) {
		nb_hex = char_to_hex(buf[i]);
		rgb |= nb_hex << ((RGB_END_INDEX - 1 - i) * SHIFT_WIDTH);
	}
	colors[retrieve_led_index(buf)] = rgb;
}

static inline void timer1_init(void)
{
	OCR1A = F_CPU / TIMER1_PRESCALER / FREQ_COLOR_CHANGE;
	TCCR1B |= (1 << WGM12); /* CTC mode */
	TIMSK1 |= (1 << OCIE1A);
}

static inline void timer1_start(void)
{
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
}

static inline void timer1_stop(void)
{
	TCCR1B &= ~((1 << CS12) | (1 << CS10));
}

static inline uint8_t is_color_code_ok(char *buf)
{
	uint8_t i = 0;

	if (buf[i++] != '#')
		return 0;
	while (buf[i] && i < RGB_END_INDEX) {
		if (!is_alnum(buf[i]))
			return 0;
		++i;
	}
	if (i != RGB_END_INDEX)
		return 0;
	if (buf[i] != 'D' || !(buf[i + 1] >= '6' && buf[i + 1] <= '8'))
		return 0;
	i += 2;
	while (buf[i]) {
		if (buf[i++] != ' ')
			return 0;
	}
	return 1;
}

ISR(USART_RX_vect)
{
	char c;
	static uint8_t i = 0;
	static char buf[BUFFER_SIZE] = {};
	static uint32_t colors[3] = {};

	c = uart_rx();
	if (c == '\r') {
		if (!avr_strcmp(buf, "#FULLRAINBOW")) {
			timer1_start();
			uart_printstr("\r\nSecret mode activated!");
		} else if (is_color_code_ok(buf)) {
			timer1_stop();
			update_color(buf, colors);
			spi_set_leds(B_LOW | colors[0], B_LOW | colors[1],
				     B_LOW | colors[2]);
			uart_printstr("\r\nRGB LED updated!");
		} else {
			goto err_parsing;
		}
		uart_printstr("\r\nEnter a hexadecimal color code (#RRGGBBDx) "
			      "to modify the RGB LED Dx:\r\n");
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
		      "#RRGGBBDx (caps only)\r\n");
reset_buf:
	i = 0;
	avr_memset(buf, 0, sizeof(buf));
}

ISR(TIMER1_COMPA_vect)
{
	static uint8_t grad = 0;

	wheel(grad++);
}

int main(void)
{
	sei();
	spi_init();
	uart_init(UART_UBRRN, (1 << RXCIE0));
	timer1_init();
	uart_printstr("Welcome!");
	uart_printstr("\r\nEnter a hexadecimal color code (#RRGGBBDx) to modify "
		      "the RGB LED Dx:\r\n");

	while (1);
}