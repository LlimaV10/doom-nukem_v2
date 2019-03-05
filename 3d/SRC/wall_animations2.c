/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_animations2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:26:33 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:26:35 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_selected_walls_to_be_animated(t_sdl *iw)
{
	t_save_wall	*tmp;
	int			i;
	int			y;

	tmp = *(iw->vw_save);
	while (tmp)
	{
		i = -1;
		while (++i < iw->v.wall_anim->count_walls)
			if (iw->walls[iw->v.wall_anim->walls[i]].next == tmp->wall ||
				iw->walls[iw->v.wall_anim->walls[i]].next->next == tmp->wall)
			{
				y = 0;
				while (++y < WINDOW_H)
					set_pixel2(iw->sur, tmp->x, y, 0xFF0000);
				break ;
			}
		tmp = tmp->next;
	}
}

void	exit_editing_wall_animation(t_sdl *iw)
{
	if (iw->v.submenu_mode >= 7)
	{
		do_wall_animation_step_dx(iw, iw->v.wall_anim, -iw->v.wall_anim->dx);
		do_wall_animation_step_dy(iw, iw->v.wall_anim, -iw->v.wall_anim->dy);
	}
	free(iw->v.wall_anim);
	iw->v.wall_anim = 0;
	iw->v.submenu_mode = 0;
	draw_submenu(iw);
}

void	change_wall_animation_status(t_sdl *iw, t_picture *p)
{
	t_wall_animation	*tmp;

	tmp = iw->wall_animations;
	while (tmp)
	{
		if (tmp->trigger == p)
		{
			tmp->status = ((tmp->status == 0) ? 1 : 0);
			return ;
		}
		tmp = tmp->next;
	}
}
