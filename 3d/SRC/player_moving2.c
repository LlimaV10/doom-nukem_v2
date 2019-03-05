/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_moving2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:25:16 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:25:19 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		is_wall_portal2(t_sdl *iw, int dx, int dy, int wall)
{
	t_line2d	mv;
	int			side1;
	int			side2;

	mv.a = (float)dy;
	mv.b = (float)(-dx);
	mv.c = -mv.a * (float)iw->p.x - mv.b * (float)iw->p.y;
	if (mv.a * (float)iw->walls[wall].x +
		mv.b * (float)iw->walls[wall].y + mv.c > 0)
		side1 = 1;
	else
		side1 = -1;
	if (mv.a * (float)iw->walls[wall].next->x +
		mv.b * (float)iw->walls[wall].next->y + mv.c > 0)
		side2 = 1;
	else
		side2 = -1;
	if (side1 * side2 < 0)
		return (1);
	return (0);
}

t_wall	*is_wall_portal(t_sdl *iw, int dx, int dy)
{
	int			wall;
	int			side1;
	int			side2;

	if (iw->d.cs < 0)
		return (0);
	wall = iw->sectors[iw->d.cs].sw - 1;
	while (++wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		if (iw->walls[wall].nextsector == -1)
			continue;
		if (iw->walls[wall].l.a * (float)iw->p.x +
			iw->walls[wall].l.b * (float)iw->p.y + iw->walls[wall].l.c > 0)
			side1 = 1;
		else
			side1 = -1;
		side2 = ((iw->walls[wall].l.a * (float)(iw->p.x + dx) +
			iw->walls[wall].l.b * (float)(iw->p.y + dy)
			+ iw->walls[wall].l.c > 0) ? 1 : -1);
		if (side1 * side2 < 0 && is_wall_portal2(iw, dx, dy, wall))
			return (&iw->walls[wall]);
	}
	return (0);
}

void	move_collisions2(t_sdl *iw, int dx, int dy, int tmp)
{
	int		dd;
	int		i;

	if (in_sec_xy(iw, iw->d.cs, iw->p.x, iw->p.y + dy * tmp) &&
		in_sec_xy(iw, iw->d.cs, iw->p.x, iw->p.y + dy))
	{
		dd = dx / 20 * 2;
		dx = 0;
		i = -1;
		while (++i < 10 &&
			in_sec_xy(iw, iw->d.cs, iw->p.x + dx * tmp, iw->p.y + dy * tmp)
			&& in_sec_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y + dy))
			dx += dd;
		iw->p.x += dx - dd;
		iw->p.y += dy;
	}
}

void	move_collisions(t_sdl *iw, int dx, int dy, int tmp)
{
	int		dd;
	int		i;

	if (in_sec_xy(iw, iw->d.cs, iw->p.x + dx * tmp, iw->p.y) &&
		in_sec_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y))
	{
		dd = dy / 20 * 2;
		dy = 0;
		i = -1;
		while (++i < 10 &&
			in_sec_xy(iw, iw->d.cs, iw->p.x + dx * tmp, iw->p.y + dy * tmp)
			&& in_sec_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y + dy))
			dy += dd;
		iw->p.x += dx;
		iw->p.y += dy - dd;
	}
	else
		move_collisions2(iw, dx, dy, tmp);
}

int		check_moving_in_portal_z(t_sdl *iw, int dx, int dy, t_wall *sw)
{
	int		nx;
	int		ny;
	int		savecs;
	int		nszu;
	int		nszd;

	nx = iw->p.x + dx + iw->walls[sw->nextsector_wall].x - sw->next->x;
	ny = iw->p.y + dy + iw->walls[sw->nextsector_wall].y - sw->next->y;
	savecs = iw->d.cs;
	iw->d.cs = sw->nextsector;
	nszu = get_ceil_z(iw, nx, ny);
	nszd = get_floor_z(iw, nx, ny);
	iw->d.cs = savecs;
	if (nszu - nszd >= PLAYER_HEIGHT + PLAYER_HEAD_SIZE
		&& (nszd < iw->p.z || nszd - iw->p.z + PLAYER_HEIGHT < MAX_CLIMB_HEIGHT)
		&& in_sec_xy(iw, sw->nextsector, nx, ny))
		return (1);
	return (0);
}
