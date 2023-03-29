/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_aht.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:27:54 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/03/29 13:27:29 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_AHT_H
#define MODE_AHT_H

void mode_x_aht_xxx_init(void);
void mode_x_aht_xxx_exec_timer0(char unit);
void mode_x_aht_xxx_clear(void);

void mode_6_aht_temp_c_exec_timer1(void);
void mode_7_aht_temp_f_exec_timer1(void);
void mode_8_aht_humi_exec_timer1(void);

#endif /* MODE_AHT_H */