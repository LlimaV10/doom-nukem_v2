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

void	draw_next_sector3(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_sdl *iw2)
{
	if (left->wall->glass >= 0)
	{
		if (iw->sectors[iw2->d.cs].cl.t < 0)
			draw_skybox(iw2);
		sort_sprites(iw);
		draw_glass_sprites(iw2);
		change_saved_top_bot_between_lines(iw, right->x - left->x + 1);
		draw_glass_tex(iw, left, right, right->x - left->x + 1);
	}
	free(iw->d.save_bot_betw);
	free(iw->d.save_top_betw);
	fill_portal(iw, left, right, iw2);
}

void	draw_next_sector2(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_sdl *iw2)
{
	t_visited_sector	*tmp;

	get_direction(iw2);
	get_screen_line(iw2);
	get_left_right_lines_points(iw2);
	iw2->d.vw = 0;
	iw2->d.vwp = 0;
	iw2->d.screen_left = left->x;
	iw2->d.screen_right = right->x;
	fill_tb_by_slsr(iw2);
	get_visible_walls(iw2);
	get_left_right_visible_walls(iw2);
	iw2->d.prev_sector = iw->d.cs;
	iw2->d.prev_sector_wall = left->wall;
	iw->sectors[iw2->d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw2->d.cs;
	tmp->next = iw2->visited_sectors;
	iw2->visited_sectors = tmp;
	draw_start(iw2);
	iw2->visited_sectors = iw2->visited_sectors->next;
	free(tmp);
	draw_next_sector3(iw, left, right, iw2);
}

void	draw_next_sector(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_sdl				iw2;
	int					i;

	iw2 = *iw;
	fill_top_skybox(&iw2, left, right->x - left->x + 1);
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	iw->d.save_bot_betw = get_between_sectors_walls
		(&iw2, left, right, &iw->d.save_top_betw);
	draw_between_sectors_walls(&iw2, left, right);
	i = left->x - 1;
	while (++i < right->x)
	{
		iw->d.top[i] = iw2.d.top[i];
		iw->d.bottom[i] = iw2.d.bottom[i];
	}
	if (sector_visited(iw, iw2.d.cs))
	{
		free(iw->d.save_bot_betw);
		free(iw->d.save_top_betw);
		return;
	}
	draw_next_sector2(iw, left, right, &iw2);
}

void	draw_next_sector_kernel4(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_sdl *iw2)
{
	if (left->wall->glass >= 0)
	{
		if (iw->sectors[iw2->d.cs].cl.t < 0)
			draw_skybox_kernel(iw2);
		sort_sprites(iw);
		draw_glass_sprites_kernel(iw2);
		change_saved_top_bot_between_lines(iw, right->x - left->x + 1);
		draw_glass_tex_kernel(iw, left, right, right->x - left->x + 1);
	}
	free(iw->d.save_bot_betw);
	free(iw->d.save_top_betw);
}

void	draw_next_sector_kernel3(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_sdl *iw2)
{
	t_visited_sector	*tmp;

	iw2->d.prev_sector_wall = left->wall;
	if (left->wall->glass >= 0)
	{
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_top,
			iw->k.m_save_top, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_bottom,
			iw->k.m_save_bottom, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
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
		return;
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
	iw->d.save_bot_betw = get_between_sectors_walls
		(&iw2, left, right, &iw->d.save_top_betw);
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
