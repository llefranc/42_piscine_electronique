/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 21:29:47 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/20 17:31:36 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"
#include "i2c.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

/**
 * 1. Il faut reussir a communiquer avec l'ecran en point to point
 * 2. Il faut surement faire un general call -> essayer de communiquer avec l'ecran comme ca
 * 2bis. Sinon est-ce que si 2 slaves ont la meme adresse ils recuperent tous les 2 la data ?
 * 3. Ensuite brancher les 2 cartes ensemble et voir si un MCU peut controler 2 ecrans
 * 4. Regle pour definir le master ? Le nombre de joueurs ?
 * 5. Le master held down the SCL line tant que ya pas de pression sur le bouton.
 *    Tous les autres vont essayer de chopper la line mais fail > ce seront les esclaves.
 *    Au moment ou le bouton du master est clique, il lache la line ?
 *    Indiquer une led qui est le master.
 *
 *
*/

// TW_MT_xxx - master transmitter
// TW_MR_xxx - master receiver
// TW_ST_xxx - slave transmitter
// TW_SR_xxx - slave receiver
// #define 	TW_START   0x08
// #define 	TW_REP_START   0x10
// #define 	TW_MT_SLA_ACK   0x18
// #define 	TW_MT_SLA_NACK   0x20
// #define 	TW_MT_DATA_ACK   0x28
// #define 	TW_MT_DATA_NACK   0x30
// #define 	TW_MT_ARB_LOST   0x38
// #define 	TW_MR_ARB_LOST   0x38
// #define 	TW_MR_SLA_ACK   0x40
// #define 	TW_MR_SLA_NACK   0x48
// #define 	TW_MR_DATA_ACK   0x50
// #define 	TW_MR_DATA_NACK   0x58
// #define 	TW_ST_SLA_ACK   0xA8
// #define 	TW_ST_ARB_LOST_SLA_ACK   0xB0
// #define 	TW_ST_DATA_ACK   0xB8
// #define 	TW_ST_DATA_NACK   0xC0
// #define 	TW_ST_LAST_DATA   0xC8
// #define 	TW_SR_SLA_ACK   0x60
// #define 	TW_SR_ARB_LOST_SLA_ACK   0x68
// #define 	TW_SR_GCALL_ACK   0x70
// #define 	TW_SR_ARB_LOST_GCALL_ACK   0x78
// #define 	TW_SR_DATA_ACK   0x80
// #define 	TW_SR_DATA_NACK   0x88
// #define 	TW_SR_GCALL_DATA_ACK   0x90
// #define 	TW_SR_GCALL_DATA_NACK   0x98
// #define 	TW_SR_STOP   0xA0
// #define 	TW_NO_INFO   0xF8
// #define 	TW_BUS_ERROR   0x00
// #define 	TW_STATUS_MASK
// #define 	TW_STATUS   (TWSR & TW_STATUS_MASK)

/*
penser a rajouter le case ou on perd l'arbitration
*/
#define I2C_SLA_PCA955 0b0100000

#define I2C_PCA955_I0 0
#define I2C_PCA955_I1 1
#define I2C_PCA955_O0 2
#define I2C_PCA955_O1 3
#define I2C_PCA955_N0 4
#define I2C_PCA955_N1 5
#define I2C_PCA955_C0 6
#define I2C_PCA955_C1 7

// ISR(TWI_vect)
// {
// 	switch (TW_STATUS) {
// 	case TW_START:
// 		uart_printstr("TW_START\r\n");
// 		i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
// 		break;
// 	case TW_REP_START:
// 		uart_printstr("TW_REP_START\r\n");
// 		break;
// 	case TW_MT_SLA_ACK:
// 		uart_printstr("TW_MT_SLA_ACK\r\n");

// 		break;
// 	case TW_MT_DATA_ACK:
// 		uart_printstr("TW_MT_DATA_ACK\r\n");
// 		break;
// 	case TW_BUS_ERROR:
// 		uart_printstr("TW_BUS_ERROR\r\n");
// 		i2c_reset_bus_error();
// 		break;
// 	default:
// 		uart_printstr("default\r\n");
// 		break;
// 	}
// }

ISR(TIMER1_COMPA_vect)
{
	static uint8_t i;
	if (i++ > 10)
		stop_timer1();
	PORTB ^= (1 << PB0);
}

#define SW5 0
#define D11 1
#define D10 2
#define D9 3
#define DIG1 4
#define DIG2 5
#define DIG3 6
#define DIG4 7

#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << PB1) | (1 << PB0);
	sei();
	uart_init(UART_UBRRN, (0 << RXCIE0));
	i2c_init();

	/* --------------- PREMIER EXEMPLE --------------- */
	/* set la direction en output de D10 et DIG4 */
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	i2c_write(I2C_PCA955_C0);
	i2c_write(0xFF & ~(1 << D10) & ~(1 << DIG4));
	i2c_write(0x00);
	i2c_stop();

	/* allume d10 quand config en output */
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	i2c_write(I2C_PCA955_O0);
	i2c_write(~(1 << D10));
	i2c_stop();

	/* eteint d10 quand config en output */
	_delay_ms(1000);
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	i2c_write(I2C_PCA955_O0);
	i2c_write(~(0 << D10));
	i2c_stop();

	/* allume toutes les barres des digits de DIG4 */
	_delay_ms(1000);
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	i2c_write(I2C_PCA955_O0);
	i2c_write(0x00); /* tous on */
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	i2c_write(I2C_PCA955_O1);
	i2c_write(0xFF); /* tous eteint car note gate */
	i2c_stop();
	/* --------------- PREMIER EXEMPLE --------------- */


	/* --------------- SECOND EXEMPLE --------------- */
	_delay_ms(1000);
	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	i2c_write(I2C_PCA955_C0);
	i2c_write(0b00001111); /* Mets en output quanc c'est 0 */
	i2c_write(0b00000000);
	i2c_stop();

	i2c_start();
	i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	i2c_write(I2C_PCA955_O0);
	i2c_write(0x00);
	i2c_write(0xFF); /* allume tous les chiffres */
	i2c_stop();
	/* ----------------------------------------------- */


	/* --------------- TROISIEME EXEMPLE --------------- */
	// _delay_ms(1000);
	// i2c_start();
	// i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	// i2c_write(I2C_PCA955_C0);
	// i2c_write(0b00001111); /* Mets en output quanc c'est 0 */
	// i2c_write(0b00000000);
	// i2c_stop();

	// i2c_start();
	// i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	// i2c_write(I2C_PCA955_O0);
	// i2c_write(0b01111111); /* allume qu'un chiffre */
	// i2c_write(0xFF); /* allume toutes les barres */
	// i2c_stop();

	// _delay_ms(1000);
	// i2c_start();
	// i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
	// i2c_write(I2C_PCA955_O0);
	// i2c_write(0b10111111); /* allume un autre chiffre */
	// i2c_stop();
	/* ----------------------------------------------- */

	/* --------------- QUATRIEME EXEMPLE --------------- */
	_delay_ms(1000);

	/* MARRRRRRRRRRRRRRRRRRRRRCHEEEEE PERMET D'ALLUMER 2 CHIFFRES !!! */
	while (1) {
		i2c_start();
		i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
		i2c_write(I2C_PCA955_O0);
		i2c_write(0b01111111); /* allume qu'un chiffre */
		i2c_write(0xFF); /* allume toutes les barres */
		i2c_stop();

		_delay_ms(10);

		i2c_start();
		i2c_sla_wr(I2C_SLA_PCA955, TW_WRITE);
		i2c_write(I2C_PCA955_O0);
		i2c_write(0b10111111); /* allume qu'un chiffre */
		i2c_write(0x0F); /* allume toutes les barres */
		i2c_stop();
		_delay_ms(10);

	}

}