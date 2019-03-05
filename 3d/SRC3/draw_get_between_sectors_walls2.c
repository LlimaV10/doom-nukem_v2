/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_get_between_sectors_walls2.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:54:49 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:54:51 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_between_sectors_walls2(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int *tmp)
{
	t_draw_line		l;
	int				lz;
	int				rz;

	l.x0 = left->x;
	l.x1 = right->x;
	lz = get_ceil_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x
		- left->wall->next->x, left->p.y + iw->walls
		[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_ceil_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x
		- left->wall->next->x, right->p.y + iw->walls
		[left->wall->nextsector_wall].y - left->wall->next->y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz)
		/ (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz)
		/ (int)right->plen + iw->p.rotup;
	brez_line(tmp, l);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1
		&& left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& tmp[WINDOW_W / 2 - left->x] > WINDOW_H / 2)
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	draw_between_sectors_top_tex(iw, left, right, tmp);
}

void	draw_between_sectors_walls3(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int *tmp)
{
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1
		&& left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& tmp[WINDOW_W / 2 - left->x] < WINDOW_H / 2)
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
}

void	draw_between_sectors_walls(t_sdl *iw, t_save_wall *left,
	t_save_wall *right)
{
	t_draw_line		l;
	int				lz;
	int				rz;
	int				*tmp;

	tmp = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	l.x0 = left->x;
	l.x1 = right->x;
	lz = get_floor_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x
		- left->wall->next->x, left->p.y + iw->walls
		[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_floor_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x
		- left->wall->next->x, right->p.y + iw->walls
		[left->wall->nextsector_wall].y - left->wall->next->y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz)
		/ (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz)
		/ (int)right->plen + iw->p.rotup;
	brez_line(tmp, l);
	draw_between_sectors_walls3(iw, left, right, tmp);
	draw_between_sectors_bot_tex(iw, left, right, tmp);
	draw_between_sectors_walls2(iw, left, right, tmp);
	free(tmp);
}
