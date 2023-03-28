/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_adc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:21:06 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/28 21:09:07 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_ADC_H
#define MODE_ADC_H

void mode_1_adc_pot_init(void);
void mode_2_adc_ldr_init(void);
void mode_3_adc_ntc_init(void);
void mode_4_adc_temp_init(void);

void mode_4_adc_temp_exec_timer1(void);

void mode_x_adc_xxx_exec_timer0(void);
void mode_x_adc_xxx_exec_timer1(void);
void mode_x_adc_xxx_clear(void);

#endif /* MODE_ADC_H */