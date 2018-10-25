#include "guardians.h"
#include <stdio.h>

void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	if (x >= 0 && x < WINDOW_W)
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
		printf("1\n");
		iw->d.screen_point.y = (float)iw->p.y;
		iw->d.screen_point.x = (float)iw->p.x + ((iw->d.view_dir.x > 0) ? len : -len);
		iw->d.screen.b = 0.0f;
		iw->d.view.a = 1.0f;
		iw->d.view.c = -1.0f * iw->d.screen_point.x;
	}
	else if (iw->d.view.b == 0.0f)
	{
		printf("2\n");
		iw->d.screen_point.x = (float)iw->p.x;
		iw->d.screen_point.y = (float)iw->p.y + ((iw->d.view_dir.y > 0) ? len : -len);
		iw->d.screen.a = 0.0f;
		iw->d.screen.b = 1.0f;
		iw->d.screen.c = -1.0f * iw->d.screen_point.y;
	}
	else
	{
		if (iw->d.view_dir.y < 0)
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
		if (iw->d.view_dir.x > 0)
			iw->d.screen_point.x = (float)iw->p.x + sqrtf(len * len - powf(fabsf((float)iw->p.y - iw->d.screen_point.y), 2.0f));
		else
			iw->d.screen_point.x = (float)iw->p.x - sqrtf(len * len - powf(fabsf((float)iw->p.y - iw->d.screen_point.y), 2.0f));
		//iw->d.screen_point.x = (-iw->d.view.c - iw->d.view.b * iw->d.screen_point.y) / iw->d.view.a;
		// angle of screen line
		diskr = iw->p.rot + G90;
		if (diskr > G360)
			diskr -= G360;
		diskr = get_k_angle(diskr);
		iw->d.screen.a = tanf(diskr);
		iw->d.screen.b = -1.0f;
		iw->d.screen.c = iw->d.screen_point.y - iw->d.screen.a * iw->d.screen_point.x;
		iw->d.screen_length = (float)WINDOW_W / (float)WINDOW_H * len / 2.0f;

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

void	add_wall1(t_sdl *iw, int x, float len, int wall)
{
	t_save_wall	*tmp;

	tmp = (t_save_wall *)malloc(sizeof(t_save_wall));
	tmp->x = x;
	tmp->len = len;
	tmp->wall = wall;
	tmp->next = 0;
	if (iw->d.vw == 0)
		iw->d.vw = tmp;
	else
		free(tmp); ////////////////
}

void	get_visible_walls2(t_sdl *iw, float clen, int wall)
{
	float	side;
	int		x;

	side = iw->d.view.a * (float)iw->walls[wall].x + iw->d.view.b * (float)iw->walls[wall].y + iw->d.view.c;
	if ((iw->d.view_dir.x < 0 && side < 0) || (iw->d.view_dir.x > 0 && side > 0))
		x = WINDOW_W / 2 - (int)((float)WINDOW_W * clen / iw->d.screen_length / 2.0f);
	else
		x = WINDOW_W / 2 + (int)((float)WINDOW_W * clen / iw->d.screen_length / 2.0f);
}

void	get_visible_walls(t_sdl *iw)
{
	int		wall;
	float	side;
	t_point2d	p;

	wall = iw->sectors[iw->d.cs].sw;
	printf("xdir %d\n", iw->d.view_dir.x);
	while (wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		side = iw->d.screen.a * (float)iw->walls[wall].x + iw->d.screen.b * (float)iw->walls[wall].y + iw->d.screen.c;
		if ((iw->d.view_dir.x < 0 && side < 0) || (iw->d.view_dir.x > 0 && side > 0))
		{
			p.x = (iw->d.screen.c / iw->d.screen.b * (float)(iw->walls[wall].x - iw->p.x) + (float)(iw->walls[wall].x * iw->p.y - iw->walls[wall].y * iw->p.x)) /
				((float)(iw->p.y - iw->walls[wall].y) - iw->d.screen.a / iw->d.screen.b * (float)(iw->walls[wall].x - iw->p.x));
			p.y = -(iw->d.screen.a * p.x + iw->d.screen.c) / iw->d.screen.b;
			side = sqrtf(powf(p.x - iw->d.screen_point.x, 2.0f) + powf(p.y - iw->d.screen_point.y, 2.0f));
			if (side <= iw->d.screen_length)
				get_visible_walls2(iw, side, wall);
				/*printf("x %d y %d len %f\n", iw->walls[wall].x, iw->walls[wall].y, side);*/
		}
		wall++;

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

}

void	get_def(t_sdl *iw)
{
	iw->p.x = 500;
	iw->p.y = 500;
	iw->p.z = 200;
	iw->p.rot = G90 * 30 / 90;
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