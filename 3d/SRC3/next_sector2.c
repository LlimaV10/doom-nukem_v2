/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_sector2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:15:03 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 17:15:04 by dbolilyi         ###   ########.fr       */
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
	iw->d.save_bot_betw = get_between_sectors_walls(
		&iw2, left, right, &iw->d.save_top_betw);
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
		return ;
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
