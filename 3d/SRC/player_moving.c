#include "../guardians.h"

t_wall	*is_wall_portal(t_sdl *iw, int dx, int dy)
{
	t_line2d	mv;
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
		if (iw->walls[wall].l.a * (float)(iw->p.x + dx) +
			iw->walls[wall].l.b * (float)(iw->p.y + dy) + iw->walls[wall].l.c > 0)
			side2 = 1;
		else
			side2 = -1;
		if (side1 * side2 < 0)
		{
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
				return (&iw->walls[wall]);
		}
	}
	return (0);
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
	else if (in_sec_xy(iw, iw->d.cs, iw->p.x, iw->p.y + dy * tmp) &&
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

int		check_moving_in_portal_z(t_sdl *iw, int dx, int dy, t_wall *sw)
{
	int		nx;
	int		ny;
	int		savecs;
	int		nszu;
	int		nszd;

	/*iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;*/
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

void	move_in_portal(t_sdl *iw, int dx, int dy, t_wall *sw, int tmp)
{
	int		nx;
	int		ny;
	int		savecs;
	int		nszu;
	int		nszd;

	/*iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;*/
	nx = iw->p.x + dx + iw->walls[sw->nextsector_wall].x - sw->next->x;
	ny = iw->p.y + dy + iw->walls[sw->nextsector_wall].y - sw->next->y;
	savecs = iw->d.cs;
	iw->d.cs = sw->nextsector;
	nszu = get_ceil_z(iw, nx, ny);
	nszd = get_floor_z(iw, nx, ny);
	iw->d.cs = savecs;
	if (nszu - nszd >= PLAYER_HEIGHT + PLAYER_HEAD_SIZE - iw->v.crouch
		&& (nszd <= iw->p.z || nszd - iw->p.z + PLAYER_HEIGHT < MAX_CLIMB_HEIGHT)
		&& (nszu >= iw->p.z + PLAYER_HEAD_SIZE - iw->v.crouch)
		&& in_sec_xy(iw, sw->nextsector, nx, ny))
	{
		iw->p.x = nx;
		iw->p.y = ny;
		iw->d.cs = sw->nextsector;
	}
	else
		move_collisions(iw, dx, dy, tmp);
}

void	move(t_sdl *iw, int pl, clock_t *time)
{
	float		ang;
	int		dx;
	int		dy;
	float	speed;
	t_wall	*sw;
	int	tmp;

	ang = (iw->p.rot + (float)pl * G1);
	while (ang >= G360)
		ang -= G360;
	speed = (float)iw->v.sprint * MOVING_SPEED_PER_HALF_SEC *
		((float)(clock() - *time) / (float)CLKS_P_S);
	*time = clock();
	if (ang < 90)
	{
		dx = (int)(speed * cosf(ang)) * 2;
		dy = (int)(-speed * sinf(ang)) * 2;
	}
	else if (ang < 180)
	{
		dx = (int)(-speed * cosf(G180 - ang)) * 2;
		dy = (int)(-speed * sinf(G180 - ang)) * 2;
	}
	else if (ang < 270)
	{
		dx = (int)(speed * cosf(ang) - G180) * 2;
		dy = (int)(-speed * sinf(ang) - G180) * 2;
	}
	else
	{
		dx = (int)(speed * cosf(G360 - ang)) * 2;
		dy = (int)(speed * sinf(G360 - ang)) * 2;
	}
	if (iw->v.fly_mode == 2)
	{
		if ((sw = is_wall_portal(iw, dx, dy)) == 0)
		{
			iw->p.x += dx;
			iw->p.y += dy;
		}
		else
		{
			iw->p.x += dx + iw->walls[sw->nextsector_wall].x - sw->next->x;
			iw->p.y += dy + iw->walls[sw->nextsector_wall].y - sw->next->y;
		}
		return;
	}
	tmp = COLLISION_SIZE / (int)(sqrtf(powf((float)dx, 2.0f) + powf((float)dy, 2.0f)) + 1.0f);
	if (in_sec_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y + dy))
	{
		if (in_sec_xy(iw, iw->d.cs, iw->p.x + dx * tmp , iw->p.y + dy * tmp)
			|| ((sw = is_wall_portal(iw, dx * tmp, dy * tmp)) != 0 && sw->glass < 0))
		{
			iw->p.x += dx + ((dx < 0) ? 1 : -1);
			iw->p.y += dy + ((dy < 0) ? 1 : -1);
		}
		else
			move_collisions(iw, dx, dy, tmp);
	}
	else if ((sw = is_wall_portal(iw, dx, dy)) == 0 || sw->glass >= 0)// || !check_moving_in_portal_z(iw, dx, dy, sw))
		move_collisions(iw, dx, dy, tmp);
	else
		move_in_portal(iw, dx, dy, sw, tmp);
}