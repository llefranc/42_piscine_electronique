/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:48:29 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/27 15:52:17 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_H
#define TIMER_H

#define TIMER_PRESCALER_1024 1024
#define TIMER_PRESCALER_256 256

void timer0_clear(void);
void timer1_clear(void);

#endif