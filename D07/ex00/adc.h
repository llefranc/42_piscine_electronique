/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 18:01:38 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/25 18:55:05 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

#define ADC_POT (0b0000)
#define ADC_LDR (0b0001)
#define ADC_NTC (0b0010)

static inline void adc_init(void)
{
	/* Enabling ADC + setting prescaler */
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);

	/* AVCC with external capacitor at AREF pin + 8 bit precision */
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
}

static inline void adc_mux_select(uint8_t adc_input)
{
	ADMUX &= 0xF0;
	ADMUX |= adc_input;
}

static inline uint8_t adc_read(void)
{
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADCH;
}

#endif /* ADC_H */