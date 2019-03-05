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
