#include "guardians.h"
#include <stdio.h>

void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	if (x >= 0 && x < WINDOW_W && y >= 0 && y < WINDOW_H)
	{
		Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
		*(Uint32 *)target_pixel = pixel;
	}
}

void	exit_x(t_sdl *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	SDL_Quit();
	system("leaks doom-nukem");
	exit(0);
}

void	key_down(int code, t_sdl *iw)
{
	//printf("keydown = %d\n", code);
	if (code == 41)
		exit_x(iw);
}

void	main_loop(t_sdl *iw)
{
	SDL_Event e;

	iw->quit = 0;
	while (!iw->quit)
	{
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT)
				iw->quit = 1;
			else if (e.type == SDL_KEYDOWN)//&& e.key.repeat == 0)
				key_down(e.key.keysym.scancode, iw);
			//else if (e.type == SDL_KEYUP)
			//	key_up(e.key.keysym.scancode, iw);
		/*else if (e.type == SDL_MOUSEMOTION)
			mouse_move(e.motion.xrel, e.motion.yrel, iw);*/
		//loop(iw);
	}
}

void	get_wall_line(t_sdl *iw, int wall)
{
	//if (iw->walls[wall].x == iw->walls[wall].next->x)
	//{
	//	iw->walls[wall].l.a = 0.0f;
	//	iw->walls[wall].l.b = 1.0f;
	//	iw->walls[wall].l.c = -1.0f * iw->walls[wall].y;
	//}
	//else if (iw->walls[wall].y == iw->walls[wall].next->y)
	//{
	//	iw->walls[wall].l.b = 0.0f;
	//	iw->walls[wall].l.a = 1.0f;
	//	iw->walls[wall].l.c = -1.0f * iw->walls[wall].x;
	//}
	//else
	//{
	iw->walls[wall].l.a = (float)(iw->walls[wall].next->y - iw->walls[wall].y);
	iw->walls[wall].l.b = (float)(iw->walls[wall].x - iw->walls[wall].next->x);
	iw->walls[wall].l.c = (float)(iw->walls[wall].next->x * iw->walls[wall].y -
		iw->walls[wall].x * iw->walls[wall].next->y);
	//}
}

void	create_map(t_sdl *iw)
{
	int		i;
	int		j;

	i = -1;
	while (++i < iw->v.sc)
	{
		if (iw->sectors[i].fr.n != 0)
			iw->sectors[i].fr.n->d = -iw->sectors[i].fr.n->a * iw->sectors[i].fr.x -
				iw->sectors[i].fr.n->b * iw->sectors[i].fr.y - iw->sectors[i].fr.n->c * iw->sectors[i].fr.z; 
		if (iw->sectors[i].cl.n != 0)
			iw->sectors[i].cl.n->d = -iw->sectors[i].cl.n->a * iw->sectors[i].cl.x -
				iw->sectors[i].cl.n->b * iw->sectors[i].cl.y - iw->sectors[i].cl.n->c * iw->sectors[i].cl.z;
		j = iw->sectors[i].sw - 1;
		while (++j < iw->sectors[i].sw + iw->sectors[i].nw)
			get_wall_line(iw, j);
	}
}

int inside_sector(int sector, t_sdl *iw)
{
	long int i, x1, y1, x2, y2;
	unsigned long int wallCrossed;

	//Quick check if the sector ID is valid.

	wallCrossed = 0;
	i = iw->sectors[sector].sw - 1;
	while (++i < iw->sectors[sector].sw + iw->sectors[sector].nw)
	{
		y1 = iw->walls[i].y - iw->p.y;
		y2 = iw->walls[i].next->y - iw->p.y;
		// y2 = iw->walls[iw->walls[i].nextwall].y - iw->p.y;
		if ((y1 ^ y2) < 0)
		{
			x1 = iw->walls[i].x - iw->p.x;
			x2  =iw->walls[i].next->x - iw->p.x;
			//x2 = iw->walls[iw->walls[i].nextwall].x - iw->p.x;
			if ((x1 ^ x2) >= 0)
				wallCrossed ^= x1;
			else
				wallCrossed ^= (x1 * y2 - x2 * y1) ^ y2;
		}
	}
	return (wallCrossed >> 31);
}


int		inside_sectorZ(int sector, t_sdl *iw)
{
	// int		maxz;
	// int		minz;
	// int		i;

	// i = iw->sectors[sector].sw;
	// maxz = iw->walls[i].zu;
	// minz = iw->walls[i].zd;
	// while (++i < iw->sectors[sector].startWall + iw->sectors[sector].numWalls)
	// {
	// 	if (iw->walls[i].zu > maxz)
	// 		maxz = iw->walls[i].zu;
	// 	if (iw->walls[i].zd < minz)
	// 		minz = iw->walls[i].zd;
	// }
	// if (iw->p.z <= maxz && iw->p.z >= minz)
	// 	return (1);
	// return (0);
	return (1);
}

int		get_sector(t_sdl *iw)
{
	int sec;

	sec = iw->v.ls - 1;
	while (++sec < iw->v.sc)
		if (inside_sector(sec, iw) != 0 && inside_sectorZ(sec, iw) == 1)
			return (sec);
	sec = -1;
	while (++sec < iw->v.ls)
		if (inside_sector(sec, iw) != 0 && inside_sectorZ(sec, iw) == 1)
			return (sec);
	return (-1);
}

void	set_top_bottom(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < WINDOW_W)
	{
		iw->d.top[i] = 0;
		iw->d.bottom[i] = WINDOW_H;
	}
}

float	get_k_angle(float rot)
{
	if (rot < G180)
		return (G180 - rot);
	else
		return (G360 - rot);
}

void	get_view_line(t_sdl *iw)
{
	float	k;

	k = get_k_angle(iw->p.rot);
	if (tanf(k) == 0.0f)
	{
		iw->d.view.a = 0.0f;
		iw->d.view.b = 1.0f;
		iw->d.view.c = -1.0f * (float)iw->p.y;
	}
	else if (tanf(k) == INFINITY)
	{
		iw->d.view.b = 0.0f;
		iw->d.view.a = 1.0f;
		iw->d.view.c = -1.0f * (float)iw->p.x;
	}
	else
	{
		iw->d.view.a = tanf(k);
		iw->d.view.b = -1.0f;
		iw->d.view.c = (float)iw->p.y - iw->d.view.a * (float)iw->p.x;
	}
}

void	get_screen_line(t_sdl *iw, float len)
{
	float	diskr;

	if (iw->d.view.a == 0.0f)
	{
		iw->d.screen_point.y = (float)iw->p.y;
		iw->d.screen_point.x = (float)iw->p.x + ((iw->d.view_dir.x > 0) ? len : -len);
		iw->d.screen.b = 0.0f;
		iw->d.view.a = 1.0f;
		iw->d.view.c = -1.0f * iw->d.screen_point.x;
	}
	else if (iw->d.view.b == 0.0f)
	{
		iw->d.screen_point.x = (float)iw->p.x;
		iw->d.screen_point.y = (float)iw->p.y + ((iw->d.view_dir.y > 0) ? len : -len);
		iw->d.screen.a = 0.0f;
		iw->d.screen.b = 1.0f;
		iw->d.screen.c = -1.0f * iw->d.screen_point.y;
	}
	else
	{
		if (iw->d.view_dir.x > 0)
			iw->d.screen_point.y = (sqrtf(powf(iw->d.view.a * len, 2.0f) -
				powf(iw->d.view.a * (float)iw->p.x, 2.0f) -
				2.0f * iw->d.view.a * iw->d.view.b * (float)(iw->p.x * iw->p.y) -
				2.0f * iw->d.view.a * iw->d.view.c * (float)iw->p.x +
				powf(iw->d.view.b * len, 2.0f) - powf(iw->d.view.b * (float)iw->p.y, 2.0f) -
				2.0f * iw->d.view.b * iw->d.view.c * (float)iw->p.y -
				powf(iw->d.view.c, 2.0f)) / iw->d.view.a -
				(iw->d.view.b * iw->d.view.c) / iw->d.view.a / iw->d.view.a -
				iw->d.view.b * (float)iw->p.x / iw->d.view.a + (float)iw->p.y) /
				(powf(iw->d.view.b / iw->d.view.a, 2.0f) + 1.0f);
		else
			iw->d.screen_point.y = (-sqrtf(powf(iw->d.view.a * len, 2.0f) -
				powf(iw->d.view.a * (float)iw->p.x, 2.0f) -
				2.0f * iw->d.view.a * iw->d.view.b * (float)(iw->p.x * iw->p.y) -
				2.0f * iw->d.view.a * iw->d.view.c * (float)iw->p.x +
				powf(iw->d.view.b * len, 2.0f) - powf(iw->d.view.b * (float)iw->p.y, 2.0f) -
				2.0f * iw->d.view.b * iw->d.view.c * (float)iw->p.y -
				powf(iw->d.view.c, 2.0f)) / iw->d.view.a -
				(iw->d.view.b * iw->d.view.c) / iw->d.view.a / iw->d.view.a -
				iw->d.view.b * (float)iw->p.x / iw->d.view.a + (float)iw->p.y) /
				(powf(iw->d.view.b / iw->d.view.a, 2.0f) + 1.0f);
		/*if (iw->d.view_dir.x > 0)
			iw->d.screen_point.x = (float)iw->p.x + sqrtf(len * len - powf(fabsf((float)iw->p.y - iw->d.screen_point.y), 2.0f));
		else
			iw->d.screen_point.x = (float)iw->p.x - sqrtf(len * len - powf(fabsf((float)iw->p.y - iw->d.screen_point.y), 2.0f));*/
		iw->d.screen_point.x = (iw->d.view.c + iw->d.view.b * iw->d.screen_point.y) / (-iw->d.view.a);
		// angle of screen line
		diskr = iw->p.rot + G90;
		if (diskr > G360)
			diskr -= G360;
		diskr = get_k_angle(diskr);
		iw->d.screen.a = tanf(diskr);
		iw->d.screen.b = -1.0f;
		iw->d.screen.c = iw->d.screen_point.y - iw->d.screen.a * iw->d.screen_point.x;
		len = sqrtf(powf((float)iw->p.x - iw->d.screen_point.x, 2.0f) + powf((float)iw->p.y - iw->d.screen_point.y, 2.0f));
		iw->d.screen_length = (float)WINDOW_W / (float)WINDOW_H * len / 2.0f;
		iw->d.pss = iw->d.screen.a * (float)iw->p.x + iw->d.screen.b * (float)iw->p.y + iw->d.screen.c;
	}
}

void	get_direction(t_sdl *iw)
{
	if (iw->p.rot > G90 && iw->p.rot < G90 * 3)
		iw->d.view_dir.x = -1;
	else
		iw->d.view_dir.x = 1;
	if (iw->p.rot < G180)
		iw->d.view_dir.y = -1;
	else
		iw->d.view_dir.y = 1;
}

void	add_wall(t_sdl *iw, t_save_wall *tmp)
{
	t_save_wall	*tmp2;

	if (iw->d.vw == 0 || iw->d.vw->len > tmp->len)
	{
		tmp->next = iw->d.vw;
		iw->d.vw = tmp;
	}
	else
	{
		tmp2 = iw->d.vw;
		while (tmp2->next != 0 && tmp2->next->len <= tmp->len)
			tmp2 = tmp2->next;
		if (tmp2->next != 0)
			tmp->next = tmp2->next;
		tmp2->next = tmp;
	}
}

int		get_floor_z(t_sdl *iw, int x, int y)
{
	if (iw->sectors[iw->d.cs].fr.n == 0)
		return (iw->sectors[iw->d.cs].fr.z);
	else
		return ((iw->sectors[iw->d.cs].fr.n->a * (float)x + iw->sectors[iw->d.cs].fr.n->b *
			(float)y + iw->sectors[iw->d.cs].fr.n->d) / (-iw->sectors[iw->d.cs].fr.n->c));
}

int		get_ceil_z(t_sdl *iw, int x, int y)
{
	if (iw->sectors[iw->d.cs].cl.n == 0)
		return (iw->sectors[iw->d.cs].cl.z);
	else
		return ((iw->sectors[iw->d.cs].cl.n->a * (float)x + iw->sectors[iw->d.cs].cl.n->b *
			(float)y + iw->sectors[iw->d.cs].cl.n->d) / (-iw->sectors[iw->d.cs].cl.n->c));
}

void	get_visible_walls2(t_sdl *iw, float clen, int wall)
{
	float		side;
	t_save_wall	*w;
	
	w = (t_save_wall *)malloc(sizeof(t_save_wall));
	/*if ((d->vk * iw->walls[d->sw].x - iw->walls[d->sw].y + d->vb) * d->xdir < 0)
		tmp->x = (int)((float)(WINDOW_W / 2) + ((float)(WINDOW_W / 2) * d->sx / d->ssz));
	else
		tmp->x = (int)((float)(WINDOW_W / 2) - ((float)(WINDOW_W / 2) * d->sx / d->ssz));*/
	side = iw->d.view.a * (float)iw->walls[wall].x + iw->d.view.b * (float)iw->walls[wall].y + iw->d.view.c;
	if ((iw->d.view_dir.x < 0 && side < 0) || (iw->d.view_dir.x > 0 && side > 0))
		w->x = WINDOW_W / 2 + (int)((float)WINDOW_W / 2.0f * clen / iw->d.screen_length);
	else
		w->x = WINDOW_W / 2 - (int)((float)WINDOW_W * clen / iw->d.screen_length / 2.0f);
	w->len = sqrtf(powf((float)(iw->p.x - iw->walls[wall].x), 2.0f) +  powf((float)(iw->p.y - iw->walls[wall].y), 2.0f));
	w->plen = fabsf(iw->d.screen.a * (float)iw->walls[wall].x + iw->d.screen.b * (float)iw->walls[wall].y + iw->d.screen.c) /
				sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	w->wall = &iw->walls[wall];
	w->zu = get_ceil_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->zd = get_floor_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->next = 0;
	add_wall(iw, w);
}

void	get_visible_walls(t_sdl *iw)
{
	int		wall;
	float	side;
	t_point2d	p;

	wall = iw->sectors[iw->d.cs].sw;
	while (wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		side = iw->d.screen.a * (float)iw->walls[wall].x + iw->d.screen.b * (float)iw->walls[wall].y + iw->d.screen.c;
		/*if ((iw->d.view_dir.x < 0 && side < 0) || (iw->d.view_dir.x > 0 && side > 0))
		if ((iw->walls[wall].x - iw->p.x) ^ iw->d.view_dir.x >= 0 && (iw->walls[wall].y - iw->p.y) ^ iw->d.view_dir.y >= 0)*/
		if (side * iw->d.pss <= 0)
		{
			p.x = (iw->d.screen.c / iw->d.screen.b * (float)(iw->walls[wall].x - iw->p.x) + (float)(iw->walls[wall].x * iw->p.y - iw->walls[wall].y * iw->p.x)) /
				((float)(iw->p.y - iw->walls[wall].y) - iw->d.screen.a / iw->d.screen.b * (float)(iw->walls[wall].x - iw->p.x));
			p.y = (iw->d.screen.a * p.x + iw->d.screen.c) / (-iw->d.screen.b);
			side = sqrtf(powf(p.x - iw->d.screen_point.x, 2.0f) + powf(p.y - iw->d.screen_point.y, 2.0f));
			//printf("wall %d len_toscr_center %f", wall, side);
			if (side <= iw->d.screen_length)
				get_visible_walls2(iw, side, wall);
				/*printf("x %d y %d len %f\n", iw->walls[wall].x, iw->walls[wall].y, side);*/
		}
		wall++;
	}
}

int		cross_two_lines(t_line2d *l1, t_line2d *l2, t_intpoint2d *p)
{
	t_line2d	*tmp;

	if (l1->a == l2->a && l1->b == l2->b)
		return (0);
	if (l1->a == 0 && l2->a != 0)
	{
		tmp = l1;
		l1 = l2;
		l2 = tmp;
	}
	else if (l1->a == 0)
		return (0);
	p->y = (l2->a * l1->c - l1->a * l2->c) / (l1->a * l2->b - l2->a * l1->b);
	if (l2->b == 0 && l2->a != 0)
		p->x = (l2->b * p->y + l2->c) / (-l2->a);
	else
		p->x = (l1->b * p->y + l1->c) / (-l1->a);
	return (1);
}

int		ft_min(int p1, int p2)
{
	if (p1 < p2)
		return (p1);
	return (p2);
}

int		ft_max(int p1, int p2)
{
	if (p1 >= p2)
		return (p1);
	return (p2);
}

int		point_in_front_and_on_wall(t_sdl *iw, t_intpoint2d *p, int wall)
{
	float	side;

	//(((d->rx - iw->p.x) * d->xdir) >= 0) && (((d->ry - iw->p.y) * d->ydir) >= 0)
	side = iw->d.screen.a * p->x + iw->d.screen.b * p->y + iw->d.screen.c;
	//if ((iw->d.view_dir.x < 0 && side < 0) || (iw->d.view_dir.x > 0 && side > 0))
	//if (((int)p->x - iw->p.x) ^ iw->d.view_dir.x >= 0 && ((int)p->y - iw->p.y) ^ iw->d.view_dir.y >= 0)
	if (side * iw->d.pss <= 0)
		if ((p->x >= ft_min(iw->walls[wall].x, iw->walls[wall].next->x) && p->x <= ft_max(iw->walls[wall].x, iw->walls[wall].next->x))
			&& (p->y >= ft_min(iw->walls[wall].y, iw->walls[wall].next->y) && p->y <= ft_max(iw->walls[wall].y, iw->walls[wall].next->y)))
			return (1);
	return (0);
}

int		visible_wall(t_sdl *iw, int wall)
{
	if ((iw->p.x - iw->walls[wall].x) * (iw->walls[wall].next->y - iw->walls[wall].y) -
			(iw->p.y - iw->walls[wall].y) * (iw->walls[wall].next->x - iw->walls[wall].x) >= 0)
		return (1);
	return (0);
}

void	add_lr_wall(t_sdl *iw, t_intpoint2d *p, t_wall *wall, int x)
{
	t_save_wall	*tmp;

	tmp = (t_save_wall *)malloc(sizeof(t_save_wall));
	tmp->x = x;
	tmp->wall = wall;
	tmp->len = sqrtf(powf((float)(iw->p.x - p->x), 2.0f) + powf((float)(iw->p.y - p->y), 2.0f));
	tmp->plen = fabsf(iw->d.screen.a * (float)p->x + iw->d.screen.b * (float)p->y + iw->d.screen.c) /
				sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	tmp->zd = get_floor_z(iw, p->x, p->y);
	tmp->zu = get_ceil_z(iw, p->x, p->y);
	tmp->next = 0;
	add_wall(iw, tmp);
}

void	get_all_intersection_line(t_sdl *iw, t_line2d *nl, int right)
{
	int		sec;
	int		wall;
	t_intpoint2d	p;

	sec = -1;
	while (++sec < iw->v.sc)
	{
		wall = iw->sectors[sec].sw - 1;
		while (++wall < iw->sectors[sec].sw + iw->sectors[sec].nw)
			if (visible_wall(iw, wall) && cross_two_lines(nl, &iw->walls[wall].l, &p)
				&& point_in_front_and_on_wall(iw, &p, wall))
				add_lr_wall(iw, &p, (right == 0) ? &iw->walls[wall] : iw->walls[wall].next, right * WINDOW_W);
	}
}

void	get_left_right_visible_walls(t_sdl *iw)
{
	float	angle;
	float	na;
	t_line2d	nl;

	angle = atanf(iw->d.screen_length / 1.0f);
	na = iw->p.rot - angle;
	if (na < 0.0f)
		na += G360;
	na = get_k_angle(na);
	nl.a = tanf(na);
	nl.b = -1.0f;
	nl.c = (float)iw->p.y - nl.a * (float)iw->p.x;

	/// LEFT
	get_all_intersection_line(iw, &nl, 0);

	na = iw->p.rot + angle;
	if (na > G360)
		na -= G360;
	na = get_k_angle(na);
	nl.a = tanf(na);
	nl.b = -1.0f;
	nl.c = (float)iw->p.y - nl.a * (float)iw->p.x;

	// RIGHT
	get_all_intersection_line(iw, &nl, 1);
}

t_save_wall	*find_next_vis_wall (t_sdl *iw, t_save_wall *left)
{
	t_save_wall	*right;

	right = iw->d.vw;
	while (right != 0)
	{
		if (right->wall == left->wall->next)
			break;
		right = right->next;
	}
	return (right);
}

void	put_wall_pixel(t_brez *b, int x, int y)
{
	if (x != b->prev_x)
	{
		*(b->wall_y++) = y;
		b->prev_x = x;
	}
}

void	print_brez(t_brez *b, int d, int d1, int d2)
{
	int i;

	if (b->k <= 0)
		b->dx = b->dy;
	i = 0;
	while (++i <= b->dx)
	{
		if (d > 0)
		{
			d += d2;
			b->y += b->sy;
		}
		else
			d += d1;
		if (b->k > 0)
			/*pixel_put_img(b->iw, b->x, b->y, b->color);*/
			put_wall_pixel(b, b->x, b->y);
		else
			put_wall_pixel(b, b->y, b->x);
			/*pixel_put_img(b->iw, b->y, b->x, b->color);*/
		b->x += b->sx;
	}
}

void	brez_line(int *wall_y, t_draw_line line)
{
	t_brez	b;

	/*pixel_put_img(iw, line.x0, line.y0, color);*/
	*(wall_y++) = line.y0;
	b.prev_x = line.x0;
	b.wall_y = wall_y;
	b.sx = (line.x1 >= line.x0) ? (1) : (-1);
	b.sy = (line.y1 >= line.y0) ? (1) : (-1);
	b.dx = (line.x1 > line.x0) ? (line.x1 - line.x0) : (line.x0 - line.x1);
	b.dy = (line.y1 > line.y0) ? (line.y1 - line.y0) : (line.y0 - line.y1);
	if (abs(line.y1 - line.y0) < abs(line.x1 - line.x0))
	{
		b.x = line.x0 + b.sx;
		b.y = line.y0;
		b.k = 1;
		print_brez(&b, (b.dy * 2) - b.dx, b.dy * 2, (b.dy - b.dx) * 2);
	}
	else
	{
		b.x = line.y0 + b.sy;
		b.y = line.x0;
		b.sy = (line.x1 >= line.x0) ? (1) : (-1);
		b.sx = (line.y1 >= line.y0) ? (1) : (-1);
		b.k = 0;
		print_brez(&b, (b.dx * 2) - b.dy, b.dx * 2, (b.dx - b.dy) * 2);
	}
}

void	draw_wall(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		i = iw->d.wallTop[j] - 1;
		while (++i < iw->d.wallBot[j])
			set_pixel(iw->sur, left->x + j, i, 0x00FF00);
	}
}

void	draw_all(t_sdl *iw, t_save_wall *left, int len)
{
	draw_wall(iw, left, len);
}

void	draw_left_right(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_draw_line		l;

	if (left->x >= right->x)
		return;
	iw->d.wallTop = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	iw->d.wallBot = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	if (!iw->d.wallTop || !iw->d.wallBot)
		return;
	l.x0 = left->x;
	l.x1 = right->x;
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zd) / (int)left->plen;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zd) / (int)right->plen;
	brez_line(iw->d.wallBot, l);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zu) / (int)left->plen;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zu) / (int)right->plen;
	brez_line(iw->d.wallTop, l);
	draw_all(iw, left, right->x - left->x + 1);
	//int i = -1;
	//while (++i < right->x - left->x + 1)
	//	printf("y[%d] = %d\n", i, iw->d.wallTop[i]);
	
	free(iw->d.wallBot);
	free(iw->d.wallTop);
}

void	draw_start(t_sdl *iw)
{
	t_save_wall *left;
	t_save_wall	*right;

	if (iw->d.vw == 0)
		return;
	left = iw->d.vw;
	while (left != 0)
	{
		if ((right = find_next_vis_wall(iw, left)) != 0)
			draw_left_right(iw, left, right);
		left = left->next;
	}
}

void	draw(t_sdl *iw)
{
	set_top_bottom(iw);
	if ((iw->d.cs = get_sector(iw)) == -1)
		return ;
	get_direction(iw);
	get_view_line(iw);
	get_screen_line(iw, 1.0f);
	iw->d.vw = 0;
	get_visible_walls(iw);
	get_left_right_visible_walls(iw);

	//////////
	t_save_wall *tmp;
	tmp = iw->d.vw;
	while (tmp != 0)
	{
		printf("x %d len %f xw %d yw %d zu %d zd %d plen %f\n", tmp->x, tmp->len, tmp->wall->x, tmp->wall->y, tmp->zu,
			tmp->zd, tmp->plen);
		tmp = tmp->next;
	}
	printf("\n\n");

	////////////

	draw_start(iw);
}

void	get_def(t_sdl *iw)
{
	iw->p.x = 500;
	iw->p.y = 500;
	iw->p.z = 200;
	iw->p.rot = G360 - G90 * 30 / 90;
	iw->p.rotup = 0.0f;
	iw->v.ls = 0;
}

int		main(void)
{
	t_sdl	iw;

	get_def(&iw);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetRelativeMouseMode(0);
	iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	iw.sur = SDL_GetWindowSurface(iw.win);
	// draw
	get_map(&iw);
	create_map(&iw);
	draw(&iw);
	// t_wall *w = iw.walls;
	// int i = 0;
	// while (w && i < 6)
	// {
	// 	printf("x %d y %d\n", w->x, w->y);
	// 	w = w->next;
	// 	i++;
	// }
	// draw
	SDL_UpdateWindowSurface(iw.win);
	main_loop(&iw);
	SDL_FreeSurface(iw.sur);
	SDL_DestroyWindow(iw.win);
	SDL_Quit();
	// system("PAUSE");
	return (0);
}