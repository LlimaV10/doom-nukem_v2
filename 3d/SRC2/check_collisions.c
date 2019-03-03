#include "../guardians.h"

int		check_walls_collisions_on_line_segment(t_sdl *iw, int wall, int len)
{
	float	len_pifagor;
	float	len_p_p;

	len_p_p = powf(iw->walls[wall].x - iw->walls[wall].next->x, 2.0f) +
		powf(iw->walls[wall].y - iw->walls[wall].next->y, 2.0f);
	len_pifagor = powf(iw->p.x - iw->walls[wall].x, 2.0f)
		+ powf(iw->p.y - iw->walls[wall].y, 2.0f) - powf(len, 2.0f);
	if (len_pifagor > len_p_p)
		return (0);
	len_pifagor = powf(iw->p.x - iw->walls[wall].next->x, 2.0f)
		+ powf(iw->p.y - iw->walls[wall].next->y, 2.0f) -
		powf(len, 2.0f);
	if (len_pifagor > len_p_p)
		return (0);
	return (1);
}

void	check_walls_collisions2(t_sdl *iw, t_check_collisions *d)
{
	d->nx = iw->walls[d->wall].l.a;
	d->ny = iw->walls[d->wall].l.b;
	if ((iw->walls[d->wall].next->x > iw->walls[d->wall].x && d->ny > 0.0f) ||
		(iw->walls[d->wall].next->x < iw->walls[d->wall].x && d->ny < 0.0f))
	{
		d->nx = -d->nx;
		d->ny = -d->ny;
	}
	d->tmp = sqrtf(powf(d->nx, 2.0f) + powf(d->ny, 2.0f));
	d->nx *= (float)(COLLISION_SIZE2 - d->len) / d->tmp;
	d->ny *= (float)(COLLISION_SIZE2 - d->len) / d->tmp;
	if (in_sec_xy(iw, iw->d.cs, iw->p.x + (int)d->nx, iw->p.y + (int)d->ny))
	{
		iw->p.x += (int)d->nx;
		iw->p.y += (int)d->ny;
	}
}

void	check_walls_collisions(t_sdl *iw)
{
	t_check_collisions	d;

	if (iw->d.cs < 0)
		return;
	d.wall = iw->sectors[iw->d.cs].sw - 1;
	while (++d.wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		if ((iw->walls[d.wall].nextsector == -1 || iw->walls[d.wall].glass != -1)
			&& (iw->walls[d.wall].x != iw->walls[d.wall].next->x ||
				iw->walls[d.wall].y != iw->walls[d.wall].next->y))
		{
			d.len = (int)(fabsf(iw->walls[d.wall].l.a * iw->p.x
				+ iw->walls[d.wall].l.b * iw->p.y + iw->walls[d.wall].l.c) / sqrtf
				(powf(iw->walls[d.wall].l.a, 2.0f) + powf(iw->walls[d.wall].l.b, 2.0f)));
			if (d.len < COLLISION_SIZE2 &&
				check_walls_collisions_on_line_segment(iw, d.wall, d.len))
				check_walls_collisions2(iw, &d);
		}
	}
}