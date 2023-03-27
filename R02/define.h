/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:33:11 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 17:57:54 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
#define DEFINE_H

#define LED1 PB0
#define LED2 PB1
#define LED3 PB2
#define LED4 PB4

#define LED5_R PD5
#define LED5_G PD6
#define LED5_B PD3

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

#endif /* DEFINE_H */