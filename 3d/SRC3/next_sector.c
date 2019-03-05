/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_sector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:05:34 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:05:35 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_next_sector_kernel3(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_sdl *iw2)
{
	t_visited_sector	*tmp;

	iw2->d.prev_sector_wall = left->wall;
	if (left->wall->glass >= 0)
	{
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_top,
			iw->k.m_save_top, 0, 0, USELESS4, 0, NULL, NULL);
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_bottom,
			iw->k.m_save_bottom, 0, 0, USELESS4, 0, NULL, NULL);
	}
	iw->sectors[iw2->d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw2->d.cs;
	tmp->next = iw2->visited_sectors;
	iw2->visited_sectors = tmp;
	draw_start(iw2);
	iw2->visited_sectors = iw2->visited_sectors->next;
	free(tmp);
	draw_next_sector_kernel4(iw, left, right, iw2);
}

void	draw_next_sector_kernel2(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_sdl *iw2)
{
	if ((iw->d.wallBot[0] < 0 && iw->d.wallBot[right->x - left->x] < 0) ||
		(iw->d.wallTop[0] >= WINDOW_H &&
		iw->d.wallTop[right->x - left->x] >= WINDOW_H)
		|| sector_visited(iw, iw2->d.cs))
	{
		free(iw->d.save_bot_betw);
		free(iw->d.save_top_betw);
		return ;
	}
	get_direction(iw2);
	get_screen_line(iw2);
	get_left_right_lines_points(iw2);
	iw2->d.vw = 0;
	iw2->d.vwp = 0;
	if (left->x > iw2->d.screen_left)
		iw2->d.screen_left = left->x;
	if (right->x < iw2->d.screen_right)
		iw2->d.screen_right = right->x;
	fill_tb_by_slsr(iw2);
	get_visible_walls(iw2);
	get_left_right_visible_walls(iw2);
	iw2->d.prev_sector = iw->d.cs;
	draw_next_sector_kernel3(iw, left, right, iw2);
}

void	draw_next_sector_kernel(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int len)
{
	t_sdl				iw2;

	iw2 = *iw;
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	iw->d.save_bot_betw = get_between_sectors_walls(
		&iw2, left, right, &iw->d.save_top_betw);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1
		&& left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& (iw->d.save_top_betw[WINDOW_W / 2 - left->x] > WINDOW_H / 2 ||
			iw->d.save_bot_betw[WINDOW_W / 2 - left->x] < WINDOW_H / 2))
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
		draw_floor_ceil_betw_tex_kernel(iw, left, right, len);
	else
		draw_inclined_floor_ceil_betw_tex_kernel(iw, left, right, len);
	draw_next_sector_kernel2(iw, left, right, &iw2);
}

void	change_saved_top_bot_between_lines(t_sdl *iw, int len)
{
	int		j;

	if (iw->d.wallTop[0] > iw->d.save_top_betw[0])
	{
		j = -1;
		while (++j < len)
			iw->d.save_top_betw[j] = iw->d.wallTop[j];
	}
	if (iw->d.wallBot[0] < iw->d.save_bot_betw[0])
	{
		j = -1;
		while (++j < len)
			iw->d.save_bot_betw[j] = iw->d.wallBot[j];
	}
}

int		sector_visited(t_sdl *iw, int sec)
{
	t_visited_sector	*s;

	s = iw->visited_sectors;
	while (s)
	{
		if (sec == s->sec)
			return (1);
		s = s->next;
	}
	return (0);
}
