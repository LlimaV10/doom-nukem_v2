/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:02:00 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:02:00 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_start(t_sdl *iw)
{
	t_save_wall			*left;
	t_save_wall			*right;
	t_save_wall_pairs	*tmp;

	if (iw->d.vw == 0)
		return ;
	left = iw->d.vw;
	while (left != 0)
	{
		if ((right = find_next_vis_wall(iw, left)) != 0 && left->x < right->x)
			if (!(right->x < iw->d.screen_left || left->x > iw->d.screen_right))
				add_pair(iw, left, right);
		left = left->next;
	}
	new_sort_pairs(iw);
	calculate_sprites_once(iw);
	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		get_sprites_top_bottom(iw, tmp);
		draw_left_right(iw, tmp->left, tmp->right);
		tmp = tmp->next;
	}
	save_walls(iw);
	free_pairs(iw);
}

void	draw4(t_sdl *iw)
{
	if (iw->v.kernel)
		draw_sprites_kernel(iw);
	else
		draw_sprites(iw);
	if (iw->v.game_mode && !iw->v.kernel)
		draw_gun(iw);
	else if (iw->v.game_mode)
		draw_gun_kernel(iw);
	if (iw->v.kernel)
		iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_sur,
		CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int),
		iw->sur->pixels, 0, NULL, NULL);
	if (iw->v.wall_anim != 0)
		draw_selected_walls_to_be_animated(iw);
	iw->d.vw = *(iw->vw_save);
	*(iw->vw_save) = 0;
	free_walls(iw);
	iw->p.z += iw->v.crouch;
}

void	draw3(t_sdl *iw)
{
	t_visited_sector	*tmp;

	iw->d.prev_sector = -1;
	iw->d.prev_sector_wall = 0;
	iw->sectors[iw->d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw->d.cs;
	tmp->next = iw->visited_sectors;
	iw->visited_sectors = tmp;
	draw_start(iw);
	iw->visited_sectors = iw->visited_sectors->next;
	free(tmp);
	if (!check_look_picture(iw))
		*(iw->v.look_picture) = 0;
	if (!iw->v.kernel)
		draw_skybox(iw);
	else
		draw_skybox_kernel(iw);
	sort_sprites(iw);
	draw4(iw);
}

void	draw2(t_sdl *iw)
{
	get_direction(iw);
	get_screen_line(iw);
	get_left_right_lines_points(iw);
	iw->d.vw = 0;
	iw->d.vwp = 0;
	iw->d.screen_left = 0;
	iw->d.screen_right = WINDOW_W - 1;
	get_visible_walls(iw);
	get_left_right_visible_walls(iw);
	if (iw->v.kernel)
	{
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_top, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_bottom, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	}
	if (iw->v.mouse_mode == 1)
	{
		iw->v.look_portal = 0;
		*(iw->v.look_wall) = 0;
		*(iw->v.look_sector) = 0;
		*(iw->v.look_picture) = 0;
		iw->v.look_sprite = 0;
	}
	draw3(iw);
}

void	draw(t_sdl *iw)
{
	t_sprite			*tmp1;
	int					j;

	j = 0;
	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		tmp1->draweble = 0;
		j = 0;
		while (j < WINDOW_W)
		{
			tmp1->top[j] = -1;
			j++;
		}
		tmp1 = tmp1->next;
	}
	j = -1;
	while (++j < iw->v.sc)
		iw->sectors[j].visited = 0;
	set_top_bottom(iw);
	if ((iw->d.cs = get_sector(iw)) == -1)
		return ;
	iw->p.z -= iw->v.crouch;
	iw->v.ls = iw->d.cs;
	draw2(iw);
}
