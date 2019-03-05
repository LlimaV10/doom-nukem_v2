/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_animations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:01:21 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/01 18:04:53 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	add_wall_to_wall_animation2(t_sdl *iw, int add_wall)
{
	int					i;
	t_wall_animation	*tmp;

	tmp = iw->v.wall_anim;
	i = -1;
	while (++i < tmp->count_walls)
		if (add_wall == tmp->walls[i])
			return ;
	tmp->walls[tmp->count_walls] = add_wall;
	tmp->count_walls++;
	if (tmp->count_walls == COUNT_WALLS_TO_ANIM)
	{
		iw->v.f_button_mode = 0;
		iw->v.f_button_pointer = 0;
		iw->v.submenu_mode = 5;
		draw_submenu(iw);
	}
}

void	add_wall_to_wall_animation(t_sdl *iw)
{
	t_wall_animation	*tmp;
	int					add_wall;

	add_wall = get_wall_by_pointer(iw,
			*(iw->v.look_sector), *(iw->v.look_wall));
	if (iw->walls[add_wall].nextsector != -1 ||
		iw->walls[add_wall].next->nextsector != -1 ||
		iw->walls[add_wall].next->next->nextsector != -1)
		return ;
	if (iw->v.wall_anim == 0)
	{
		tmp = (t_wall_animation *)malloc(sizeof(t_wall_animation));
		tmp->dx = 0;
		tmp->dy = 0;
		tmp->curr_dx = 0;
		tmp->curr_dy = 0;
		tmp->speed = 1;
		tmp->status = 0;
		tmp->count_walls = 1;
		tmp->walls[0] = add_wall;
		tmp->trigger = (t_picture *)iw->v.f_button_pointer;
		iw->v.wall_anim = tmp;
	}
	else
		add_wall_to_wall_animation2(iw, add_wall);
}

void	calculate_pictures_list(t_sdl *iw, t_wall *wall, t_picture *p)
{
	while (p)
	{
		calculate_picture(iw, wall, p);
		p = p->next;
	}
}

void	do_wall_animation_step_dx(t_sdl *iw, t_wall_animation *a, int dx)
{
	iw->i = -1;
	while (++iw->i < a->count_walls)
	{
		if (a->moving_type == 1 && iw->walls[a->walls[iw->i]].next
			->next->y > iw->walls[a->walls[iw->i]].next->y)
		{
			iw->walls[a->walls[iw->i]].next->x -= dx;
			iw->walls[a->walls[iw->i]].next->next->x -= dx;
		}
		else
		{
			iw->walls[a->walls[iw->i]].next->x += dx;
			iw->walls[a->walls[iw->i]].next->next->x += dx;
		}
		get_wall_line2(&iw->walls[a->walls[iw->i]]);
		get_wall_line2(iw->walls[a->walls[iw->i]].next);
		get_wall_line2(iw->walls[a->walls[iw->i]].next->next);
		calculate_pictures_list(iw, &iw->walls[a->walls[iw->i]],
				iw->walls[a->walls[iw->i]].p);
		calculate_pictures_list(iw, iw->walls[a->walls[iw->i]].next,
				iw->walls[a->walls[iw->i]].next->p);
		calculate_pictures_list(iw, iw->walls[a->walls[iw->i]].next->
				next, iw->walls[a->walls[iw->i]].next->next->p);
	}
}

void	do_wall_animation_step_dy(t_sdl *iw, t_wall_animation *a, int dy)
{
	iw->i = -1;
	while (++iw->i < a->count_walls)
	{
		if (a->moving_type == 2 && iw->walls[a->walls[iw->i]].next
			->next->x > iw->walls[a->walls[iw->i]].next->x)
		{
			iw->walls[a->walls[iw->i]].next->y -= dy;
			iw->walls[a->walls[iw->i]].next->next->y -= dy;
		}
		else
		{
			iw->walls[a->walls[iw->i]].next->y += dy;
			iw->walls[a->walls[iw->i]].next->next->y += dy;
		}
		get_wall_line2(&iw->walls[a->walls[iw->i]]);
		get_wall_line2(iw->walls[a->walls[iw->i]].next);
		get_wall_line2(iw->walls[a->walls[iw->i]].next->next);
		calculate_pictures_list(iw, &iw->walls[a->walls[iw->i]],
				iw->walls[a->walls[iw->i]].p);
		calculate_pictures_list(iw, iw->walls[a->walls[iw->i]].next,
				iw->walls[a->walls[iw->i]].next->p);
		calculate_pictures_list(iw, iw->walls[a->walls[iw->i]].next->
				next, iw->walls[a->walls[iw->i]].next->next->p);
	}
}
