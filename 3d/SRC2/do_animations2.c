/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_animations2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:36:15 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:36:16 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	do_sector_animations(t_sdl *iw)
{
	t_sector_animation	*tmp;

	tmp = iw->sector_animations;
	while (tmp)
	{
		if (clock() - tmp->prev_clock > CLKS_P_S / 50)
		{
			if (tmp->status == 1 && abs(tmp->curr_dy) < abs(tmp->dy))
			{
				do_sector_animation_step(iw, tmp, ((tmp->dy > 0) ?
					tmp->speed : -tmp->speed) * 10);
				tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
			}
			else if (tmp->status == 0 && tmp->curr_dy != 0)
			{
				do_sector_animation_step(iw, tmp, ((tmp->dy < 0) ?
					tmp->speed : -tmp->speed) * 10);
				tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
			}
			tmp->prev_clock = clock();
		}
		tmp = tmp->next;
	}
}
