/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 18:01:38 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/28 19:57:42 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

#define ADC_POT (0b0000)  /* Potentiometer (ADC0) */
#define ADC_LDR (0b0001)  /* Light sensor (ADC1) */
#define ADC_NTC (0b0010)  /* Thermistor NTC (ADC2)*/
#define ADC_TEMP (0b1000) /* Internal temperature sensor (ADC8)*/

/*
 * Found this calibration value for atmega328p on github :
 * https://gist.github.com/jenschr/1762b4784e1dcf11df146f928b92ca61
 */
#define ADC_INTERNAL_SENSOR_TEMP_CAL_OFFSET 334

/* 10-bit precision needs a prescaler of 128 */
#define ADC_PRESC_32 ((1 << ADPS2) | (1 << ADPS0))
#define ADC_PRESC_64 ((1 << ADPS2) | (1 << ADPS1))
#define ADC_PRESC_128 ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0))

/**
 * Init the ADC with a prescaler and 8-bit or 10-bit precision and with AVCC as
 * reference.
*/
static inline void adc_init(uint8_t prescaler, uint8_t adlar_bit)
{
	/* Enabling ADC + setting prescaler */
	ADCSRA |= (1 << ADEN) | prescaler;

	/* AVCC with external capacitor at AREF pin + set precision */
	ADMUX |= (1 << REFS0) | adlar_bit;
}

/**
 * Select an input source for ADC.
*/
static inline void adc_mux_select(uint8_t adc_input)
{
	ADMUX &= 0xF0;
	ADMUX |= adc_input;
}

/**
 * Start an 8-bit ADC conversion.
*/
static inline uint8_t adc_read_8bit(void)
{
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADCH;
}

/**
 * Start a 10-bit ADC conversion.
*/
static inline uint16_t adc_read_10bit(void)
{
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

static inline void adc_clear(void)
{
	ADCSRA = 0;
	ADMUX = 0;
}

#endif /* ADC_H */