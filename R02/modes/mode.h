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

#define MODE_START_PREV -1
#define MODE_START_NEXT 1
#define MODE_START_ACTUAL 0

enum e_mode {
	E_MODE_INIT,
	E_MODE_1,
	E_MODE_2,
	E_MODE_3,
	E_MODE_4,
	E_MODE_5,
	E_MODE_6,
	E_MODE_7,
	E_MODE_8,
	E_MODE_9,
	E_MODE_10,
	E_MODE_11,
};

#define FIRST_MODE_NB E_MODE_1
#define LAST_MODE_NB E_MODE_11

void mode_1_adc_pot_init(void);
void mode_1_adc_pot_clear(void);
void mode_2_adc_ldr_init(void);
void mode_2_adc_ldr_clear(void);
void mode_3_adc_ntc_init(void);
void mode_3_adc_ntc_clear(void);
void mode_4_adc_temp_init(void);
void mode_4_adc_temp_clear(void);
void mode_5_rgb_init(void);
void mode_5_rgb_clear(void);
void mode_6_aht_temp_c_init(void);
void mode_6_aht_temp_c_clear(void);
void mode_7_aht_temp_f_init(void);
void mode_7_aht_temp_f_clear(void);
void mode_8_aht_humi_init(void);
void mode_8_aht_humi_clear(void);
void mode_9_time_init(void);
void mode_9_time_clear(void);
void mode_10_date_day_init(void);
void mode_10_date_day_clear(void);
void mode_11_date_year_init(void);
void mode_11_date_year_clear(void);

#endif /* MODE_H */