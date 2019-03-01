/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_moving.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:00:55 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/01 16:04:37 by dbolilyi         ###   ########.fr       */
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

void	move_in_portal(t_sdl *iw, t_move *d)
{
	int		nx;
	int		ny;
	int		savecs;
	int		nszu;
	int		nszd;

	nx = iw->p.x + d->dx + iw->walls[d->sw->nextsector_wall].x - d->sw->next->x;
	ny = iw->p.y + d->dy + iw->walls[d->sw->nextsector_wall].y - d->sw->next->y;
	savecs = iw->d.cs;
	iw->d.cs = d->sw->nextsector;
	nszu = get_ceil_z(iw, nx, ny);
	nszd = get_floor_z(iw, nx, ny);
	iw->d.cs = savecs;
	if (nszu - nszd >= PLAYER_HEIGHT + PLAYER_HEAD_SIZE - iw->v.crouch
		&& (nszd <= iw->p.z || nszd - iw->p.z
			+ PLAYER_HEIGHT < MAX_CLIMB_HEIGHT)
		&& (nszu >= iw->p.z + PLAYER_HEAD_SIZE - iw->v.crouch)
		&& in_sec_xy(iw, d->sw->nextsector, nx, ny))
	{
		iw->p.x = nx;
		iw->p.y = ny;
		iw->d.cs = d->sw->nextsector;
	}
	else
		move_collisions(iw, d->dx, d->dy, d->tmp);
}

void	move3(t_sdl *iw, t_move *d)
{
	if (in_sec_xy(iw, iw->d.cs, iw->p.x + d->dx, iw->p.y + d->dy))
	{
		if (in_sec_xy(iw, iw->d.cs, iw->p.x +
			d->dx * d->tmp, iw->p.y + d->dy * d->tmp)
			|| ((d->sw = is_wall_portal(iw, d->dx * d->tmp,
				d->dy * d->tmp)) != 0
				&& d->sw->glass < 0))
		{
			iw->p.x += d->dx + ((d->dx < 0) ? 1 : -1);
			iw->p.y += d->dy + ((d->dy < 0) ? 1 : -1);
		}
		else
			move_collisions(iw, d->dx, d->dy, d->tmp);
	}
	else if ((d->sw = is_wall_portal(iw, d->dx, d->dy))
			== 0 || d->sw->glass >= 0)
		move_collisions(iw, d->dx, d->dy, d->tmp);
	else
		move_in_portal(iw, d);
}

void	move2(t_sdl *iw, t_move *d)
{
	if (iw->v.fly_mode == 2)
	{
		if ((d->sw = is_wall_portal(iw, d->dx, d->dy)) == 0)
		{
			iw->p.x += d->dx;
			iw->p.y += d->dy;
		}
		else
		{
			iw->p.x += d->dx + iw->walls[d->sw->nextsector_wall].x
				- d->sw->next->x;
			iw->p.y += d->dy + iw->walls[d->sw->nextsector_wall].y
				- d->sw->next->y;
		}
		return ;
	}
	d->tmp = COLLISION_SIZE / (int)(sqrtf(powf((float)d->dx,
					2.0f) + powf((float)d->dy, 2.0f)) + 1.0f);
	move3(iw, d);
}

void	move1(t_move *d)
{
	if (d->ang < 90)
	{
		d->dx = (int)(d->speed * cosf(d->ang)) * 2;
		d->dy = (int)(-d->speed * sinf(d->ang)) * 2;
	}
	else if (d->ang < 180)
	{
		d->dx = (int)(-d->speed * cosf(G180 - d->ang)) * 2;
		d->dy = (int)(-d->speed * sinf(G180 - d->ang)) * 2;
	}
	else if (d->ang < 270)
	{
		d->dx = (int)(d->speed * cosf(d->ang) - G180) * 2;
		d->dy = (int)(-d->speed * sinf(d->ang) - G180) * 2;
	}
	else
	{
		d->dx = (int)(d->speed * cosf(G360 - d->ang)) * 2;
		d->dy = (int)(d->speed * sinf(G360 - d->ang)) * 2;
	}
}

void	move(t_sdl *iw, int pl, clock_t *time)
{
	t_move	d;

	d.ang = (iw->p.rot + (float)pl * G1);
	while (d.ang >= G360)
		d.ang -= G360;
	d.speed = (float)iw->v.sprint * MOVING_SPEED_PER_HALF_SEC *
		((float)(clock() - *time) / (float)CLKS_P_S);
	*time = clock();
	move1(&d);
	move2(iw, &d);
}
