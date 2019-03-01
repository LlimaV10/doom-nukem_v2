#include "../guardians.h"

int		check_walls_collisions_on_line_segment(t_sdl *iw, int wall, int len)
{
	//float	len_point;
	float	len_pifagor;
	float	len_p_p;

	len_p_p = powf(iw->walls[wall].x - iw->walls[wall].next->x, 2.0f) +
		powf(iw->walls[wall].y - iw->walls[wall].next->y, 2.0f);
	//len_point = sqrtf(powf(iw->p.x - iw->walls[wall].x, 2.0f) + powf(iw->p.y - iw->walls[wall].y, 2.0f));
	len_pifagor = powf(iw->p.x - iw->walls[wall].x, 2.0f) + powf(iw->p.y - iw->walls[wall].y, 2.0f) -
		powf(len, 2.0f);
	if (len_pifagor > len_p_p)
		return (0);
	len_pifagor = powf(iw->p.x - iw->walls[wall].next->x, 2.0f) + powf(iw->p.y - iw->walls[wall].next->y, 2.0f) -
		powf(len, 2.0f);
	if (len_pifagor > len_p_p)
		return (0);
	return (1);
}

void	check_walls_collisions(t_sdl *iw)
{
	int		wall;
	int		len;
	float	nx;
	float	ny;
	float	tmp;

	if (iw->d.cs < 0)
		return;
	wall = iw->sectors[iw->d.cs].sw - 1;
	while (++wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		if ((iw->walls[wall].nextsector == -1 || iw->walls[wall].glass != -1) && (iw->walls[wall].x != iw->walls[wall].next->x || 
			iw->walls[wall].y != iw->walls[wall].next->y))
		{
			len = (int)(fabsf(iw->walls[wall].l.a * iw->p.x + iw->walls[wall].l.b * iw->p.y + iw->walls[wall].l.c) /
				sqrtf(powf(iw->walls[wall].l.a, 2.0f) + powf(iw->walls[wall].l.b, 2.0f)));
			if (len < COLLISION_SIZE2)
			{
				if (!check_walls_collisions_on_line_segment(iw, wall, len))
					continue;
				nx = iw->walls[wall].l.a;
				ny = iw->walls[wall].l.b;
				if ((iw->walls[wall].next->x > iw->walls[wall].x && ny > 0.0f) ||
					(iw->walls[wall].next->x < iw->walls[wall].x && ny < 0.0f))
				{
					nx = -nx;
					ny = -ny;
				}
				tmp = sqrtf(powf(nx, 2.0f) + powf(ny, 2.0f));
				nx *= (float)(COLLISION_SIZE2 - len) / tmp;
				ny *= (float)(COLLISION_SIZE2 - len) / tmp;
				if (in_sec_xy(iw, iw->d.cs, iw->p.x + (int)nx, iw->p.y + (int)ny))
				{
					iw->p.x += (int)nx;
					iw->p.y += (int)ny;
				}
			}
		}
	}
}