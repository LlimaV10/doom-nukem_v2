#include "guardians.h"
#include <stdio.h>

void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	if (x >= 0 && x < WINDOW_W && y >= 0 && y < WINDOW_H + 200)
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
		return (0);
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

int inside_sector_xy(t_sdl *iw, int sector, int x, int y)
{
	long int i, x1, y1, x2, y2;
	unsigned long int wallCrossed;

	//Quick check if the sector ID is valid.

	wallCrossed = 0;
	i = iw->sectors[sector].sw - 1;
	while (++i < iw->sectors[sector].sw + iw->sectors[sector].nw)
	{
		y1 = iw->walls[i].y - y;
		y2 = iw->walls[i].next->y - y;
		// y2 = iw->walls[iw->walls[i].nextwall].y - iw->p.y;
		if ((y1 ^ y2) < 0)
		{
			x1 = iw->walls[i].x - x;
			x2 = iw->walls[i].next->x - x;
			//x2 = iw->walls[iw->walls[i].nextwall].x - iw->p.x;
			if ((x1 ^ x2) >= 0)
				wallCrossed ^= x1;
			else
				wallCrossed ^= (x1 * y2 - x2 * y1) ^ y2;
		}
	}
	return (wallCrossed >> 31);
}

void	draw(t_sdl *iw);

void	draw_crosshair(t_sdl *iw)
{
	int		i;
	int		j;
	int		to;

	j = WINDOW_W / 2;
	i = WINDOW_H / 2 - 4;
	to = WINDOW_H / 2 - 15;
	while (--i > to)
		set_pixel(iw->sur, j, i, 0x00FF00);
	i = WINDOW_H / 2 + 4;
	to = WINDOW_H / 2 + 15;
	while (++i < to)
		set_pixel(iw->sur, j, i, 0x00FF00);
	i = WINDOW_H / 2;
	j = WINDOW_W / 2 + 4;
	to = WINDOW_W / 2 + 15;
	while (++j < to)
		set_pixel(iw->sur, j, i, 0x00FF00);
	j = WINDOW_W / 2 - 4;
	to = WINDOW_W / 2 - 15;
	while (--j > to)
		set_pixel(iw->sur, j, i, 0x00FF00);
}

//void	draw_text_to_window_surface(t_sdl *iw, int x, int y, SDL_Surface *t)
//{
//	int		i;
//	int		j;
//	char	*pixels;
//	int		color;
//
//	color = ((255 - (int)t->format->palette->colors->r) << 16) +
//		((255 - (int)t->format->palette->colors->g) << 8) +
//		((255 - (int)t->format->palette->colors->b));
//	pixels = (char *)t->pixels;
//	i = -1;
//	while (++i < t->h)
//	{
//		j = -1;
//		while (++j < t->w)
//		{
//			if ((int)(*pixels) == 1)
//				set_pixel(iw->sur, x + j, y + i, color);
//			pixels += 1;
//		}
//		/*pixels += 1;*/
//	}
//}

void	draw_text_number(t_sdl *iw, t_draw_info *d, const char *s, int numb)
{
	d->s2 = ft_itoa(numb);
	d->s3 = ft_strjoin(s, d->s2);
	free(d->s2);
	d->stext = TTF_RenderText_Solid(iw->arial_font, d->s3, d->col);
	free(d->s3);
	SDL_BlitSurface(d->stext, NULL, iw->sur, &d->rect);
	SDL_FreeSurface(d->stext);
}

void	draw_text(t_sdl *iw, const char *s, int x, int y)
{
	SDL_Color	col;
	SDL_Rect	rect;
	SDL_Surface	*stext;

	col.a = 0;
	col.r = 0;
	col.g = 255;
	col.b = 0;
	rect.x = x;
	rect.y = y;
	rect.h = 100;
	rect.w = 100;
	stext = TTF_RenderText_Solid(iw->arial_font, s, col);
	SDL_BlitSurface(stext, NULL, iw->sur, &rect);
	SDL_FreeSurface(stext);
}

void	draw_some_info(t_sdl *iw)
{
	t_draw_info	d;

	d.col.r = 0;
	d.col.g = 255;
	d.col.b = 0;
	d.col.a = 0;
	d.rect.x = 0;
	d.rect.y = 0;
	d.rect.w = 200;
	d.rect.h = 100;
	draw_text_number(iw, &d, "FPS: ", iw->v.fps);
	d.rect.y = 25;
	draw_text_number(iw, &d, "Sector: ", iw->d.cs);
	if (iw->v.edit_mode == 0)
		draw_text(iw, "Wall texture editing mode", 0, 50);
	else if (iw->v.edit_mode == 1)
		draw_text(iw, "Pictures editing mode", 0, 50);

	/*if (*(iw->v.look_picture) == 0)
		draw_text(iw, "Look picture is NULL", 0, 75);
	else
		draw_text(iw, "Look picture not NULL", 0, 75);*/
}

void	draw_menu_sphere(t_sdl *iw, int a, const char *t)
{
	float	sq;
	int		i;
	int		b;
	int		r;

	b = WINDOW_H + 150;
	r = 25;
	i = a - r;
	while (++i < a + r)
	{
		sq = sqrtf(-powf(a, 2.0f) + 2.0f * (float)(a * i) +
			powf(r, 2.0f) - powf(i, 2.0f));
		set_pixel(iw->sur, i, (int)((float)b - sq) - 20, MENU_COLOR);
		set_pixel(iw->sur, i, (int)(sq + (float)b) + 20, MENU_COLOR);
	}
	i = -1;
	while (++i < r / 3)
	{
		set_pixel(iw->sur, a + r + i - 2, b - 20 - i, MENU_COLOR);
		set_pixel(iw->sur, a + r - i - 2, b - 20 - i, MENU_COLOR);
		set_pixel(iw->sur, a - r + i + 2, b + 20 + i, MENU_COLOR);
		set_pixel(iw->sur, a - r - i + 2, b + 20 + i, MENU_COLOR);
	}
	draw_text(iw, t, a - 6, WINDOW_H + 135);
}

void	draw_up_down_menu_arrows(t_sdl *iw, int x)
{
	int		i;

	i = WINDOW_H + 140;
	while (--i > WINDOW_H + 110)
		set_pixel(iw->sur, x, i, MENU_COLOR);
	i = WINDOW_H + 160;
	while (++i < WINDOW_H + 190)
		set_pixel(iw->sur, x, i, MENU_COLOR);
	i = -1;
	while (++i < 10)
	{
		set_pixel(iw->sur, x + i, WINDOW_H + 110 + i, MENU_COLOR);
		set_pixel(iw->sur, x - i, WINDOW_H + 110 + i, MENU_COLOR);
		set_pixel(iw->sur, x + i, WINDOW_H + 190 - i, MENU_COLOR);
		set_pixel(iw->sur, x - i, WINDOW_H + 190 - i, MENU_COLOR);
	}
}

void	draw_menu(t_sdl *iw)
{
	draw_menu_sphere(iw, 100, "X");
	draw_menu_sphere(iw, 170, "Y");
	draw_up_down_menu_arrows(iw, 50);
	if (iw->v.changing_fc)
		draw_text(iw, "C", 15, WINDOW_H + 135);
	else
		draw_text(iw, "F", 15, WINDOW_H + 135);
	draw_text(iw, "R", 210, WINDOW_H + 135);
}

void	ft_scaled_blit(SDL_Surface *tex, SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;

	i = -1;
	while (++i < rect->h)
	{
		j = -1;
		while (++j < rect->w)
			set_pixel(winsur, rect->x + j, rect->y + i, get_pixel(tex,
				tex->w * j / rect->w, tex->h * i / rect->h));
	}
}

void	draw_tex_to_select(t_sdl *iw)
{
	int		i;
	SDL_Rect	rect;

	rect.x = 0;
	rect.y = WINDOW_H;
	rect.w = 100;
	rect.h = 100;
	i = iw->v.scroll_first_tex - 1;
	while (++i < TEXTURES_COUNT && rect.x < WINDOW_W)
	{
		ft_scaled_blit(iw->t[i], iw->sur, &rect);
		rect.x += 100;
	}
}

void	draw_selected_tex(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.x = WINDOW_W - 110;
	rect.y = 10;
	rect.w = 100;
	rect.h = 100;
	ft_scaled_blit(iw->t[iw->v.tex_to_fill], iw->sur, &rect);
}

void	update(t_sdl *iw)
{
	SDL_FillRect(iw->sur, &iw->winrect, 0x000000);
	draw(iw);
	draw_crosshair(iw);
	draw_some_info(iw);
	draw_selected_tex(iw);
	SDL_UpdateWindowSurface(iw->win);
	//printf("Update\n");
	//printf("update ret %d\n", ret);
}

//void	check_animations(t_sdl *iw)
//{
//	//(*(iw->v.look_wall))
//	int		i;
//	t_wall	*w;
//	t_wall	*sw;
//
//	i = -1;
//	while (++i < iw->v.count_portal_rot_anim)
//	{
//		sw = &iw->walls[iw->anim[i].start_wall];
//		if (sw == *(iw->v.look_wall))
//		{
//			iw->anim[0].time = clock();
//			return;
//		}
//		w = iw->walls[iw->anim[i].start_wall].next;
//		while (w != sw)
//		{
//			if (w == *(iw->v.look_wall))
//			{
//				iw->anim[0].time = clock();
//				return;
//			}
//			w = w->next;
//		}
//	}
//}

void	key_up(int code, t_sdl *iw)
{
	printf("keyup = %d\n", code);
	if (code == 41)
		exit_x(iw);
	else if (code == 79)
		iw->v.rot_right = -1;
	else if (code == 80)
		iw->v.rot_left = -1;
	else if (code == 26)
		iw->v.front = -1;
	else if (code == 22)
		iw->v.back = -1;
	else if (code == 4)
		iw->v.left = -1;
	else if (code == 7)
		iw->v.right = -1;
	else if (code == 82)
		iw->v.rot_up = -1;
	else if (code == 81)
		iw->v.rot_down = -1;
	else if (code == 19)
		iw->v.edit_mode = (iw->v.edit_mode == 0) ? 1 : 0;
	printf("rot = %d px %d py %d pz %d rotup %d\n", iw->p.introt, iw->p.x, iw->p.y, iw->p.z, iw->p.rotup);
}

void	key_down(int code, t_sdl *iw)
{
	printf("keydown = %d\n", code);
	if (code == 41)
		exit_x(iw);
	else if (code == 79)
		iw->v.rot_right = clock();
	else if (code == 80)
		iw->v.rot_left = clock();
	else if (code == 26)
		iw->v.front = clock();
	else if (code == 22)
		iw->v.back = clock();
	else if (code == 4)
		iw->v.left = clock();
	else if (code == 7)
		iw->v.right = clock();
	else if (code == 82)
		iw->v.rot_up = clock();
	else if (code == 81)
		iw->v.rot_down = clock();
	else if (code == 18)
	{
		iw->v.kernel = ((iw->v.kernel == 1) ? 0 : 1);
		if (iw->v.kernel)
			printf("OpenCL ON\n");
		else
			printf("OpenCL OFF\n");
	}
	else if (code == 43)
	{
		iw->v.mouse_mode = ((iw->v.mouse_mode == 1) ? 0 : 1);
		SDL_SetRelativeMouseMode(iw->v.mouse_mode);
	}
	else if (code == 55 && *(iw->v.look_sector) != 0 && iw->v.mouse_mode == 1)
		(*(iw->v.look_sector))->cl.t = iw->v.tex_to_fill;
	else if (code == 56 && *(iw->v.look_sector) != 0 && iw->v.mouse_mode == 1)
		(*(iw->v.look_sector))->fr.t = iw->v.tex_to_fill;
	else if (code == 54 && *(iw->v.look_sector) != 0 && iw->v.mouse_mode == 1)
		(*(iw->v.look_sector))->cl.t = -1;
	else if (code == 16 && iw->v.mouse_mode == 1 && *(iw->v.look_wall) != 0 && (*iw->v.look_wall)->nextsector == -1)
		(*(iw->v.look_wall))->t = -1;
	/*else if (code == 9)
		check_animations(iw);*/
	printf("rot = %d px %d py %d pz %d rotup %d\n", iw->p.introt, iw->p.x, iw->p.y, iw->p.z, iw->p.rotup);
}

void	key_down_repeat(int code, t_sdl *iw)
{
	if (code == 44 && iw->v.jump_time == -1 && iw->v.fall == -1)
	{
		iw->v.jump_time = clock();
		iw->v.jump = JUMP_HEIGHT;
	}
}

void	mouse_move(int xrel, int yrel, t_sdl *iw)
{
	if (iw->v.mouse_mode == 0)
		return;
	//printf("xrel %d yrel %d\n", xrel, yrel);
	iw->p.rot += MOUSE_SENSIVITY * (float)xrel;
	if (iw->p.rot < 0.0f)
		iw->p.rot += G360;
	else if (iw->p.rot >= G360)
		iw->p.rot -= G360;
	iw->p.introt = (int)(iw->p.rot / G1);
	iw->p.rotup -= MOUSE_UP_DOWN_SENSIVITY * yrel;
	if (iw->p.rotup > 2 * WINDOW_H)
		iw->p.rotup = 2 * WINDOW_H;
	else if (iw->p.rotup < -2 * WINDOW_H)
		iw->p.rotup = -2 * WINDOW_H;
}

void	rotate_fc(t_sector_fc *fc, int xy, int pl)
{
	if (fc->n == 0)
	{
		fc->n = (t_vector *)malloc(sizeof(t_vector));
		fc->n->c = INCLINED_FC_Z;
		fc->n->a = 0;
		fc->n->b = 0;
	}
	if (xy && ((pl > 0 && fc->n->b < MAX_INCLINED_FC_XY) || (pl < 0 && fc->n->b > -MAX_INCLINED_FC_XY)))
		fc->n->b += pl;
	else if (!xy && ((pl > 0 && fc->n->a < MAX_INCLINED_FC_XY) || (pl < 0 && fc->n->a > -MAX_INCLINED_FC_XY)))
		fc->n->a += pl;
	if (fc->n->a == 0 && fc->n->b == 0)
	{
		free(fc->n);
		fc->n = 0;
	}
	else
		fc->n->d = -fc->n->a * fc->x - fc->n->b * fc->y - fc->n->c * fc->z;
}

void	mouse_buttonleft_up(int x, int y, t_sdl *iw)
{
	int		i;

	if (y > WINDOW_H && y < WINDOW_H + 100 && iw->v.mouse_mode == 0)
	{
		i = x / 100 + iw->v.scroll_first_tex;
		if (i < TEXTURES_COUNT)
			iw->v.tex_to_fill = i;
	}
	else if (y > WINDOW_H + 100 && y < WINDOW_H + 200
		&& iw->v.mouse_mode == 0 && *(iw->v.look_wall) != 0)
	{
		if (x < 40)
		{
			iw->v.changing_fc = ((iw->v.changing_fc) ? 0 : 1);
			SDL_FillRect(iw->sur, &iw->v.chang_fc_rect, 0x000000);
			if (iw->v.changing_fc)
				draw_text(iw, "C", 15, WINDOW_H + 135);
			else
				draw_text(iw, "F", 15, WINDOW_H + 135);
		}
		else if (x < 70)
		{
			if (y < WINDOW_H + 150)
			{
				if (iw->v.changing_fc)
					(*(iw->v.look_sector))->cl.z += 100;
				else if ((*(iw->v.look_sector))->cl.z - (*(iw->v.look_sector))->fr.z > 200)
					(*(iw->v.look_sector))->fr.z += 100;
			}
			else
			{
				if (!iw->v.changing_fc)
					(*(iw->v.look_sector))->fr.z -= 100;
				else if ((*(iw->v.look_sector))->cl.z - (*(iw->v.look_sector))->fr.z > 200)
					(*(iw->v.look_sector))->cl.z -= 100;
			}
			if (iw->v.changing_fc && (*(iw->v.look_sector))->cl.n != 0)
				(*(iw->v.look_sector))->cl.n->d = -(*(iw->v.look_sector))->cl.n->a * (*(iw->v.look_sector))->cl.x -
				(*(iw->v.look_sector))->cl.n->b * (*(iw->v.look_sector))->cl.y -
				(*(iw->v.look_sector))->cl.n->c * (*(iw->v.look_sector))->cl.z;
			else if (!iw->v.changing_fc && (*(iw->v.look_sector))->fr.n != 0)
				(*(iw->v.look_sector))->fr.n->d = -(*(iw->v.look_sector))->fr.n->a * (*(iw->v.look_sector))->fr.x -
				(*(iw->v.look_sector))->fr.n->b * (*(iw->v.look_sector))->fr.y -
				(*(iw->v.look_sector))->fr.n->c * (*(iw->v.look_sector))->fr.z;
		}
		else if (x < 140)
		{
			if (y < WINDOW_H + 150)
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 0, 1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 0, 1);
			}
			else
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 0, -1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 0, -1);
			}
		}
		else if (x < 210)
		{
			if (y < WINDOW_H + 150)
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 1, 1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 1, 1);
			}
			else
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 1, -1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 1, -1);
			}
		}
		else if (x < 230)
		{
			if (iw->v.changing_fc)
			{
				free((*(iw->v.look_sector))->cl.n);
				(*(iw->v.look_sector))->cl.n = 0;
			}
			else
			{
				free((*(iw->v.look_sector))->fr.n);
				(*(iw->v.look_sector))->fr.n = 0;
			}
		}
	}
	else if (iw->v.mouse_mode == 1 && iw->v.edit_mode == 0
		&& *(iw->v.look_wall) != 0)
	{
		if (iw->v.look_portal == 0 || iw->v.look_portal->glass < 0)
			(*(iw->v.look_wall))->t = iw->v.tex_to_fill;
		else
			iw->v.look_portal->glass = -1;
	}
	else if (iw->v.mouse_mode == 1 && iw->v.edit_mode == 1
		&& *(iw->v.look_picture) != 0 && *(iw->v.look_wall) != 0)
	{
		(*(iw->v.look_picture))->t = iw->v.tex_to_fill;
		calculate_picture(iw, *(iw->v.look_wall), *(iw->v.look_picture));
	}
}

void	mouse_buttonright_up(int x, int y, t_sdl *iw)
{
	if (iw->v.mouse_mode == 1 && iw->v.look_portal != 0)
		iw->v.look_portal->glass = iw->v.tex_to_fill;
}

void	mouse_wheel(SDL_Event *e, t_sdl *iw)
{
	if (iw->v.mouse_y > WINDOW_H && iw->v.mouse_y < WINDOW_H + 100 && iw->v.mouse_mode == 0)
	{
		iw->v.scroll_first_tex -= e->wheel.y;
		if (iw->v.scroll_first_tex < 0)
			iw->v.scroll_first_tex = 0;
		if (iw->v.scroll_first_tex >= TEXTURES_COUNT)
			iw->v.scroll_first_tex = TEXTURES_COUNT - 1;
		SDL_FillRect(iw->sur, &iw->v.scroll_tex_rect, 0x000000);
		draw_tex_to_select(iw);
	}
}

t_wall	*is_wall_portal(t_sdl *iw, int dx, int dy)
{
	t_line2d	mv;
	int			wall;
	int			side1;
	int			side2;

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

void	move_collisions(t_sdl *iw, int dx, int dy)
{
	int		dd;
	int		i;

	if (inside_sector_xy(iw, iw->d.cs, iw->p.x + dx * PL_COL_SZ, iw->p.y) &&
		inside_sector_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y))
	{
		dd = dy / 20 * 2;
		dy = 0;
		i = -1;
		while (++i < 10 &&
			inside_sector_xy(iw, iw->d.cs, iw->p.x + dx * PL_COL_SZ, iw->p.y + dy * PL_COL_SZ)
			&& inside_sector_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y + dy))
			dy += dd;
		iw->p.x += dx;
		iw->p.y += dy - dd;
	}
	else if (inside_sector_xy(iw, iw->d.cs, iw->p.x, iw->p.y + dy * PL_COL_SZ) &&
		inside_sector_xy(iw, iw->d.cs, iw->p.x, iw->p.y + dy))
	{
		dd = dx / 20 * 2;
		dx = 0;
		i = -1;
		while (++i < 10 &&
			inside_sector_xy(iw, iw->d.cs, iw->p.x + dx * PL_COL_SZ, iw->p.y + dy * PL_COL_SZ)
			&& inside_sector_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y + dy))
			dx += dd;
		iw->p.x += dx - dd;
		iw->p.y += dy;
	}
}

void	move_in_portal(t_sdl *iw, int dx, int dy, t_wall *sw)
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
		&& nszd - iw->p.z + PLAYER_HEIGHT < MAX_CLIMB_HEIGHT
		&& inside_sector_xy(iw, sw->nextsector, nx, ny))
	{
		iw->p.x = nx;
		iw->p.y = ny;
	}
	else
		move_collisions(iw, dx, dy);
}

void	move(t_sdl *iw, int pl, int *time)
{
	int		ang;
	int		dx;
	int		dy;
	float	speed;
	t_wall	*sw;

	ang = (iw->p.introt + pl) % 360;
	speed = MOVING_SPEED_PER_HALF_SEC * (float)(clock() - *time) / (float)CLKS_P_S;
	*time = clock();
	if (ang < 90)
	{
		dx = (int)(speed * cosf((float)ang * G1)) * 2;
		dy = (int)(-speed * sinf((float)ang * G1)) * 2;
	}
	else if (ang < 180)
	{
		dx = (int)(-speed * cosf(G180 - (float)ang * G1)) * 2;
		dy = (int)(-speed * sinf(G180 - (float)ang * G1)) * 2;
	}
	else if (ang < 270)
	{
		dx = (int)(speed * cosf((float)ang * G1) - G180) * 2;
		dy = (int)(-speed * sinf((float)ang * G1) - G180) * 2;
	}
	else
	{
		dx = (int)(speed * cosf(G360 - (float)ang * G1)) * 2;
		dy = (int)(speed * sinf(G360 - (float)ang * G1)) * 2;
	}
	if (inside_sector_xy(iw, iw->d.cs, iw->p.x + dx, iw->p.y + dy))
	{
		if (inside_sector_xy(iw, iw->d.cs, iw->p.x + dx * PL_COL_SZ, iw->p.y + dy * PL_COL_SZ)
			|| ((sw = is_wall_portal(iw, dx * PL_COL_SZ, dy * PL_COL_SZ)) != 0 && sw->glass < 0) )
		{
			iw->p.x += dx;
			iw->p.y += dy;
		}
		else
			move_collisions(iw, dx, dy);
	}
	else if ((sw = is_wall_portal(iw, dx, dy)) == 0 || sw->glass >= 0)
		move_collisions(iw, dx, dy);
	else
		move_in_portal(iw, dx, dy, sw);
}

void	get_wall_line2(t_wall *wall)
{
	wall->l.a = (float)(wall->next->y - wall->y);
	wall->l.b = (float)(wall->x - wall->next->x);
	wall->l.c = (float)(wall->next->x * wall->y -
		wall->x * wall->next->y);
}

//void	do_animations(t_sdl *iw)
//{
//	int		i;
//	t_wall	*sw;
//	t_wall	*w;
//	t_wall	*nw;
//	float	ang;
//
//	i = -1;
//	while (++i < iw->v.count_portal_rot_anim)
//	{
//		if (iw->anim[i].time > 0)
//		{
//			//ang = (float)(clock() - iw->anim[i].time) / (float)CLKS_P_S
//			if (iw->anim[i].anim_status == 0)
//			{
//				ang = (float)iw->anim[i].angle * G1;
//				//iw->anim[i].anim_status = 1;
//				//iw->anim[i].time = -1;
//				sw = &iw->walls[iw->anim[i].start_wall];
//				w = sw->next;
//				while (w != sw)
//				{
//					printf("old x %d y %d\n", w->x, w->y);
//					w->x = (float)sw->x + (float)(w->x - sw->x) * cosf(ang) -
//						(float)(w->y - sw->y) * sinf(ang);
//					w->y = (float)sw->y + (float)(w->y - sw->y) * cosf(ang) +
//						(float)(w->x - sw->x) * sinf(ang);
//					printf("new x %d y %d\n", w->x, w->y);
//					nw = iw->walls[w->nextsector_wall].next;
//					nw->x = w->x;
//					nw->y = w->y;
//					w = w->next;
//				}
//				w = sw->next;
//				while (w != sw)
//				{
//					nw = iw->walls[w->nextsector_wall].next;
//					get_wall_line2(iw, w);
//					get_wall_line2(iw, nw);
//					w = w->next;
//				}
//			}
//		}
//	}
//}

void	loop(t_sdl *iw)
{
	int		t;
	float	jsz;

	if ((double)(clock() - iw->loop_update_time) < (double)CLKS_P_S / (double)MAX_FPS)
		return;
	if (iw->v.rot_right != -1)
	{
		iw->p.rot += (ROTATION_SPEED_PER_HALF_SEC * (double)(clock() - iw->v.rot_right)
			/ (double)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot >= G360)
			iw->p.rot -= G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_right = clock();
	}
	if (iw->v.rot_left != -1)
	{
		iw->p.rot -= (ROTATION_SPEED_PER_HALF_SEC * (double)(clock() - iw->v.rot_left)
			/ (double)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot < 0.0f)
			iw->p.rot += G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_left = clock();
	}
	if (iw->v.rot_up != -1 && iw->p.rotup < 2 * WINDOW_H)
	{
		iw->p.rotup += 2 * WINDOW_H * (clock() - iw->v.rot_up) / CLKS_P_S;
		iw->v.rot_up = clock();
	}
	if (iw->v.rot_down != -1 && iw->p.rotup > -2 * WINDOW_H)
	{
		iw->p.rotup -= 2 * WINDOW_H * (clock() - iw->v.rot_down) / CLKS_P_S;
		iw->v.rot_down = clock();
	}
	//do_animations(iw);
	if (iw->d.cs >= 0)
	{
		iw->v.plrzu = get_ceil_z(iw, iw->p.x, iw->p.y);
		iw->v.plrzd = get_floor_z(iw, iw->p.x, iw->p.y);
		if (iw->v.front != -1)
		{
			move(iw, 0, &iw->v.front);
			//iw->v.front = clock();
		}
		if (iw->v.back != -1)
		{
			move(iw, 180, &iw->v.back);
			//iw->v.back = clock();
		}
		if (iw->v.left != -1)
		{
			move(iw, 270, &iw->v.left);
			//iw->v.left = clock();
		}
		if (iw->v.right != -1)
		{
			move(iw, 90, &iw->v.right);
			//iw->v.right = clock();
		}
		if (iw->v.fall != -1)
		{
			t = clock();
			iw->p.z -= (int)(iw->v.accel * ((double)(t - iw->v.fall) /
				(double)CLKS_P_S) * 50.0f/* *
				((float)(t - iw->loop_update_time) / (float)CLOCKS_PER_SEC)*/);
		}
		else if (iw->v.jump_time != -1)
		{
			jsz = (double)(clock() - iw->v.jump_time) / (double)CLKS_P_S * (double)JUMP_HEIGHT *
				iw->v.accel / 10.0f;
			if ((int)jsz >= iw->v.jump)
			{
				iw->p.z += iw->v.jump;
				iw->v.jump_time = -1;
			}
			else
			{
				iw->p.z += (int)jsz;
				iw->v.jump -= (int)jsz;
			}

		}
		if (iw->v.fall == -1 && iw->v.jump_time == -1
			&& (iw->p.z - iw->v.plrzd) > PLAYER_HEIGHT)
			iw->v.fall = clock();
		if (iw->sectors[iw->d.cs].cl.t >= 0 && iw->p.z + PLAYER_HEAD_SIZE > iw->v.plrzu)
			iw->p.z = iw->v.plrzu - PLAYER_HEAD_SIZE;
		else if (iw->p.z - iw->v.plrzd < PLAYER_HEIGHT)
		{
			iw->p.z = iw->v.plrzd + PLAYER_HEIGHT;
			iw->v.fall = -1;
		}
	}
	else
		iw->v.fall = -1;
	update(iw);
	iw->v.fps = (double)CLKS_P_S / (double)(clock() - iw->loop_update_time);
	iw->loop_update_time = clock();
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
			else if (e.type == SDL_KEYDOWN)
			{
				key_down_repeat(e.key.keysym.scancode, iw);
				if (e.key.repeat == 0)
					key_down(e.key.keysym.scancode, iw);
			}
			else if (e.type == SDL_KEYUP)
				key_up(e.key.keysym.scancode, iw);
			else if (e.type == SDL_MOUSEMOTION)
			{
				mouse_move(e.motion.xrel, e.motion.yrel, iw);
				iw->v.mouse_x = e.motion.x;
				iw->v.mouse_y = e.motion.y;
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
				mouse_buttonleft_up(e.button.x, e.button.y, iw);
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
				mouse_buttonright_up(e.button.x, e.button.y, iw);
			else if (e.type == SDL_MOUSEWHEEL && e.wheel.y != 0)
				mouse_wheel(&e, iw);
		//else if (e.type == SDL_KEYUP)
		//	key_up(e.key.keysym.scancode, iw);
	/**/
	//pthread_mutex_lock(&iw->mutex);
		loop(iw);
		//pthread_mutex_unlock(&iw->mutex);
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
			x2 = iw->walls[i].next->x - iw->p.x;
			//x2 = iw->walls[iw->walls[i].nextwall].x - iw->p.x;
			if ((x1 ^ x2) >= 0)
				wallCrossed ^= x1;
			else
				wallCrossed ^= (x1 * y2 - x2 * y1) ^ y2;
		}
	}
	return (wallCrossed >> 31);
}

int		get_sector(t_sdl *iw)
{
	int sec;

	sec = iw->v.ls - 1;
	while (++sec < iw->v.sc)
		if (inside_sector(sec, iw) != 0)
			return (sec);
	sec = -1;
	while (++sec < iw->v.ls)
		if (inside_sector(sec, iw) != 0)
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
	iw->d.screen_len = sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
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
		return ((iw->sectors[iw->d.cs].fr.n->a * x + iw->sectors[iw->d.cs].fr.n->b *
			y + iw->sectors[iw->d.cs].fr.n->d) / (-iw->sectors[iw->d.cs].fr.n->c));
}

int		get_ceil_z(t_sdl *iw, int x, int y)
{
	if (iw->sectors[iw->d.cs].cl.n == 0)
		return (iw->sectors[iw->d.cs].cl.z);
	else
		return ((iw->sectors[iw->d.cs].cl.n->a * x + iw->sectors[iw->d.cs].cl.n->b *
			y + iw->sectors[iw->d.cs].cl.n->d) / (-iw->sectors[iw->d.cs].cl.n->c));
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
	w->len = sqrtf(powf((float)(iw->p.x - iw->walls[wall].x), 2.0f) + powf((float)(iw->p.y - iw->walls[wall].y), 2.0f));
	w->plen = fabsf(iw->d.screen.a * (float)iw->walls[wall].x + iw->d.screen.b * (float)iw->walls[wall].y + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	if ((int)w->plen == 0)
		w->plen = 1.0f;
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
	if ((int)tmp->plen == 0)
		tmp->plen = 1.0f;
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

t_save_wall	*find_next_vis_wall(t_sdl *iw, t_save_wall *left)
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

	/*d.left_len = 0.0f;
	d.tx = left->olen * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
	while (d.tx >= (float)iw->t[left->wall->t]->w)
		d.tx -= (float)iw->t[left->wall->t]->w;*/

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < iw->d.top[j + left->x])
			d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
		i = iw->d.top[left->x + j] - 1;
		while (++i < iw->d.bottom[left->x + j])
		{
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h));
			d.ty += d.dty;
		}
		iw->d.top[left->x + j] = iw->d.bottom[left->x + j];
		d.ang += d.dang;
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
			continue;
		if (iw->d.wallBot[j] < iw->d.top[left->x + j])
			i = iw->d.top[left->x + j] - 1;
		else
			i = iw->d.wallBot[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0x0000FF);
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
	}
}

void	draw_inclined_floor_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

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
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.pl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallBot[j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.coef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b));
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallBot[j] < iw->d.top[left->x + j])
			i = iw->d.top[left->x + j] - 1;
		else
			i = iw->d.wallBot[j] - 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(i + 1 - iw->d.wallBot[j]);
		while (++i < iw->d.bottom[left->x + j])
		{
			d.weight = d.wall_dist * d.coef / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			d.coef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
				get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.coef) / 2.0f;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		d.ang += d.dang;
	}
}

void	draw_floor_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

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
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, 0, 0);
	d.zd = get_floor_z(iw, 0, 0);
	d.coef = d.zu - d.zd;
	d.pl = d.coef / (float)(iw->p.z - d.zd);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallBot[j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b) / d.coef);
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallBot[j] < iw->d.top[left->x + j])
			i = iw->d.top[left->x + j] - 1;
		else
			i = iw->d.wallBot[j] - 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(i + 1 - iw->d.wallBot[j]);
		while (++i < iw->d.bottom[left->x + j])
		{
			d.weight = d.wall_dist / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		d.ang += d.dang;
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
		while (++i < iw->d.wallTop[j] && i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0x00FFFF);
		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
			iw->d.top[left->x + j] = iw->d.wallTop[j];
	}
}

void	draw_inclined_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

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
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.pl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallTop[j] <= iw->d.top[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.coef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b));
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
			i = iw->d.wallTop[j] + 1;
		else
			i = iw->d.bottom[left->x + j] + 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(iw->d.wallTop[j] - i + 1);
		while (--i >= iw->d.top[left->x + j])
		{
			d.weight = d.wall_dist * d.coef / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			d.coef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
				get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.coef) / 2.0f;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		d.ang += d.dang;
	}
}

void	draw_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

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
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, 0, 0);
	d.zd = get_floor_z(iw, 0, 0);
	d.coef = d.zu - d.zd;
	d.pl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallTop[j] <= iw->d.top[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b)) * d.coef;
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
			i = iw->d.wallTop[j] + 1;
		else
			i = iw->d.bottom[left->x + j] + 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(iw->d.wallTop[j] - i + 1);
		while (--i >= iw->d.top[left->x + j])
		{
			d.weight = d.wall_dist / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		d.ang += d.dang;
	}
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
			continue;
		i = tmp[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0xFF0000);
		iw->d.bottom[left->x + j] = tmp[j];
	}
}

void	draw_between_sectors_bot_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int *tmp)
{
	int		i;
	int		j;
	t_draw_wall_tex	d;

	if (left->wall->t < 0)
		return;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) /
		(sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) *
			sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)(right->x - left->x);
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) /
		(sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) *
			sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;

	d.left_len = 0.0f;
	d.tx = left->olen * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
	while (d.tx >= (float)iw->t[left->wall->t]->w)
		d.tx -= (float)iw->t[left->wall->t]->w;

	j = -1;
	while (++j < right->x - left->x)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.bottom[left->x + j] <= tmp[j])
		{
			d.ang += d.dang;
			continue;
		}
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < tmp[j])
			d.ty = d.zu + iw->tsz[left->wall->t] *
			(d.zu - d.zd) * (float)(tmp[j] - iw->d.wallTop[j]) /
			(float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) /
			(float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
		while (d.ty > (float)iw->t[left->wall->t]->h)
			d.ty -= (float)iw->t[left->wall->t]->h;
		i = tmp[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
		{
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx, (int)d.ty));
			d.ty += d.dty;
			while (d.ty >= (float)iw->t[left->wall->t]->h)
				d.ty -= (float)iw->t[left->wall->t]->h;
		}
		iw->d.bottom[left->x + j] = tmp[j];
		d.ang += d.dang;
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		while (d.tx > (float)iw->t[left->wall->t]->w)
			d.tx -= (float)iw->t[left->wall->t]->w;
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
		while (++i < tmp[j] && i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0xFF0000);
		iw->d.top[left->x + j] = tmp[j];
	}
}

void	draw_between_sectors_top_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int *tmp)
{
	int		i;
	int		j;
	t_draw_wall_tex	d;

	if (left->wall->t < 0)
		return;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)(right->x - left->x);
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
	while (++j < right->x - left->x)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= tmp[j])
		{
			d.ang += d.dang;
			continue;
		}
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < tmp[j])
			d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[left->x + j] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
		while (d.ty > (float)iw->t[left->wall->t]->h)
			d.ty -= (float)iw->t[left->wall->t]->h;
		/*i = tmp[j] - 1;*/
		i = iw->d.top[left->x + j] - 1;
		while (++i < tmp[j] && i < iw->d.bottom[left->x + j])
		{
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx, (int)d.ty));
			d.ty += d.dty;
			while (d.ty >= (float)iw->t[left->wall->t]->h)
				d.ty -= (float)iw->t[left->wall->t]->h;
		}
		iw->d.top[left->x + j] = tmp[j];
		d.ang += d.dang;
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		while (d.tx > (float)iw->t[left->wall->t]->w)
			d.tx -= (float)iw->t[left->wall->t]->w;
	}
}

void	draw_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

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
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;
	if (iw->sectors[iw->d.cs].cl.t < 0)
	{
		d.rot = (iw->p.rot - iw->v.angle) + (float)left->x /
			(float)WINDOW_W * iw->v.angle * 2.0f;
		if (d.rot < 0.0f)
			d.rot += G360;
		else if (d.rot > G360)
			d.rot -= G360;
		d.sky_x = d.rot * ((float)iw->t[iw->l.skybox]->w) / G360;
		d.dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
		d.dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	}
	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.frcoef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len) * d.frcoef;
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;


		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
		{
			if (iw->sectors[iw->d.cs].cl.t >= 0)
			{
				if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
					i = iw->d.wallTop[j] + 1;
				else
					i = iw->d.bottom[left->x + j] + 1;
				d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
				while (--i >= iw->d.top[left->x + j])
				{
					d.weight = d.wall_dist / d.k;
					d.k += d.clpl;
					d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
					d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
					set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
						((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
						((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
							((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
						((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
					));
				}
				iw->d.top[left->x + j] = iw->d.wallTop[j];
			}
			else
			{
				d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
				d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
				i = iw->d.top[left->x + j] - 1;
				d.sky_y += d.dy * iw->d.top[left->x + j];
				if (d.sky_x > iw->t[iw->l.skybox]->w)
					d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
				while (++i < iw->d.wallTop[j] && i < iw->d.bottom[left->x + j])
				{
					set_pixel(iw->sur, j + left->x, i, get_pixel(iw->t[iw->l.skybox],
						(int)d.sky_x, (int)d.sky_y));
					d.sky_y += d.dy;
				}
			}
		}
		if (iw->sectors[iw->d.cs].cl.t < 0)
			d.sky_x += d.dx;
		d.ang += d.dang;
	}
}

void	draw_inclined_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

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
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;
	if (iw->sectors[iw->d.cs].cl.t < 0)
	{
		d.rot = (iw->p.rot - iw->v.angle) + (float)left->x /
			(float)WINDOW_W * iw->v.angle * 2.0f;
		if (d.rot < 0.0f)
			d.rot += G360;
		else if (d.rot > G360)
			d.rot -= G360;
		d.sky_x = d.rot * ((float)iw->t[iw->l.skybox]->w) / G360;
		d.dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
		d.dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	}

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.frcoef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.clcoef = d.frcoef;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len);
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist * d.frcoef / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				d.frcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
					get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.frcoef) / 2.0f;
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
		{
			if (iw->sectors[iw->d.cs].cl.t >= 0)
			{
				if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
					i = iw->d.wallTop[j] + 1;
				else
					i = iw->d.bottom[left->x + j] + 1;
				d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
				while (--i >= iw->d.top[left->x + j])
				{
					d.weight = d.wall_dist * d.clcoef / d.k;
					d.k += d.clpl;
					d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
					d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
					d.clcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
						get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.clcoef) / 2.0f;
					set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
						((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
						((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
							((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
						((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
					));
				}
				iw->d.top[left->x + j] = iw->d.wallTop[j];
			}
			else
			{
				d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
				d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
				i = iw->d.top[left->x + j] - 1;
				d.sky_y += d.dy * iw->d.top[left->x + j];
				if (d.sky_x > iw->t[iw->l.skybox]->w)
					d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
				while (++i < iw->d.wallTop[j] && i < iw->d.bottom[left->x + j])
				{
					set_pixel(iw->sur, j + left->x, i, get_pixel(iw->t[iw->l.skybox],
						(int)d.sky_x, (int)d.sky_y));
					d.sky_y += d.dy;
				}
			}
		}
		if (iw->sectors[iw->d.cs].cl.t < 0)
			d.sky_x += d.dx;
		d.ang += d.dang;
	}
}

void	draw_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

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
	/*d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;*/
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, 0, 0);
	d.zd = get_floor_z(iw, 0, 0);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;
	d.frcoef = d.zu - d.zd;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			/*if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;*/
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;

		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len) * d.frcoef;
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;


		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j] && iw->sectors[iw->d.cs].cl.t >= 0)
		{
			if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				i = iw->d.wallTop[j] + 1;
			else
				i = iw->d.bottom[left->x + j] + 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
			while (--i >= iw->d.top[left->x + j])
			{
				d.weight = d.wall_dist / d.k;
				d.k += d.clpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
				));
			}
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		}

		if (left->wall->t >= 0)
		{
			d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
			else
				d.ty = d.zu;
			d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
			d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallTop[j] - 1;
			while (++i < iw->d.bottom[left->x + j])
			{
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h));
				d.ty += d.dty;
			}
			if (iw->d.wallTop[j] < iw->d.top[j + left->x] &&
				iw->d.top[left->x + j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.top[left->x + j] - 1;
			else if (iw->d.wallTop[j] >= iw->d.top[j + left->x] &&
				iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.wallTop[j] - 1;
		}
		//iw->d.top[left->x + j] = iw->d.bottom[left->x + j];

		d.ang += d.dang;
	}
}

void	draw_inclined_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

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
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.frcoef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.clcoef = d.frcoef;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len);
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		//printf("wd %f frcoef %f\n", d.wall_dist, d.frcoef);

		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist * d.frcoef / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				d.frcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
					get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.frcoef) / 2.0f;
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j] && iw->sectors[iw->d.cs].cl.t >= 0)
		{
			if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				i = iw->d.wallTop[j] + 1;
			else
				i = iw->d.bottom[left->x + j] + 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
			while (--i >= iw->d.top[left->x + j])
			{
				d.weight = d.wall_dist * d.clcoef / d.k;
				d.k += d.clpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				d.clcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
					get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.clcoef) / 2.0f;
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
				));
			}
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		}

		if (left->wall->t >= 0)
		{
			d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
			d.zu = (float)left->zu + d.left_len * d.zudiff;
			d.zd = (float)left->zd + d.left_len * d.zddiff;
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
			else
				d.ty = d.zu;
			d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
			d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallTop[j] - 1;
			while (++i < iw->d.bottom[left->x + j])
			{
				set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h));
				d.ty += d.dty;
			}
			if (iw->d.wallTop[j] < iw->d.top[j + left->x] &&
				iw->d.top[left->x + j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.top[left->x + j] - 1;
			else if (iw->d.wallTop[j] >= iw->d.top[j + left->x] &&
				iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.wallTop[j] - 1;
		}
		//if (iw->d.wallTop[j] < iw->d.top[j + left->x])
		//	iw->d.bottom[left->x + j] = iw->d.top[left->x + j] - 1;
		//else
		//	iw->d.bottom[left->x + j] = iw->d.wallTop[j] - 1;
		//iw->d.top[left->x + j] = iw->d.bottom[left->x + j];

		d.ang += d.dang;
	}
}

void	draw_between_sectors_walls(t_sdl *iw, t_save_wall *left, t_save_wall *right)
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
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(tmp, l);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& tmp[WINDOW_W / 2 - left->x] < WINDOW_H / 2)
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	draw_between_sectors_bot_tex(iw, left, right, tmp);
	/*lz = get_ceil_z(iw, left->wall->x, left->wall->y);
	rz = get_ceil_z(iw, right->wall->x, right->wall->y);*/
	lz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	rz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(tmp, l);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& tmp[WINDOW_W / 2 - left->x] > WINDOW_H / 2)
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	draw_between_sectors_top_tex(iw, left, right, tmp);
	free(tmp);
}

int		*get_between_sectors_walls(t_sdl *iw, t_save_wall *left, t_save_wall *right, int **top)
{
	t_draw_line		l;
	int				lz;
	int				rz;
	int				*bottom;

	bottom = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	*top = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	l.x0 = left->x;
	l.x1 = right->x;
	/*lz = get_floor_z(iw, left->wall->x, left->wall->y);
	rz = get_floor_z(iw, right->wall->x, right->wall->y);*/
	lz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	rz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(bottom, l);
	/*draw_between_sectors_bot_tex(iw, left, right, tmp);*/

	/*lz = get_ceil_z(iw, left->wall->x, left->wall->y);
	rz = get_ceil_z(iw, right->wall->x, right->wall->y);*/
	lz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	rz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(*top, l);
	/*draw_between_sectors_top_tex(iw, left, right, tmp);*/
	return (bottom);
}

void	fill_portal(t_sdl *iw, t_save_wall *left, t_save_wall *right, t_sdl *iw2)
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

void	fill_bot_by_wallTop(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;

	i = -1;
	while (++i < len)
		if (iw->d.wallTop[i] < iw->d.bottom[left->x + i])
			iw->d.bottom[left->x + i] = iw->d.wallTop[i];
}

// void	fill_portal_rev(t_sdl *iw, t_sdl *iw2)
// {
// 	int		j;

// 	j = -1;
// 	while (++j <= WINDOW_W)
// 	{
// 		iw2->d.top[j] = iw->d.top[j];
// 		iw2->d.bottom[j] = iw->d.bottom[j];
// 	}
// }

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

void	change_saved_top_bot_between_lines(t_sdl *iw, int len, int left_x)
{
	int		j;

	// j = -1;
	// while (++j < len)
	// {
	// 	if (iw->d.top[j + left_x] > iw->d.save_top_betw[j])
	// 		iw->d.save_top_betw[j] = iw->d.top[j + left_x];
	// 	if (iw->d.bottom[left_x + j] < iw->d.save_bot_betw[j])
	// 		iw->d.save_bot_betw[j] = iw->d.bottom[left_x + j];
	// }
	if (iw->d.wallTop[0] > iw->d.save_top_betw[0])
	{
		j = -1;
		while (++j < len)
			iw->d.save_top_betw[j] = iw->d.wallTop[j];
	}
	if (iw->d.wallBot[0] < iw->d.save_bot_betw[0])
	{
		j = -1;
		while (++j < len)
			iw->d.save_bot_betw[j] = iw->d.wallBot[j];
	}
}

void	draw_glass_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		j;
	int		i;
	t_draw_glass	d;

	/*printf("GLAAAAAAAAAAASSSSSSSSS\n");
	j = -1;
	while (++j < len)
	{
		i = iw->d.save_top_betw[j] - 1;
		while (++i < iw->d.save_bot_betw[j])
			set_pixel(iw->sur, j + left->x, i, 0x00FF00);
	}*/
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

	i = iw->d.cs;
	iw->d.cs = left->wall->nextsector;
	d.nleft_zu = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	d.nleft_zd = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	if (left->zu < d.nleft_zu)
	{
		d.zudiff = (right->zu - left->zu) / d.len_lr;
		d.nleft_zu = left->zu;
	}
	else
	{
		d.nright_zu = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
		d.zudiff = (d.nright_zu - d.nleft_zu) / d.len_lr;
	}
	if (left->zd > d.nleft_zd)
	{
		d.zddiff = (right->zd - left->zd) / d.len_lr;
		d.nleft_zd = left->zd;
	}
	else
	{
		d.nright_zd = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
		d.zddiff = (d.nright_zd - d.nleft_zd) / d.len_lr;
	}
	iw->d.cs = i;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->glass]->w * iw->tsz[left->wall->glass] / 1000.0f;
		d.zu = (float)d.nleft_zu + d.left_len * d.zudiff;
		d.zd = (float)d.nleft_zd + d.left_len * d.zddiff;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->glass]->h / 1000.0f) /
			(float)(iw->d.save_bot_betw[j] - iw->d.save_top_betw[j]) * iw->tsz[left->wall->glass];
		d.ty = 0.0f;
		if (iw->d.save_top_betw[j] < iw->d.top[left->x + j])
		{
			d.ty += (float)(iw->d.top[left->x + j] - iw->d.save_top_betw[j]) * d.dty;
			i = iw->d.top[left->x + j] - 1;
		}
		else
			i = iw->d.save_top_betw[j] - 1;
	
		while (++i < iw->d.save_bot_betw[j] && i < iw->d.bottom[j + left->x])
		{
			d.pixel = get_pixel(iw->t[left->wall->glass],
				(int)d.tx % iw->t[left->wall->glass]->w,
				(int)d.ty % iw->t[left->wall->glass]->h);
			if (d.pixel != 0x000000)
				set_pixel(iw->sur, left->x + j, i, d.pixel);
			d.ty += d.dty;
		}
		d.ang += d.dang;
	}
}

void	draw_start(t_sdl *iw);
void	draw_next_sector(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_sdl	iw2;

	//printf("next_sector\n");
	iw2 = *iw;
	//	if (iw->sectors[iw->d.cs].cl.t < 0)
	fill_top_skybox(&iw2, left, right->x - left->x + 1);
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	iw->d.save_bot_betw = get_between_sectors_walls(&iw2, left, right, &iw->d.save_top_betw);
	draw_between_sectors_walls(&iw2, left, right);
	//fill_portal(iw, left, right, &iw2);
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
	iw2.d.prev_sector_wall = left->wall;
	draw_start(&iw2);
	if (left->wall->glass >= 0)
	{
		change_saved_top_bot_between_lines(iw, right->x - left->x + 1, left->x);
		draw_glass_tex(iw, left, right, right->x - left->x + 1);
	}
	free(iw->d.save_bot_betw);
	free(iw->d.save_top_betw);

	//fill_bot_by_wallTop(iw, left, right->x - left->x);
	fill_portal(iw, left, right, &iw2);
	/*iw->d.top = iw2.d.top;
	iw->d.bottom = iw2.d.bottom;*/
}

void	draw_next_sector_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_sdl	iw2;

	iw2 = *iw;
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	iw->d.save_bot_betw = get_between_sectors_walls(&iw2, left, right, &iw->d.save_top_betw);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& (iw->d.save_top_betw[WINDOW_W / 2 - left->x] > WINDOW_H / 2 ||
			iw->d.save_bot_betw[WINDOW_W / 2 - left->x] < WINDOW_H / 2))
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
		draw_floor_ceil_betw_tex_kernel(iw, left, right, len);
	else
		draw_inclined_floor_ceil_betw_tex_kernel(iw, left, right, len);
	if ((iw->d.wallBot[0] < 0 && iw->d.wallBot[len - 1] < 0) ||
		(iw->d.wallTop[0] >= WINDOW_H && iw->d.wallTop[len - 1] >= WINDOW_H))
	{
		free(iw->d.save_bot_betw);
		free(iw->d.save_top_betw);
		return;
	}
	/*fill_portal_rev(iw, &iw2);*/
	get_direction(&iw2);
	get_screen_line(&iw2);
	get_left_right_lines_points(&iw2);
	iw2.d.vw = 0;
	iw2.d.vwp = 0;
	if (left->x > iw2.d.screen_left)
		iw2.d.screen_left = left->x;
	if (right->x < iw2.d.screen_right)
		iw2.d.screen_right = right->x;
	fill_tb_by_slsr(&iw2);
	get_visible_walls(&iw2);
	get_left_right_visible_walls(&iw2);
	iw2.d.prev_sector = iw->d.cs;
	iw2.d.prev_sector_wall = left->wall;
	if (left->wall->glass >= 0)
	{
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_top,
			iw->k.m_save_top, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_bottom,
			iw->k.m_save_bottom, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
	}
	draw_start(&iw2);
	if (left->wall->glass >= 0)
	{
		change_saved_top_bot_between_lines(iw, len, left->x);
		draw_glass_tex_kernel(iw, left, right, len);
		/*clReleaseMemObject(iw->k.m_save_bottom);
		clReleaseMemObject(iw->k.m_save_top);*/
	}
	free(iw->d.save_bot_betw);
	free(iw->d.save_top_betw);
}

int		draw_picture(t_sdl *iw, t_picture *pic)
{
	t_draw_picture	d;
	int		i;
	int		j;

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
			(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
			(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx0 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d.rx0 = -(int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
			(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
			(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx1 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d.rx1 = -(int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));

	d.plen = fabsf(iw->d.screen.a * (float)pic->x0 + iw->d.screen.b * (float)pic->y0 + iw->d.screen.c) /
				sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	d.ry0_up = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	d.ry0_down = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	d.plen = fabsf(iw->d.screen.a * (float)pic->x1 + iw->d.screen.b * (float)pic->y1 + iw->d.screen.c) /
				sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	
	d.ry1_up = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	d.ry1_down = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	d.pic_x = 0;
	d.down = d.ry0_down - d.ry1_down;
	d.up = d.ry0_up - d.ry1_up;
	d.dx = d.rx0 - d.rx1;
	d.dy_down = d.down / d.dx;
	d.dy_up = d.up / d.dx;
	i = d.rx1;
	if (i < 0)
	{
		d.pic_x += iw->t[pic->t]->w * abs(i) / d.dx;
		d.dy_down += (float)(d.down / d.dx * abs(i));
		d.dy_up += (float)(d.up / d.dx * abs(i));
		i = 0;
	}
	while(i++ <= d.rx0 && i <= WINDOW_W)
	{
		if(iw->d.top_save[i] >= iw->d.bottom_save[i])
			d.pic_x += iw->t[pic->t]->w / d.dx;
		else
		{
			d.dy_plus = iw->t[pic->t]->h / ((d.ry1_down + (float)d.dy_down) - (d.ry1_up + (float)d.dy_up));
			j = d.ry1_up + (float)d.dy_up;
			if (j >= iw->d.top_save[i])
				d.pic_y = 0;
			else
			{
				d.pic_y = d.dy_plus * (iw->d.top_save[i] - j);
				j = iw->d.top_save[i];
			}
			while(j++ <= d.ry1_down + (float)d.dy_down && j <= iw->d.bottom_save[i])
			{
				set_pixel(iw->sur, i, j, get_pixel(iw->t[pic->t], (int)d.pic_x, (int)d.pic_y));
				d.pic_y += d.dy_plus;
			}
			d.pic_x += iw->t[pic->t]->w / d.dx;
			d.dy_down += (float)(d.down / d.dx);
			d.dy_up += (float)(d.up / d.dx);
		}
	}
	if (d.rx1 <= WINDOW_W / 2 && d.rx0 >= WINDOW_W / 2)
		return (1);
	return (0);
}

void	draw_pictures(t_sdl *iw, t_save_wall *left)
{
	t_picture	*pic;

	pic = left->wall->p;
	while (pic != 0)
	{
		if (draw_picture(iw, pic) && *(iw->v.look_picture) == 0)
			*(iw->v.look_picture) = pic;
		pic = pic->next;
	}
}

void	draw_all(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	
	if (left->wall->nextsector == -1)
	{
		/*draw_ceil_tex(iw, left, right, len);
		draw_inclined_floor_tex_kernel(iw, left, right, len);
		draw_wall_tex_kernel(iw, left, right, len);*/
		if (left->wall->p != 0)
		{
			// i = -1;
			// while (++i <= WINDOW_W)
			// {
			// 	iw->d.top_save[i] = iw->d.top[i];
			// 	iw->d.bottom_save[i] = iw->d.bottom[i];
			// }
			ft_memcpy(iw->d.top_save, iw->d.top, WINDOW_W * sizeof(int));
			ft_memcpy(iw->d.bottom_save, iw->d.bottom, WINDOW_W * sizeof(int));
		}
		if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_wall_floor_ceil_tex(iw, left, right, len);
		else
			draw_inclined_wall_floor_ceil_tex(iw, left, right, len);
		if (left->wall->p != 0)
			draw_pictures(iw, left);
	}
	else
	{
		if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_floor_ceil_tex(iw, left, right, len);
		else
			draw_inclined_floor_ceil_tex(iw, left, right, len);
		if (left->wall->nextsector != iw->d.prev_sector)
			draw_next_sector(iw, left, right);
	}
}

void	draw_all_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	if (left->wall->nextsector == -1)
	{
		/*draw_ceil_tex(iw, left, right, len);
		draw_inclined_floor_tex_kernel(iw, left, right, len);
		draw_wall_tex_kernel(iw, left, right, len);*/
		if (left->wall->p != 0)
		{
			clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_top,
				iw->k.m_save_top, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
			clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_bottom,
				iw->k.m_save_bottom, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
		}
		if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_wall_floor_ceil_tex_kernel(iw, left, right, len);
		else
			draw_inclined_wall_floor_ceil_tex_kernel(iw, left, right, len);
		if (left->wall->p != 0)
			draw_pictures_kernel(iw, left);
	}
	else
	{
		if (left->wall->nextsector != iw->d.prev_sector)
			draw_next_sector_kernel(iw, left, right, len);
		else if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_floor_ceil_tex_kernel(iw, left, right, len);
		else
			draw_inclined_floor_ceil_tex_kernel(iw, left, right, len);
	}
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
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zd) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zd) / (int)right->plen + iw->p.rotup;
	brez_line(iw->d.wallBot, l);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zu) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zu) / (int)right->plen + iw->p.rotup;
	brez_line(iw->d.wallTop, l);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& iw->d.screen_left < WINDOW_W / 2 && iw->d.screen_right > WINDOW_W / 2)
	{
		if (left->wall->nextsector != -1 && iw->v.look_portal == 0)
			iw->v.look_portal = left->wall;
		if (left->wall->nextsector == -1)
		{
			*(iw->v.look_wall) = left->wall;
			*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
		}
		else if (left->wall->nextsector == iw->d.prev_sector)
		{
			*(iw->v.look_wall) = left->wall;
			*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
		}
	}
	if (iw->v.kernel)
		draw_all_kernel(iw, left, right, right->x - left->x + 1);
	else
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

t_save_wall_pairs	*get_closest_between_pair(t_save_wall_pairs	*pair)
{
	t_save_wall_pairs	*save;
	t_save_wall_pairs	*tmp;

	save = 0;
	tmp = pair->next;
	while (tmp != 0)
	{
		//(bx-ax)*(py-ay)-(by-ay)*(px-ax)
		if (tmp != pair)
			if ((tmp->left->x >= pair->left->x && tmp->left->x < pair->right->x &&
				((pair->left->wall->x - pair->right->wall->x) * (tmp->left->p.y - pair->right->wall->y) -
				(pair->left->wall->y - pair->right->wall->y) * (tmp->left->p.x - pair->right->wall->x) > 0)) ||
					(tmp->right->x > pair->left->x && tmp->right->x <= pair->right->x &&
				((pair->left->wall->x - pair->right->wall->x) * (tmp->right->p.y - pair->right->wall->y) -
						(pair->left->wall->y - pair->right->wall->y) * (tmp->right->p.x - pair->right->wall->x) > 0)))
				save = tmp;
		tmp = tmp->next;
	}
	return (save);
}

// t_save_wall_pairs	*get_closest_between_pair(t_save_wall_pairs	*pair)
// {
// 	t_save_wall_pairs_closest	save;
// 	t_save_wall_pairs	*tmp;

// 	save.tmp = 0;
// 	save.lr = -1;
// 	tmp = pair->next;
// 	while (tmp != 0)
// 	{
// 		//(bx-ax)*(py-ay)-(by-ay)*(px-ax)
// 		if (tmp != pair)
// 			if (tmp->left->x >= pair->left->x && tmp->left->x < pair->right->x &&
// 				((pair->left->wall->x - pair->right->wall->x) * (tmp->left->p.y - pair->right->wall->y) -
// 					(pair->left->wall->y - pair->right->wall->y) * (tmp->left->p.x - pair->right->wall->x) > 0))
// 			{
// 				if (save.lr == -1 || (save.lr == 0 && save.tmp->left->len < tmp->left->len) ||
// 						(save.lr == 1 && save.tmp->right->len < tmp->left->len))
// 				{
// 					save.lr = 0;
// 					save.tmp = tmp;
// 				}

// 			}
// 			else if(tmp->right->x > pair->left->x && tmp->right->x <= pair->right->x &&
// 				((pair->left->wall->x - pair->right->wall->x) * (tmp->right->p.y - pair->right->wall->y) -
// 					(pair->left->wall->y - pair->right->wall->y) * (tmp->right->p.x - pair->right->wall->x) > 0))
// 				if (save.lr == -1 || (save.lr == 0 && save.tmp->left->len < tmp->right->len) ||
// 						(save.lr == 1 && save.tmp->right->len < tmp->right->len))
// 				{
// 					save.lr = 0;
// 					save.tmp = tmp;
// 				}
// 		tmp = tmp->next;
// 	}
// 	return (save.tmp);
// }

void	new_sort_pairs(t_sdl *iw)
{
	t_save_wall_pairs	start;
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;
	t_save_wall_pairs	*after;
	int					i;

	start.next = iw->d.vwp;
	tmp = &start;
	i = 0;
	while (tmp->next != 0)
	{
		i++;
		if (i > 1000)
			break;
		if ((after = get_closest_between_pair(tmp->next)) != 0)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			tmp2->next = after->next;
			after->next = tmp2;
		}
		else
			tmp = tmp->next;
		//////
		/*t_save_wall_pairs *ttt;
		ttt = iw->d.vwp;
		while (ttt != 0)
		{
			printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
			ttt = ttt->next;
		}
		printf("\n\n");*/
		/////
	}
	iw->d.vwp = start.next;
}

void	draw_skybox(t_sdl *iw)
{
	int j;
	int i;
	t_draw_skybox	d;

	d.rot = iw->p.rot - iw->v.angle;
	if (d.rot < 0.0f)
		d.rot += G360;
	d.sky_x = d.rot * ((double)iw->t[iw->l.skybox]->w) / G360;
	d.dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
	d.dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	j = -1;
	while (++j < WINDOW_W)
	{
		d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
		d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
		d.sky_y += d.dy * iw->d.top[j];
		i = iw->d.top[j] - 1;
		while (++i <= iw->d.bottom[j] && i < WINDOW_H)
		{
			set_pixel(iw->sur, j, i, get_pixel(iw->t[iw->l.skybox],
				(int)d.sky_x, (int)d.sky_y));
			d.sky_y += d.dy;
		}
		d.sky_x += d.dx;
		if (d.sky_x >= iw->t[iw->l.skybox]->w)
			d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
	}
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
	/*t_save_wall_pairs *ttt;
	ttt = iw->d.vwp;
	while (ttt != 0)
	{
		printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
		ttt = ttt->next;
	}
	printf("\n\n");*/
	/////

	new_sort_pairs(iw);

	//////
	/*printf("sorted:\n");
	ttt = iw->d.vwp;
	while (ttt != 0)
	{
		printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
		ttt = ttt->next;
	}
	printf("\n\n");*/
	/////

	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		draw_left_right(iw, tmp->left, tmp->right);
		tmp = tmp->next;
		// iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_sur, CL_TRUE, 0,
		// 	WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
		// SDL_UpdateWindowSurface(iw->win);
		// char s[500];
		// read(0, s, 10);
		//system("PAUSE");
	}
	free_walls(iw);
	free_pairs(iw);
}

void	draw(t_sdl *iw)
{
	set_top_bottom(iw);
	if ((iw->d.cs = get_sector(iw)) == -1)
		return;
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
	/*t_save_wall *tmp;
	tmp = iw->d.vw;
	while (tmp != 0)
	{
		if (tmp->wall == &iw->walls[0] || tmp->wall == &iw->walls[1] || tmp->wall == &iw->walls[7] || tmp->wall == &iw->walls[8])
		printf("x %d len %f\n", tmp->x, tmp->len, tmp->wall->x, tmp->wall->y, tmp->zu,
			tmp->zd, tmp->plen);
		tmp = tmp->next;
	}
	printf("\n\n");*/
	////////////
	iw->d.prev_sector = -1;
	iw->d.prev_sector_wall = 0;
	if (iw->v.kernel)
	{
		/*clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_sur, CL_TRUE, 0,
			WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);*/
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_top, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_bottom, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	}
	if (iw->v.mouse_mode == 1)
	{
		iw->v.look_portal = 0;
		*(iw->v.look_wall) = 0;
		*(iw->v.look_sector) = 0;
		*(iw->v.look_picture) = 0;
	}
	draw_start(iw);
	if (!iw->v.kernel)
		draw_skybox(iw);
	else
		draw_skybox_kernel(iw);

	if (iw->v.kernel)
		iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_sur, CL_TRUE, 0,
			WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
}

void	read_textures(t_sdl *iw)
{
	iw->t[0] = SDL_LoadBMP("textures/0.bmp");
	iw->tsz[0] = 1.0f;
	iw->t[1] = SDL_LoadBMP("textures/1.bmp");
	iw->tsz[1] = 1.0f;
	iw->t[2] = SDL_LoadBMP("textures/2.bmp");
	iw->tsz[2] = 1.0f;
	iw->t[3] = SDL_LoadBMP("textures/3.bmp");
	iw->tsz[3] = 1.0f;
	iw->t[4] = SDL_LoadBMP("textures/4.bmp");
	iw->tsz[4] = 1.0f;
	iw->t[5] = SDL_LoadBMP("textures/5.bmp");
	iw->tsz[5] = 1.0f;
	iw->t[6] = SDL_LoadBMP("textures/6.bmp");
	iw->tsz[6] = 1.0f;
	iw->t[7] = SDL_LoadBMP("textures/7.bmp");
	iw->tsz[7] = 1.0f;
	iw->t[8] = SDL_LoadBMP("textures/8.bmp");
	iw->tsz[8] = 1.0f;
	iw->t[9] = SDL_LoadBMP("textures/9.bmp");
	iw->tsz[9] = 1.0f;
	iw->t[10] = SDL_LoadBMP("textures/10.bmp");
	iw->tsz[10] = 1.0f;
	iw->t[11] = SDL_LoadBMP("textures/11.bmp");
	iw->tsz[11] = 1.0f;
	iw->t[12] = SDL_LoadBMP("textures/12.bmp");
	iw->tsz[12] = 1.0f;
	iw->t[13] = SDL_LoadBMP("textures/13.bmp");
	iw->tsz[13] = 1.0f;
	iw->t[14] = SDL_LoadBMP("textures/14.bmp");
	iw->tsz[14] = 1.0f;
	iw->t[15] = SDL_LoadBMP("textures/15.bmp");
	iw->tsz[15] = 1.0f;
	iw->t[16] = SDL_LoadBMP("textures/16.bmp");
	iw->tsz[16] = 1.0f;
	//Uint8 *target_pixel = (Uint8 *)(iw->t)[0]->pixels;
	//set_pixel((iw->t)[0], 0, 0, 0xFF0000);
	//printf("%d\n", read_pixel((iw->t)[0], 2, 0));
}

void	get_def(t_sdl *iw)
{
	iw->p.x = 8219;
	iw->p.y = 2149; //-2360
	iw->p.z = 600;
	iw->p.introt = 271;
	iw->p.rot = (float)iw->p.introt * G1;
	iw->p.rotup = 12; //550
	iw->v.ls = 0;
	iw->v.angle = (float)WINDOW_W / (float)WINDOW_H * 22.0f * G1;// 0.698132f;
	iw->v.kernel = 0;
	load_kernel(&iw->k);
	//fill_floor_coefficients(iw);
	iw->v.front = -1;
	iw->v.back = -1;
	iw->v.left = -1;
	iw->v.right = -1;
	iw->v.rot_left = -1;
	iw->v.rot_right = -1;
	iw->v.rot_up = -1;
	iw->v.rot_down = -1;
	iw->v.fall = -1;
	iw->v.jump_time = -1;
	iw->v.jump = 0;
	iw->v.accel = 9.81f;
	iw->loop_update_time = clock();
	iw->v.fps = 0;
	iw->winrect.x = 0;
	iw->winrect.y = 0;
	iw->winrect.w = WINDOW_W;
	iw->winrect.h = WINDOW_H;
	iw->v.tex_to_fill = 0;
	iw->v.scroll_first_tex = 0;
	iw->v.mouse_mode = 1;
	iw->v.scroll_tex_rect.h = 100;
	iw->v.scroll_tex_rect.w = WINDOW_W;
	iw->v.scroll_tex_rect.x = 0;
	iw->v.scroll_tex_rect.y = WINDOW_H;
	iw->v.look_wall = (t_wall **)malloc(sizeof(t_wall *));
	*(iw->v.look_wall) = 0;
	iw->v.look_sector = (t_sector **)malloc(sizeof(t_sector *));
	*(iw->v.look_sector) = 0;
	iw->v.look_picture = (t_picture **)malloc(sizeof(t_picture *));
	*(iw->v.look_picture) = 0;
	iw->v.look_portal = 0;
	iw->v.changing_fc = 0;
	iw->v.chang_fc_rect.h = 100;
	iw->v.chang_fc_rect.w = 40;
	iw->v.chang_fc_rect.x = 0;
	iw->v.chang_fc_rect.y = WINDOW_H + 100;
	iw->l.skybox = 13;
	iw->v.edit_mode = 0;
}

void	get_kernel_mem(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < TEXTURES_COUNT)
	{
		/*if (i != 1)
		{*/
		iw->k.m_t[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t[i]->w * iw->t[i]->h * 3, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t[i], CL_TRUE, 0,
			iw->t[i]->w * iw->t[i]->h * 3, iw->t[i]->pixels, 0, NULL, NULL);
		//}
	}
	iw->k.m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_sur = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY,
		(WINDOW_W + 1) * (WINDOW_H + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		25 * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		19 * sizeof(float), NULL, &iw->k.ret);
	iw->k.m_top_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_bot_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
}

//void	calculate_not_squared_wall_picture(t_sdl *iw, t_wall *wall, t_picture *pic)
//{
//	float	ra;
//	float	rb;
//	float	rc;
//	float	y1;
//	float	y2;
//	float	disk;
//
//	get_wall_line2(wall);
//	ra = 1.0f + powf(wall->l.b, 2.0f) / powf(wall->l.a, 2.0f);
//	rb = 2.0f * wall->l.b / wall->l.a * ((float)wall->x + wall->l.c / wall->l.a) - (float)wall->y;
//	rc = wall->l.c / wall->l.a * (wall->l.c / wall->l.a + 2.0f * (float)wall->x) + powf(wall->y, 2.0f) +
//		(float)wall->x;// -powf(pic->left_plus, 2.0f);
//
//	disk = sqrtf(powf(rb, 2.0f) - ra * (rc - powf(pic->left_plus, 2.0f)));
//	y1 = (-rb + disk) / ra;
//	y2 = (-rb - disk) / ra;
//	if (wall->next->y > wall->y)
//		pic->y1 = ((int)y1 > wall->y) ? (int)y1 : (int)y2;
//	else
//		pic->y1 = ((int)y1 < wall->y) ? (int)y1 : (int)y2;
//	pic->x1 = (int)(-(wall->l.b * (float)pic->y1 + wall->l.c) / wall->l.a);
//
//	disk = sqrtf(powf(rb, 2.0f) - ra * (rc - powf(pic->left_plus + pic->tw, 2.0f)));
//	y1 = (-rb + disk) / ra;
//	y2 = (-rb - disk) / ra;
//	if (wall->next->y > wall->y)
//		pic->y0 = ((int)y1 > wall->y) ? (int)y1 : (int)y2;
//	else
//		pic->y0 = ((int)y1 < wall->y) ? (int)y1 : (int)y2;
//	pic->x0 = (int)(-(wall->l.b * (float)pic->y0 + wall->l.c) / wall->l.a);
//}

void	calculate_not_squared_wall_picture(t_sdl *iw, t_wall *wall, t_picture *pic)
{
	float	alpha;

	alpha = get_vectors_angle(wall->next->x - wall->x, wall->next->y - wall->y,
		((wall->next->x > wall->x) ? 10 : -10), 0);
	pic->x1 = (float)wall->x + (float)pic->left_plus * cosf(alpha) *
		((wall->next->x > wall->x) ? 1.0f : -1.0f);
	pic->y1 = (float)wall->y + (float)pic->left_plus * sinf(alpha) *
		((wall->next->y > wall->y) ? 1.0f : -1.0f);

	pic->x0 = (float)wall->x + (float)(pic->left_plus + pic->tw) * cosf(alpha) *
		((wall->next->x > wall->x) ? 1.0f : -1.0f);
	pic->y0 = (float)wall->y + (float)(pic->left_plus + pic->tw) * sinf(alpha) *
		((wall->next->y > wall->y) ? 1.0f : -1.0f);
}

void	calculate_picture(t_sdl *iw, t_wall *wall, t_picture *pic)
{
	if (wall->x == wall->next->x)
	{
		pic->x1 = wall->x;
		pic->x0 = wall->x;
		if (wall->y > wall->next->y)
		{
			pic->y1 = wall->y - pic->left_plus;
			pic->y0 = pic->y1 - pic->tw;
		}
		else
		{
			pic->y1 = wall->y + pic->left_plus;
			pic->y0 = pic->y1 + pic->tw;
		}
	}
	else if (wall->y == wall->next->y)
	{
		pic->y1 = wall->y;
		pic->y0 = wall->y;
		if (wall->x > wall->next->x)
		{
			pic->x1 = wall->x - pic->left_plus;
			pic->x0 = pic->x1 - pic->tw;
		}
		else
		{
			pic->x1 = wall->x + pic->left_plus;
			pic->x0 = pic->x1 + pic->tw;
		}
	}
	else
		calculate_not_squared_wall_picture(iw, wall, pic);

	pic->zd = pic->zu - pic->tw * iw->t[pic->t]->h * 120 / iw->t[pic->t]->w / 100;
}

void	add_picture1(t_sdl *iw)
{
	t_picture	*tmp;

	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 15;
	tmp->next = 0;
	iw->walls[5].p = tmp;
	calculate_picture(iw, &iw->walls[5], tmp);
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 5;
	tmp->next = 0;
	iw->walls[18].p = tmp;
	calculate_picture(iw, &iw->walls[18], tmp);
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 6;
	tmp->next = 0;
	iw->walls[19].p = tmp;
	calculate_picture(iw, &iw->walls[19], tmp);
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 7;
	tmp->next = 0;
	iw->walls[20].p = tmp;
	calculate_picture(iw, &iw->walls[20], tmp);
}

int		main(void)
{
	t_sdl	iw;

	get_def(&iw);
	read_textures(&iw);
	get_kernel_mem(&iw);
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	iw.arial_font = TTF_OpenFont("fonts/ARIAL.TTF", 24);
	SDL_SetRelativeMouseMode(iw.v.mouse_mode);
	iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H + 100 + 100, SDL_WINDOW_SHOWN);
	//iw.ren = SDL_CreateRenderer(iw.win, -1, 0);
	iw.sur = SDL_GetWindowSurface(iw.win);
	draw_tex_to_select(&iw);
	draw_menu(&iw);
	// draw
	get_map(&iw);
	add_picture1(&iw); ///////
	//get_animation(&iw);
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