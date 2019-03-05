/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_animations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:05:25 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 12:05:26 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	do_wall_animations_s1_p2(t_sdl *iw, t_wall_animation *tmp)
{
	if (abs(tmp->curr_dy) < abs(tmp->dy))
	{
		do_wall_animation_step_dy(iw, tmp, ((tmp->dy > 0) ?
			tmp->speed : -tmp->speed) * 10);
		tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
	}
	else if (abs(tmp->curr_dx) < abs(tmp->dx))
	{
		do_wall_animation_step_dx(iw, tmp, ((tmp->dx > 0) ?
			tmp->speed : -tmp->speed) * 10);
		tmp->curr_dx += ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10;
	}
}

void	do_wall_animations_s1(t_sdl *iw, t_wall_animation *tmp)
{
	if (tmp->priority == 0)
	{
		if (abs(tmp->curr_dx) < abs(tmp->dx))
		{
			do_wall_animation_step_dx(iw, tmp, ((tmp->dx > 0) ?
				tmp->speed : -tmp->speed) * 10);
			tmp->curr_dx += ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10;
		}
		else if (abs(tmp->curr_dy) < abs(tmp->dy))
		{
			do_wall_animation_step_dy(iw, tmp, ((tmp->dy > 0) ?
				tmp->speed : -tmp->speed) * 10);
			tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
		}
	}
	else
		do_wall_animations_s1_p2(iw, tmp);
}

void	do_wall_animations_s2_p2(t_sdl *iw, t_wall_animation *tmp)
{
	if (tmp->curr_dx != 0)
	{
		do_wall_animation_step_dx(iw, tmp, ((tmp->dx < 0) ?
			tmp->speed : -tmp->speed) * 10);
		tmp->curr_dx += ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10;
	}
	else if (tmp->curr_dy != 0)
	{
		do_wall_animation_step_dy(iw, tmp, ((tmp->dy < 0) ?
			tmp->speed : -tmp->speed) * 10);
		tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
	}
}

void	do_wall_animations_s2(t_sdl *iw, t_wall_animation *tmp)
{
	if (tmp->priority == 0)
	{
		if (tmp->curr_dy != 0)
		{
			do_wall_animation_step_dy(iw, tmp, ((tmp->dy < 0) ?
				tmp->speed : -tmp->speed) * 10);
			tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
		}
		else if (tmp->curr_dx != 0)
		{
			do_wall_animation_step_dx(iw, tmp, ((tmp->dx < 0) ?
				tmp->speed : -tmp->speed) * 10);
			tmp->curr_dx += ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10;
		}
	}
	else
		do_wall_animations_s2_p2(iw, tmp);
}

void	do_wall_animations(t_sdl *iw)
{
	t_wall_animation	*tmp;

	tmp = iw->wall_animations;
	while (tmp)
	{
		if (clock() - tmp->prev_clock > CLKS_P_S / 50)
		{
			if (tmp->status == 1)
				do_wall_animations_s1(iw, tmp);
			else
				do_wall_animations_s2(iw, tmp);
			tmp->prev_clock = clock();
		}
		tmp = tmp->next;
	}
}
