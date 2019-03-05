/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_get_between_sectors_walls.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:02:46 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:02:47 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_between_sectors_walls2(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int **top)
{
	int				lz;
	int				rz;
	t_draw_line		l;

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
	brez_line(*top, l);
}

int		*get_between_sectors_walls(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int **top)
{
	t_draw_line		l;
	int				lz;
	int				rz;
	int				*bottom;

	bottom = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	*top = (int *)malloc((right->x - left->x + 1) * sizeof(int));
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
	brez_line(bottom, l);
	get_between_sectors_walls2(iw, left, right, top);
	return (bottom);
}

void	fill_portal(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_sdl *iw2)
{
	int		j;

	j = left->x - 1;
	while (++j < right->x)
	{
		if (iw2->d.top[j] > iw->d.top[j])
			iw->d.top[j] = iw2->d.top[j];
		if (iw2->d.bottom[j] < iw->d.bottom[j])
			iw->d.bottom[j] = iw2->d.bottom[j];
	}
}

void	fill_top_skybox(t_sdl *iw2, t_save_wall *left, int len)
{
	int		i;

	i = -1;
	while (++i < len)
		if (iw2->d.wallTop[i] > iw2->d.top[left->x + i])
			iw2->d.top[left->x + i] = iw2->d.wallTop[i];
}

void	fill_tb_by_slsr(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < iw->d.screen_left)
		iw->d.top[i] = iw->d.bottom[i];
	i = iw->d.screen_right - 1;
	while (++i <= WINDOW_W)
		iw->d.top[i] = iw->d.bottom[i];
}
