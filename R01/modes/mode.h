/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:12:37 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/30 11:28:08 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_H
#define MODE_H

#include <avr/io.h>

#define MODE_START_PREV -1
#define MODE_START_NEXT 1
#define MODE_START_ACTUAL 0

enum e_mode {
	E_MODE_0_START_SEQ,
	E_MODE_1_ADC_POT,
	E_MODE_2_ADC_LDR,
	E_MODE_3_ADC_NTC,
	E_MODE_4_ADC_TEMP,
	E_MODE_5_RGB,
	E_MODE_6_AHT_TEMPC,
	E_MODE_7_AHT_TEMPF,
	E_MODE_8_AHT_HUMI,
	E_MODE_9_DATE_HOUR,
	E_MODE_10_DATE_DAY,
	E_MODE_11_DATE_YEAR,
};

#define FIRST_MODE_NB E_MODE_1_ADC_POT
#define LAST_MODE_NB E_MODE_11_DATE_YEAR

extern volatile int8_t g_mode;

void switch_mode(int8_t dir);
void mode_0_start_seq_init(void);
void mode_0_start_seq_exec_timer0(void);
void mode_0_start_seq_clear(void);

#endif /* MODE_H */