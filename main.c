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

Uint32	get_pixel(SDL_Surface *sur, const int x, const int y)
{
	uint8_t *v;
	int		bpp;

	if (x < 0 || x >= sur->w || y < 0 || y >= sur->h)
		return ;
	bpp = sur->format->BytesPerPixel;
	v = (uint8_t *)sur->pixels + y * sur->pitch + x * bpp;
	/*printf("bpp %d\n", bpp);
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		printf("big endian\n");
	else
		printf("small endian\n");*/
		/*if (bpp == 1)
			return (*p);
		if (bpp == 2)
			return (*(uint16_t *)p);
		if (bpp == 3)
		{
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return (p[0] << 16 | p[1] << 8 | p[2]);
			else*/
	return (v[0] | v[1] << 8 | v[2] << 16);
	/*}
	if (bpp == 4)
		return (*(uint32_t *)p);
	return (0);*/
}

void	exit_x(t_sdl *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	SDL_Quit();
	system("leaks doom-nukem");
	exit(0);
}

void	draw(t_sdl *iw);

void	update(t_sdl *iw)
{
	SDL_FillRect(iw->sur, NULL, 0x000000);
	draw(iw);
	//printf("Update\n");
	SDL_UpdateWindowSurface(iw->win);
}

void	key_down(int code, t_sdl *iw)
{
	printf("keydown = %d\n", code);
	if (code == 41)
		exit_x(iw);
	else if (code == 79)
	{
		iw->p.introt += 4;
		if (iw->p.introt > 360)
			iw->p.introt -= 360;
		iw->p.rot = (float)iw->p.introt * G1;
		update(iw);
	}
	else if (code == 80)
	{
		iw->p.introt -= 4;
		if (iw->p.introt < 0)
			iw->p.introt += 360;
		iw->p.rot = (float)iw->p.introt * G1;
		update(iw);
	}
	else if (code == 26)
	{
		iw->p.x += 60;
		update(iw);
	}
	else if (code == 22)
	{
		iw->p.x -= 60;
		update(iw);
	}
	else if (code == 4)
	{
		iw->p.y += 60;
		update(iw);
	}
	else if (code == 7)
	{
		iw->p.y -= 60;
		update(iw);
	}
	else if (code == 8)
	{
		iw->p.z += 60;
		update(iw);
	}
	else if (code == 20)
	{
		iw->p.z -= 60;
		update(iw);
	}
	printf("rot = %d px %d py %d pz %d\n", iw->p.introt, iw->p.x, iw->p.y, iw->p.z);
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
	iw->walls[wall].l.a = (float)(iw->walls[wall].next->y - iw->walls[wall].y);
	iw->walls[wall].l.b = (float)(iw->walls[wall].x - iw->walls[wall].next->x);
	iw->walls[wall].l.c = (float)(iw->walls[wall].next->x * iw->walls[wall].y -
		iw->walls[wall].x * iw->walls[wall].next->y);
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
	while (++i <= WINDOW_W)
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

//void	recalculate_angle(t_sdl *iw)
//{
//	t_point2d	lv;
//	t_point2d	rv;
//
//	lv.x = iw->d.left_point.x - (float)iw->p.x;
//	lv.y = iw->d.left_point.y - (float)iw->p.y;
//	rv.x = iw->d.right_point.x - (float)iw->p.x;
//	rv.y = iw->d.right_point.y - (float)iw->p.y;
//	iw->v.angle = acosf((lv.x * rv.x + lv.y * rv.y) / (sqrtf(lv.x * lv.x + lv.y * lv.y) * sqrtf(rv.x * rv.x + rv.y * rv.y))) / 2.0f;
//}

void	get_left_right_lines_points(t_sdl *iw)
{
	float	na;
	float	nk;

	na = iw->p.rot - iw->v.angle;
	if (na < 0.0f)
		na += G360;
	nk = get_k_angle(na);
	iw->d.left_line.a = tanf(nk);
	iw->d.left_line.b = -1.0f;
	iw->d.left_line.c = (float)iw->p.y - iw->d.left_line.a * (float)iw->p.x;
	if (na < 180.0f * G1)
		iw->d.left_point.y = (float)iw->p.y - 1.0f;
	else
		iw->d.left_point.y = (float)iw->p.y + 1.0f;
	iw->d.left_point.x = (iw->d.left_line.b * iw->d.left_point.y + iw->d.left_line.c) / (-iw->d.left_line.a);

	na = iw->p.rot + iw->v.angle;
	if (na > G360)
		na -= G360;
	nk = get_k_angle(na);
	iw->d.right_line.a = tanf(nk);
	iw->d.right_line.b = -1.0f;
	iw->d.right_line.c = (float)iw->p.y - iw->d.right_line.a * (float)iw->p.x;
	if (na < 180.0f * G1)
		iw->d.right_point.y = (float)iw->p.y - 1.0f;
	else
		iw->d.right_point.y = (float)iw->p.y + 1.0f;
	iw->d.right_point.x = (iw->d.right_line.b * iw->d.right_point.y + iw->d.right_line.c) / (-iw->d.right_line.a);
}

void	get_screen_line(t_sdl *iw)
{
	float	na;

	na = iw->p.rot + G90;
	if (na > G360)
		na -= G360;
	na = get_k_angle(na);
	iw->d.screen.a = tanf(na);
	iw->d.screen.b = -1.0f;
	iw->d.screen.c = (float)iw->p.y - iw->d.screen.a * (float)iw->p.x;
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

void	free_walls(t_sdl *iw)
{
	t_save_wall	*tmp;
	t_save_wall	*tmp2;

	tmp = iw->d.vw;
	while (tmp != 0)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	iw->d.vw = 0;
}

void	free_pairs(t_sdl *iw)
{
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;

	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	iw->d.vwp = 0;
}

void	add_wall(t_sdl *iw, t_save_wall *tmp)
{
	t_save_wall	*tmp2;

	if (iw->d.vw == 0 || /*iw->d.vw->len > tmp->len*/iw->d.vw->x > tmp->x)
	{
		tmp->next = iw->d.vw;
		iw->d.vw = tmp;
	}
	else
	{
		tmp2 = iw->d.vw;
		while (tmp2->next != 0 && /*tmp2->next->len <= tmp->len*/tmp2->next->x < tmp->x)
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

float	get_vectors_angle(float x1, float y1, float x2, float y2)
{
	float	k;

	k = (x1 * x2 + y1 * y2) / (sqrtf(x1 * x1 + y1 * y1) * sqrtf(x2 * x2 + y2 * y2));
	if (k > 1.0f)
		return (acos(1.0f));
	else if (k < -1.0f)
		return (acos(-1.0f));
	return (acosf(k));
}

void	get_visible_walls2(t_sdl *iw, int wall, float lang)
{
	t_save_wall	*w;
	
	w = (t_save_wall *)malloc(sizeof(t_save_wall));
	w->x = (int)(lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	w->len = sqrtf(powf((float)(iw->p.x - iw->walls[wall].x), 2.0f) +  powf((float)(iw->p.y - iw->walls[wall].y), 2.0f));
	w->plen = fabsf(iw->d.screen.a * (float)iw->walls[wall].x + iw->d.screen.b * (float)iw->walls[wall].y + iw->d.screen.c) /
				sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	w->olen = 0.0f;
	w->p.x = iw->walls[wall].x;
	w->p.y = iw->walls[wall].y;
	w->wall = &iw->walls[wall];
	w->zu = get_ceil_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->zd = get_floor_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->next = 0;
	add_wall(iw, w);
}

void	get_visible_walls(t_sdl *iw)
{
	int		wall;
	float	lang;
	float	rang;
	t_point2d	p;

	wall = iw->sectors[iw->d.cs].sw;
	while (wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
			(float)(iw->walls[wall].x - iw->p.x), (float)(iw->walls[wall].y - iw->p.y));
		rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
			(float)(iw->walls[wall].x - iw->p.x), (float)(iw->walls[wall].y - iw->p.y));
		if ((lang <= 2 * iw->v.angle && rang <= 2 * iw->v.angle) || lang == 0.0f || rang == 0.0f)
			get_visible_walls2(iw, wall, lang);
		wall++;
	}
}

int		cross_two_lines(t_line2d *l1, t_line2d *l2, t_intpoint2d *p)
{
	t_line2d	*tmp;
	float		py;

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
	py = (l2->a * l1->c - l1->a * l2->c) / (l1->a * l2->b - l2->a * l1->b);
	p->y = (int)roundf(py);
	if (l2->b == 0 && l2->a != 0)
		p->x = (int)roundf((l2->b * py + l2->c) / (-l2->a));
	else
		p->x = (int)roundf((l1->b * py + l1->c) / (-l1->a));
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
	if (side * iw->d.view_dir.y < 0)
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

int		if_not_in_vw(t_sdl *iw, t_wall *wall)
{
	t_save_wall	*tmp;

	tmp = iw->d.vw;
	while (tmp != 0)
	{
		if (tmp->wall == wall)
			return (0);
		tmp = tmp->next;
	}
	return (1);
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
	tmp->olen = sqrtf(powf(p->x - wall->x, 2.0f) + powf(p->y - wall->y, 2.0f));
	tmp->p = *p;
	tmp->zd = get_floor_z(iw, p->x, p->y);
	tmp->zu = get_ceil_z(iw, p->x, p->y);
	tmp->next = 0;
	//printf("Adding x %d px %d py %d len %f plen %f\n", tmp->x, p->x, p->y, tmp->len, tmp->plen);
	add_wall(iw, tmp);
}

void	get_all_intersection_line(t_sdl *iw, t_line2d *nl, int right)
{
	int		wall;
	t_intpoint2d	p;

	wall = iw->sectors[iw->d.cs].sw - 1;
	while (++wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
		if (if_not_in_vw(iw, ((right == 0) ? &iw->walls[wall] : iw->walls[wall].next))
			&& visible_wall(iw, wall) && cross_two_lines(nl, &iw->walls[wall].l, &p)
			&& point_in_front_and_on_wall(iw, &p, wall))
			add_lr_wall(iw, &p, ((right == 0) ? &iw->walls[wall] : iw->walls[wall].next), right * WINDOW_W);
}

void	get_left_right_visible_walls(t_sdl *iw)
{
	get_all_intersection_line(iw, &iw->d.left_line, 0);
	get_all_intersection_line(iw, &iw->d.right_line, 1);
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
			put_wall_pixel(b, b->x, b->y);
		else
			put_wall_pixel(b, b->y, b->x);
		b->x += b->sx;
	}
}

void	brez_line(int *wall_y, t_draw_line line)
{
	t_brez	b;

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

	j = left->x - 1;
	while (++j < left->x + len)
	{
		i = iw->d.top[j] - 1;
		while (++i < iw->d.bottom[j])
			set_pixel(iw->sur, j, i, 0x00FF00);
		iw->d.top[j] = iw->d.bottom[j];
	}
}

void	draw_wall_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;

	d.left_len = 0.0f;
	d.tx = left->olen * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
	while (d.tx >= (float)iw->t[left->wall->t]->w)
		d.tx -= (float)iw->t[left->wall->t]->w;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < iw->d.top[j + left->x])
			d.ty = d.zu + (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		while (d.ty > (float)iw->t[left->wall->t]->h)
			d.ty -= (float)iw->t[left->wall->t]->h;
		i = iw->d.top[left->x + j] - 1;
		while (++i < iw->d.bottom[left->x + j])
		{
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx, (int)d.ty));
			d.ty += d.dty;
			while (d.ty >= (float)iw->t[left->wall->t]->h)
				d.ty -= (float)iw->t[left->wall->t]->h;
		}
		iw->d.top[left->x + j] = iw->d.bottom[left->x + j];
		d.ang += d.dang;
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		while (d.tx > (float)iw->t[left->wall->t]->w)
			d.tx -= (float)iw->t[left->wall->t]->w;
	}

}

void	draw_floor(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallBot[j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
			continue ;
		i = iw->d.wallBot[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0x0000FF);
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
	}
}

void	draw_ceil(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallTop[j] <= iw->d.top[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
			continue;
		i = iw->d.top[left->x + j] - 1;
		while (++i < iw->d.wallTop[j])
			set_pixel(iw->sur, left->x + j, i, 0x00FFFF);
		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
			iw->d.top[left->x + j] = iw->d.wallTop[j];
	}
}

void	draw_useless_lines(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;

	i = iw->d.wallTop[0] - 1;
	while (++i < iw->d.wallBot[0])
		set_pixel(iw->sur, left->x, i, 0xFF00FF);
}

void	draw_between_sectors_bot(t_sdl *iw, t_save_wall *left, int len, int *tmp)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.bottom[left->x + j] <= tmp[j])
			continue ;
		i = tmp[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0xFF0000);
		iw->d.bottom[left->x + j] = tmp[j];
	}
}

void	draw_between_sectors_top(t_sdl *iw, t_save_wall *left, int len, int *tmp)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= tmp[j])
			continue;
		i = iw->d.top[left->x + j] - 1;
		while (++i < tmp[j])
			set_pixel(iw->sur, left->x + j, i, 0xFF0000);
		iw->d.top[left->x + j] = tmp[j];
	}
}

void	draw_between_sectors_walls(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_line		l;
	int				lz;
	int				rz;
	int				*tmp;

	tmp = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	l.x0 = left->x;
	l.x1 = right->x;
	/*lz = get_floor_z(iw, left->wall->x, left->wall->y);
	rz = get_floor_z(iw, right->wall->x, right->wall->y);*/
	lz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	rz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen;
	brez_line(tmp, l);
	draw_between_sectors_bot(iw, left, right->x - left->x + 1, tmp);

	/*lz = get_ceil_z(iw, left->wall->x, left->wall->y);
	rz = get_ceil_z(iw, right->wall->x, right->wall->y);*/
	lz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	rz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen;
	brez_line(tmp, l);
	draw_between_sectors_top(iw, left, right->x - left->x + 1, tmp);
	free(tmp);
}

void	fill_portal(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	int		j;

	j = left->x - 1;
	while (++j < right->x)
		iw->d.top[j] = iw->d.bottom[j];
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

void	draw_start(t_sdl *iw);
void	draw_next_sector(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_sdl	iw2;

	iw2 = *iw;
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	draw_between_sectors_walls(&iw2, left, right, len);
	get_direction(&iw2);
	get_screen_line(&iw2);
	get_left_right_lines_points(&iw2);
	//recalculate_angle(&iw2); 
	iw2.d.vw = 0;
	iw2.d.vwp = 0;
	iw2.d.screen_left = left->x;
	iw2.d.screen_right = right->x;
	fill_tb_by_slsr(&iw2);
	get_visible_walls(&iw2);
	get_left_right_visible_walls(&iw2);
	iw2.d.prev_sector = iw->d.cs;
	draw_start(&iw2);
	fill_portal(iw, left, right);
	/*iw->d.top = iw2.d.top;
	iw->d.bottom = iw2.d.bottom;*/
}

void	draw_all(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	draw_floor(iw, left, len);
	draw_ceil(iw, left, len);
	if (left->wall->nextsector == -1)
	{
		draw_wall_tex(iw, left, right, len);
		//draw_useless_lines(iw, left, len);
	}
	else if (left->wall->nextsector != iw->d.prev_sector)
		draw_next_sector(iw, left, right, len);
}

void	draw_left_right(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_draw_line		l;

	if (left->x >= right->x)
		return;
	//printf("malloc\n");
	iw->d.wallTop = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	iw->d.wallBot = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	//printf("malloced\n");
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
	draw_all(iw, left, right, right->x - left->x + 1);
	//printf("draw lpx %d lpy %d rpx %d rpy %d lplen %f lx %d rx %d\n", left->wall->x, left->wall->y, right->wall->x, right->wall->y, left->plen, left->x, right->x);
	/*SDL_UpdateWindowSurface(iw->win);
	system("PAUSE");*/
	free(iw->d.wallBot);
	free(iw->d.wallTop);
}

void	add_pair(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;

	tmp = (t_save_wall_pairs *)malloc(sizeof(t_save_wall_pairs));
	tmp->left = left;
	tmp->right = right;
	tmp->next = 0;
	if (iw->d.vwp == 0)
		iw->d.vwp = tmp;
	else
	{
		tmp2 = iw->d.vwp;
		while (tmp2->next != 0)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
}

void	sort_pairs(t_sdl *iw)
{
	t_save_wall_pairs	start;
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;
	int					f;

	if (iw->d.vwp == 0)
		return ;
	start.next = iw->d.vwp;
	f = 1;
	while (f)
	{
		tmp = &start;
		f = 0;
		while (tmp->next->next != 0)
		{
			if (tmp->next->right->x >= tmp->next->next->left->x && tmp->next->right->len > tmp->next->next->left->len
				&& tmp->next->left->x <= tmp->next->next->left->x)
			{
				tmp2 = tmp->next;
				tmp->next = tmp->next->next;
				tmp2->next = tmp->next->next;
				tmp->next->next = tmp2;
				f = 1;
			}
			tmp = tmp->next;
		}
	}
	iw->d.vwp = start.next;
}

void	draw_start(t_sdl *iw)
{
	t_save_wall *left;
	t_save_wall	*right;
	t_save_wall_pairs	*tmp;

	if (iw->d.vw == 0)
		return;
	left = iw->d.vw;
	while (left != 0)
	{
		if ((right = find_next_vis_wall(iw, left)) != 0 && left->x < right->x)
			if (!(right->x < iw->d.screen_left || left->x > iw->d.screen_right))
				add_pair(iw, left, right);
				//draw_left_right(iw, left, right);
		left = left->next;
	}

	//////
	t_save_wall_pairs *ttt;
	ttt = iw->d.vwp;
	while (ttt != 0)
	{
		printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
		ttt = ttt->next;
	}
	printf("\n\n");
	/////

	sort_pairs(iw);

	//////
	printf("sorted:\n");
	ttt = iw->d.vwp;
	while (ttt != 0)
	{
		printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
		ttt = ttt->next;
	}
	printf("\n\n");
	/////

	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		draw_left_right(iw, tmp->left, tmp->right);
		tmp = tmp->next;
	}
	free_walls(iw);
	free_pairs(iw);
}

void	draw(t_sdl *iw)
{
	set_top_bottom(iw);
	if ((iw->d.cs = get_sector(iw)) == -1)
		return ;
	get_direction(iw);
	get_screen_line(iw);
	get_left_right_lines_points(iw);
	//recalculate_angle(iw);
	iw->d.vw = 0;
	iw->d.vwp = 0;
	iw->d.screen_left = 0;
	iw->d.screen_right = WINDOW_W - 1;
	get_visible_walls(iw);
	get_left_right_visible_walls(iw);

	//////////
	t_save_wall *tmp;
	tmp = iw->d.vw;
	while (tmp != 0)
	{
		if (tmp->wall == &iw->walls[0] || tmp->wall == &iw->walls[1] || tmp->wall == &iw->walls[7] || tmp->wall == &iw->walls[8])
		printf("x %d len %f\n", tmp->x, tmp->len, tmp->wall->x, tmp->wall->y, tmp->zu,
			tmp->zd, tmp->plen);
		tmp = tmp->next;
	}
	printf("\n\n");

	////////////
	iw->d.prev_sector = -1;
	draw_start(iw);
}

void	read_textures(t_sdl *iw)
{
	iw->t[0] = SDL_LoadBMP("textures/1.bmp");
	iw->tsz[0] = 1.0f;
	iw->t[1] = SDL_LoadBMP("textures/2.bmp");
	iw->tsz[1] = 1.0f;
	iw->t[2] = SDL_LoadBMP("textures/3.bmp");
	iw->tsz[2] = 1.0f;
	iw->t[3] = SDL_LoadBMP("textures/4.bmp");
	iw->tsz[3] = 1.0f;
	//Uint8 *target_pixel = (Uint8 *)(iw->t)[0]->pixels;
	//set_pixel((iw->t)[0], 0, 0, 0xFF0000);
	//printf("%d\n", read_pixel((iw->t)[0], 2, 0));
}

void	get_def(t_sdl *iw)
{
	iw->p.x = 220;
	iw->p.y = 700;
	iw->p.z = 240;
	iw->p.introt = 345;
	iw->p.rot = (float)iw->p.introt * G1;
	iw->p.rotup = 0.0f;
	iw->v.ls = 0;
	iw->v.angle = 0.698132f;
}

int		main(void)
{
	t_sdl	iw;

	get_def(&iw);
	read_textures(&iw);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetRelativeMouseMode(0);
	iw.win = SDL_CreateWindow("SDL", 10/* SDL_WINDOWPOS_CENTERED*/, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	iw.sur = SDL_GetWindowSurface(iw.win);
	// draw
	get_map(&iw);
	create_map(&iw);
	draw(&iw);
	SDL_UpdateWindowSurface(iw.win);
	main_loop(&iw);
	SDL_FreeSurface(iw.sur);
	SDL_DestroyWindow(iw.win);
	SDL_Quit();
	// system("PAUSE");
	return (0);
}