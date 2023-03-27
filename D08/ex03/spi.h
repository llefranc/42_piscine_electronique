/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:27:07 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 15:44:51 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#define SPI_SS PB2
#define SPI_MOSI PB3
#define SPI_MISO PB4
#define SPI_SCK PB5

#define DDR_SPI DDRB
#define PORT_SPI PORTB

#define PADDING_BITS 0xE0

#define LED_OFF 0xE0000000

/* Levels of brightness */
#define B_LOW 0x01000000
#define B_MED 0x08000000
#define B_HIGH 0x1F000000

/* LED colors */
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define CYAN 0x00FFFF
#define MAGENTA 0xFF00FF
#define WHITE 0xFFFFFF

/**
 * Init the SPI as master, MOSI/SCK/SS pins as outputs.
*/
static inline void spi_init(void)
{
	/*
	 * Set as output MOSI, SCK and SS (even if not used). This step must
	 * occur before enabling SPI
	 */
	DDR_SPI = (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);

	/* Enable SPI master mode + prescaler of 64 */
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

/**
 * Send a byte of data through MOSI.
*/
static inline void spi_send_byte(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
}

/**
 * Send a word of data through MOSI.
*/
static inline void spi_send_word(uint16_t data)
{
	spi_send_byte((uint8_t)(data >> 8));
	spi_send_byte((uint8_t)data);
}

/**
 * Send a dword of data through MOSI.
*/
static inline void spi_send_dword(uint32_t data)
{
	spi_send_byte((uint8_t)(data >> 24));
	spi_send_byte((uint8_t)(data >> 16));
	spi_send_byte((uint8_t)(data >> 8));
	spi_send_byte((uint8_t)data);
}

/**
 * Send LED APA102 start frame (32 bits set to 0).
*/
static inline void spi_send_led_start(void)
{
	spi_send_dword(0);
}

/**
 * Send LED APA102 stop frame (32 bits set to 1).
*/
static inline void spi_send_led_stop(void)
{
	spi_send_dword(0xFFFFFFFF);
}

/**
 * Send LED APA102 color frame (3 bits to 1, 5 bits for brightness, and
 * blue/green/red bytes).
*/
static inline void spi_send_led_rgb(uint8_t bright, uint8_t r, uint8_t g,
				    uint8_t b)
{
	spi_send_byte(0b11100000 | bright);
	spi_send_byte(b);
	spi_send_byte(g);
	spi_send_byte(r);
}

/**
 * Set the 3 LEDs through SPI.
*/
static inline void spi_set_leds(uint32_t led1, uint32_t led2, uint32_t led3)
{
	spi_send_led_start();
	spi_send_led_rgb(PADDING_BITS | (uint8_t)(led1 >> 24), led1 >> 16,
			 led1 >> 8, (uint8_t)led1);
	spi_send_led_rgb(PADDING_BITS | (uint8_t)(led2 >> 24), led2 >> 16,
			 led2 >> 8, (uint8_t)led2);
	spi_send_led_rgb(PADDING_BITS | (uint8_t)(led3 >> 24), led3 >> 16,
			 led3 >> 8, (uint8_t)led3);
	spi_send_led_stop();
}

#endif /* SPI_H */