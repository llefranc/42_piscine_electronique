/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:12:37 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 17:28:59 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_H
#define MODE_H

void mode_1_adc_pot_start(void);
void mode_1_adc_pot_stop(void);
void mode_2_adc_ldr_start(void);
void mode_2_adc_ldr_stop(void);
void mode_3_adc_ntc_start(void);
void mode_3_adc_ntc_stop(void);
void mode_4_adc_temp_start(void);
void mode_4_adc_temp_stop(void);
void mode_5_rgb_start(void);
void mode_5_rgb_stop(void);
void mode_6_aht_temp_c_start(void);
void mode_6_aht_temp_c_stop(void);
void mode_7_aht_temp_f_start(void);
void mode_7_aht_temp_f_stop(void);
void mode_8_aht_humi_start(void);
void mode_8_aht_humi_stop(void);
void mode_9_time_start(void);
void mode_9_time_stop(void);
void mode_10_date_day_start(void);
void mode_10_date_day_stop(void);
void mode_11_date_year_start(void);
void mode_11_date_year_stop(void);

#endif /* MODE_H */