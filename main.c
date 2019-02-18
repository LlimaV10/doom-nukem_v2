#include "guardians.h"
#include <stdio.h>

void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	if (x >= 0 && x < WINDOW_W && y >= 0 && y < WINDOW_H + 300)
	{
		Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
		*(Uint32 *)target_pixel = pixel;
	}
}

int		get_light_color(int color, t_picture *light)
{
	if (light == 0 || light->t != 18)
		return (color);
	return ((((int)((float)(color >> 16) * 0.2f)) << 16) +
		(((int)((float)((color >> 8) - (color >> 16 << 8))
		* 0.2f)) << 8) + (int)((float)(color - (color
		>> 8 << 8)) * 0.2f));
}

Uint32	get_pixel(t_packaging_texture *sur, const int x, const int y)
{
	uint8_t *v;
	int		bpp;

	if (x < 0 || x >= sur->w || y < 0 || y >= sur->h)
		return (0);
	bpp = sur->bpp;
	v = (uint8_t *)sur->pixels + y * sur->pitch + x * bpp;
	// printf("bpp %d\n", bpp);
	// if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	// 	printf("big endian\n");
	// else
	// 	printf("small endian\n");*/
	// *if (bpp == 1)
	// 		return (*p);
	// 	if (bpp == 2)
	// 		return (*(uint16_t *)p);
	// 	if (bpp == 3)
	// 	{
	// 		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	// 			return (p[0] << 16 | p[1] << 8 | p[2]);
	// 		else
	return (v[0] | v[1] << 8 | v[2] << 16);
	/*}
	if (bpp == 4)
		return (*(uint32_t *)p);
	return (0);*/
}

Uint32	get_pixel_surface(SDL_Surface *sur, const int x, const int y)
{
	uint8_t *v;
	int		bpp;

	if (x < 0 || x >= sur->w || y < 0 || y >= sur->h)
		return (0);
	bpp = sur->format->BytesPerPixel;
	v = (uint8_t *)sur->pixels + y * sur->pitch + x * bpp;
	return (v[0] | v[1] << 8 | v[2] << 16);
}

void	exit_x(t_sdl *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	TTF_Quit();
	SDL_Quit();
	system("leaks doom-nukem");
	exit(0);
}

int		in_sec_xy(t_sdl *iw, int sector, int x, int y)
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

int		get_sector_by_pointer(t_sdl *iw, t_sector *sec)
{
	int		i;

	i = -1;
	while (++i < iw->v.sc)
		if (&iw->sectors[i] == sec)
			return (i);
	return (0);
}

int		get_wall_by_pointer(t_sdl *iw, t_sector *sec, t_wall *w)
{
	int		i;

	i = sec->sw - 1;
	while (++i < sec->sw + sec->nw)
		if (iw->walls[i].next == w)
			return (i);
	return (0);
}

void	add_sector_animation(t_sdl *iw)
{
	t_sector_animation	*tmp;

	tmp = (t_sector_animation *)malloc(sizeof(t_sector_animation));
	tmp->trigger = (t_picture *)iw->v.f_button_pointer;
	tmp->sector = get_sector_by_pointer(iw, *(iw->v.look_sector));
	tmp->speed = 1;
	tmp->dy = 0;
	tmp->status = 0;
	tmp->curr_dy = 0;
	iw->v.sector_anim = tmp;
}

void	do_sector_animation_step(t_sdl *iw, t_sector_animation *a, int dz)
{
	int			w;
	t_picture	*p;
	int			i;

	if (a->type == 0)
	{
		iw->sectors[a->sector].fr.z += dz;
		iw->sectors[a->sector].cl.z += dz;
		w = iw->sectors[a->sector].sw - 1;
		while (++w < iw->sectors[a->sector].sw + iw->sectors[a->sector].nw)
		{
			p = iw->walls[w].p;
			while (p)
			{
				p->zu += dz;
				p->zd += dz;
				p = p->next;
			}
		}
	}
	else if (a->type == 1)
		iw->sectors[a->sector].fr.z += dz;
	else if (a->type == 2)
		iw->sectors[a->sector].cl.z += dz;
	i = a->sector;
	if (iw->sectors[i].fr.n != 0)
		iw->sectors[i].fr.n->d = -iw->sectors[i].fr.n->a * iw->sectors[i].fr.x -
		iw->sectors[i].fr.n->b * iw->sectors[i].fr.y - iw->sectors[i].fr.n->c * iw->sectors[i].fr.z;
	if (iw->sectors[i].cl.n != 0)
		iw->sectors[i].cl.n->d = -iw->sectors[i].cl.n->a * iw->sectors[i].cl.x -
		iw->sectors[i].cl.n->b * iw->sectors[i].cl.y - iw->sectors[i].cl.n->c * iw->sectors[i].cl.z;
}

void	draw_selected_walls_to_be_animated(t_sdl *iw)
{
	t_save_wall	*tmp;
	int			i;
	int			y;

	tmp = *(iw->vw_save);
	while (tmp)
	{
		i = -1;
		while (++i < iw->v.wall_anim->count_walls)
			if (iw->walls[iw->v.wall_anim->walls[i]].next == tmp->wall ||
				iw->walls[iw->v.wall_anim->walls[i]].next->next == tmp->wall)
			{
				y = 0;
				while (++y < WINDOW_H)
					set_pixel(iw->sur, tmp->x, y, 0xFF0000);
				break;
			}
		tmp = tmp->next;
	}
}

void	add_wall_to_wall_animation2(t_sdl *iw, int add_wall)
{
	int		i;
	t_wall_animation	*tmp;

	tmp = iw->v.wall_anim;
	i = -1;
	while (++i < tmp->count_walls)
		if (add_wall == tmp->walls[i]/* || *(iw->v.look_wall) == iw->walls[tmp->walls[i]].next
			|| (*(iw->v.look_wall))->next == &iw->walls[tmp->walls[i]]*/)
			return;
	tmp->walls[tmp->count_walls] = add_wall;
	tmp->count_walls++;
	if (tmp->count_walls == COUNT_WALLS_TO_ANIM)
	{
		iw->v.f_button_mode = 0;
		iw->v.f_button_pointer = 0;
		iw->v.submenu_mode = 5;
		draw_submenu(iw);
	}
}

void	add_wall_to_wall_animation(t_sdl *iw)
{
	t_wall_animation	*tmp;
	int					add_wall;

	add_wall = get_wall_by_pointer(iw, *(iw->v.look_sector), *(iw->v.look_wall));
	if (iw->walls[add_wall].nextsector != -1 ||
		iw->walls[add_wall].next->nextsector != -1 ||
		iw->walls[add_wall].next->next->nextsector != -1)
		return;
	if (iw->v.wall_anim == 0)
	{
		tmp = (t_wall_animation *)malloc(sizeof(t_wall_animation));
		tmp->dx = 0;
		tmp->dy = 0;
		tmp->curr_dx = 0;
		tmp->curr_dy = 0;
		tmp->speed = 1;
		tmp->status = 0;
		tmp->count_walls = 1;
		tmp->walls[0] = add_wall;
		tmp->trigger = (t_picture *)iw->v.f_button_pointer;
		iw->v.wall_anim = tmp;
	}
	else
		add_wall_to_wall_animation2(iw, add_wall);
}

void	calculate_pictures_list(t_sdl *iw, t_wall *wall, t_picture *p)
{
	while (p)
	{
		calculate_picture(iw, wall, p);
		p = p->next;
	}
}

void	do_wall_animation_step_dx(t_sdl *iw, t_wall_animation *a, int dx)
{
	int		i;

	i = -1;
	while (++i < a->count_walls)
	{
		if (a->moving_type == 1 &&
			iw->walls[a->walls[i]].next->next->y > iw->walls[a->walls[i]].next->y)
		{
			iw->walls[a->walls[i]].next->x -= dx;
			iw->walls[a->walls[i]].next->next->x -= dx;
		}
		else
		{
			iw->walls[a->walls[i]].next->x += dx;
			iw->walls[a->walls[i]].next->next->x += dx;
		}
		get_wall_line2(&iw->walls[a->walls[i]]);
		get_wall_line2(iw->walls[a->walls[i]].next);
		get_wall_line2(iw->walls[a->walls[i]].next->next);
		calculate_pictures_list(iw, &iw->walls[a->walls[i]], iw->walls[a->walls[i]].p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next, iw->walls[a->walls[i]].next->p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next->next, iw->walls[a->walls[i]].next->next->p);
	}
}

void	do_wall_animation_step_dy(t_sdl *iw, t_wall_animation *a, int dy)
{
	int		i;

	i = -1;
	while (++i < a->count_walls)
	{
		if (a->moving_type == 2 &&
			iw->walls[a->walls[i]].next->next->x > iw->walls[a->walls[i]].next->x)
		{
			iw->walls[a->walls[i]].next->y -= dy;
			iw->walls[a->walls[i]].next->next->y -= dy;
		}
		else
		{
			iw->walls[a->walls[i]].next->y += dy;
			iw->walls[a->walls[i]].next->next->y += dy;
		}
		get_wall_line2(&iw->walls[a->walls[i]]);
		get_wall_line2(iw->walls[a->walls[i]].next);
		get_wall_line2(iw->walls[a->walls[i]].next->next);
		calculate_pictures_list(iw, &iw->walls[a->walls[i]], iw->walls[a->walls[i]].p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next, iw->walls[a->walls[i]].next->p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next->next, iw->walls[a->walls[i]].next->next->p);
	}
}

void	exit_editing_sector_animation(t_sdl *iw)
{
	if (iw->v.submenu_mode == 2 || iw->v.submenu_mode == 3)
		do_sector_animation_step(iw, iw->v.sector_anim, -iw->v.sector_anim->dy);
	free(iw->v.sector_anim);
	iw->v.sector_anim = 0;
	iw->v.submenu_mode = 0;
	draw_submenu(iw);
}

void	exit_editing_wall_animation(t_sdl *iw)
{
	///////////////////////
	if (iw->v.submenu_mode >= 7)
	{
		do_wall_animation_step_dx(iw, iw->v.wall_anim, -iw->v.wall_anim->dx);
		do_wall_animation_step_dy(iw, iw->v.wall_anim, -iw->v.wall_anim->dy);
	}
	free(iw->v.wall_anim);
	iw->v.wall_anim = 0;
	iw->v.submenu_mode = 0;
	draw_submenu(iw);
}

void	add_picture(t_sdl *iw, t_wall *wall)
{
	t_picture	*tmp;

	if (iw->v.submenu_mode != 0 || iw->v.f_button_mode != 0)
		return;
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 500;
	tmp->zu = get_ceil_z(iw, wall->x, wall->y) - 100;
	tmp->tw = 500;
	tmp->t = iw->v.tex_to_fill;
	tmp->next = wall->p;
	wall->p = tmp;
	if (iw->v.tex_to_fill == 17 || iw->v.tex_to_fill == 18)
	{
		iw->v.f_button_mode = 1;
		iw->v.f_button_pointer = (void *)tmp;
	}
	else if (iw->v.tex_to_fill == 19)
	{
		iw->v.submenu_mode = 4;
		draw_submenu(iw);
		iw->v.f_button_pointer = (void *)tmp;
	}
	calculate_picture(iw, wall, tmp);
}

void	delete_wall_animation(t_sdl *iw, t_picture *pic)
{
	t_wall_animation	*tmp;
	t_wall_animation	*tmp2;

	if (iw->wall_animations == 0)
		return;
	if (iw->wall_animations->trigger == pic)
	{
		tmp2 = iw->wall_animations;
		do_wall_animation_step_dx(iw, tmp2, -tmp2->curr_dx);
		do_wall_animation_step_dy(iw, tmp2, -tmp2->curr_dy);
		iw->wall_animations = iw->wall_animations->next;
		free(tmp2);
	}
	else
	{
		tmp = iw->wall_animations;
		while (tmp->next)
		{
			if (tmp->next->trigger == pic)
			{
				tmp2 = tmp->next;
				do_wall_animation_step_dx(iw, tmp2, -tmp2->curr_dx);
				do_wall_animation_step_dy(iw, tmp2, -tmp2->curr_dy);
				tmp->next = tmp->next->next;
				free(tmp2);
				return;
			}
			tmp = tmp->next;
		}
	}
}

void	delete_light_and_animations(t_sdl *iw, t_picture *pic)
{
	int		sec;
	t_sector_animation	a;
	t_sector_animation	*tmp;
	t_sector_animation	*tmp2;

	sec = -1;
	while (++sec < iw->v.sc)
		if (iw->sectors[sec].light == pic)
			iw->sectors[sec].light = 0;
	a.next = iw->sector_animations;
	tmp = &a;
	while (tmp->next)
	{
		if (tmp->next->trigger == pic)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			do_sector_animation_step(iw, tmp2, -tmp2->curr_dy);
			free(tmp2);
		}
		else
			tmp = tmp->next;
	}
	iw->sector_animations = a.next;
	delete_wall_animation(iw, pic);
}

void	delete_picture(t_wall *wall, t_picture *pic, t_sdl *iw)
{
	t_picture	*tmp;

	if (iw->v.submenu_mode != 0 || iw->v.f_button_mode != 0)
		return;
	delete_light_and_animations(iw, pic);
	if (pic == wall->p)
	{
		wall->p = wall->p->next;
		free(pic);
	}
	else
	{
		tmp = wall->p;
		while (tmp->next != 0)
		{
			if (tmp->next == pic)
				break;
			tmp = tmp->next;
		}
		if (tmp->next != 0)
		{
			tmp->next = tmp->next->next;
			free(pic);
		}
	}
}

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

void	draw_text_blue(t_sdl *iw, const char *s, int x, int y)
{
	SDL_Color	col;
	SDL_Rect	rect;
	SDL_Surface	*stext;

	col.a = 0;
	col.r = 0;
	col.g = 0;
	col.b = 170;
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
	char		*s;

	if (iw->v.game_mode)
		return;
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
	d.rect.y = 50;
	draw_text_number(iw, &d, "Fly mode: ", iw->v.fly_mode);
	if (iw->v.f_button_mode == 1)
	{
		draw_text(iw, "Select Sectors to control light by pressing F", 50, 100);
		draw_text(iw, "Press G to exit this mode", 50, 125);
	}
	else if (iw->v.f_button_mode == 2)
	{
		draw_text(iw, "Select Sector to be animated by pressing F", 50, 100);
		draw_text(iw, "Press G to exit this mode", 50, 125);
	}
	else if (iw->v.f_button_mode == 3)
	{
		draw_text(iw, "Select walls to be animated by pressing F", 50, 100);
		draw_text(iw, "Press G to exit this mode", 50, 125);
		if (iw->v.wall_anim != 0)
		{
			draw_text(iw, "Selected walls: ", 50, 150);
			draw_text(iw, (s = ft_itoa(iw->v.wall_anim->count_walls)), 250, 150);
			free(s);
		}
	}
	// if (iw->v.edit_mode == 0)
	// 	draw_text(iw, "Wall texture editing mode", 0, 50);
	// else if (iw->v.edit_mode == 1)
	// 	draw_text(iw, "Pictures editing mode", 0, 50);

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
	if (iw->v.game_mode)
		return;
	draw_menu_sphere(iw, 100, "X");
	draw_menu_sphere(iw, 170, "Y");
	draw_up_down_menu_arrows(iw, 50);
	if (iw->v.changing_fc)
		draw_text(iw, "C", 15, WINDOW_H + 135);
	else
		draw_text(iw, "F", 15, WINDOW_H + 135);
	draw_text(iw, "R", 210, WINDOW_H + 135);
	draw_text(iw, "Sprites:", 250, WINDOW_H + 135);
	draw_text(iw, "Decor.", 335, WINDOW_H + 105);
	draw_text(iw, "Pickup", 335, WINDOW_H + 135);
	draw_text(iw, "Enemies", 335, WINDOW_H + 165);
}

void	ft_scaled_blit(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;
	int		p;

	i = -1;
	while (++i < rect->h)
	{
		j = -1;
		while (++j < rect->w)
		{
			p = get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h);
			if (p != 0x010000)
				set_pixel(winsur, rect->x + j, rect->y + i, p);
		}
	}
}

void	draw_tex_to_select(t_sdl *iw)
{
	int		i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return;
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

void	clear_sprites_tex_to_select(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = WINDOW_W;
	rect.h = 100;
	SDL_FillRect(iw->sur, &rect, 0x000000);
}

void	draw_decor_tex_to_select(t_sdl *iw)
{
	int		i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return;
	clear_sprites_tex_to_select(iw);
	iw->v.sprites_select_mode = 0;
	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = 100;
	rect.h = 100;
	i = iw->v.scroll_decor_sprites - 1;
	while (++i < DECOR_TEXTURES_COUNT && rect.x < WINDOW_W)
	{
		ft_scaled_blit(iw->t_decor[i], iw->sur, &rect);
		rect.x += 100;
	}
}

void	draw_pickup_tex_to_select(t_sdl *iw)
{
	int		i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return;
	clear_sprites_tex_to_select(iw);
	iw->v.sprites_select_mode = 1;
	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = 100;
	rect.h = 100;
	i = iw->v.scroll_pickup_sprites - 1;
	while (++i < PICK_UP_TEXTURES_COUNT && rect.x < WINDOW_W)
	{
		ft_scaled_blit(iw->t_pickup[i], iw->sur, &rect);
		rect.x += 100;
	}
}

void	draw_enemies_tex_to_select(t_sdl *iw)
{
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return;
	clear_sprites_tex_to_select(iw);
	iw->v.sprites_select_mode = 2;
	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = 100;
	rect.h = 100;
	ft_scaled_blit(iw->t_enemies[0], iw->sur, &rect);
	rect.x += 100;
	ft_scaled_blit(iw->t_enemies[8], iw->sur, &rect);
}

void	draw_selected_tex(t_sdl *iw)
{
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return;
	rect.x = WINDOW_W - 110;
	rect.y = 10;
	rect.w = 100;
	rect.h = 100;
	ft_scaled_blit(iw->t[iw->v.tex_to_fill], iw->sur, &rect);
}

void	draw_selected_sprite(t_sdl *iw)
{
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return;
	rect.x = WINDOW_W - 110;
	rect.y = 130;
	rect.w = 100;
	rect.h = 100;
	if (iw->v.selected_sprite_type == 0)
		ft_scaled_blit(iw->t_decor[iw->v.selected_sprite], iw->sur, &rect);
	else if (iw->v.selected_sprite_type == 1)
		ft_scaled_blit(iw->t_pickup[iw->v.selected_sprite], iw->sur, &rect);
	else if (iw->v.selected_sprite_type == 2)
	{
		if (iw->v.selected_sprite == 0)
			ft_scaled_blit(iw->t_enemies[0], iw->sur, &rect);
		else if (iw->v.selected_sprite == 1)
			ft_scaled_blit(iw->t_enemies[8], iw->sur, &rect);
	}
}

void	draw_submenu(t_sdl *iw)
{
	SDL_Rect	rect;
	char		*s;

	rect.x = WINDOW_W - 500;
	rect.y = WINDOW_H + 100;
	rect.w = 500;
	rect.h = 100;
	SDL_FillRect(iw->sur, &rect, 0x000000);
	if (iw->v.submenu_mode == 1)
	{
		draw_text(iw, "Choose type of sector animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "F  C  FC    Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 2)
	{
		draw_text(iw, "Set DZ of sector animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 3)
	{
		draw_text(iw, "Set speed of sector animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Speed:", WINDOW_W - 450, WINDOW_H + 135);
		draw_text(iw, (s = ft_itoa(iw->v.sector_anim->speed)), WINDOW_W - 370, WINDOW_H + 135);
		free(s);
		draw_text(iw, "+  -  OK  Exit", WINDOW_W - 340, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 4)
	{
		draw_text(iw, "Choose animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Sector  Walls   Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 5)
	{
		draw_text(iw, "Select walls moving type:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Same  Inv_X  Inv_Y   Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 6)
	{
		draw_text(iw, "Select walls moving priority:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "DXDY  DYDX   Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 7)
	{
		draw_text(iw, "Set DX of walls animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 8)
	{
		draw_text(iw, "Set DY of walls animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 9)
	{
		draw_text(iw, "Set speed of walls animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Speed:", WINDOW_W - 450, WINDOW_H + 135);
		draw_text(iw, (s = ft_itoa(iw->v.wall_anim->speed)), WINDOW_W - 370, WINDOW_H + 135);
		free(s);
		draw_text(iw, "+  -  OK  Exit", WINDOW_W - 340, WINDOW_H + 135);
	}
}

void	change_sector_animation_status(t_sdl *iw, t_picture *p)
{
	t_sector_animation	*tmp;

	tmp = iw->sector_animations;
	while (tmp)
	{
		if (tmp->trigger == p)
		{
			tmp->status = ((tmp->status == 0) ? 1 : 0);
			return;
		}
		tmp = tmp->next;
	}
}

void	change_wall_animation_status(t_sdl *iw, t_picture *p)
{
	t_wall_animation	*tmp;

	tmp = iw->wall_animations;
	while (tmp)
	{
		if (tmp->trigger == p)
		{
			tmp->status = ((tmp->status == 0) ? 1 : 0);
			return;
		}
		tmp = tmp->next;
	}
}

// HUD ////////////////////////////////////////////////////////////////

void	drawCircle(t_hud *den, int xc, int yc, int x, int y) 
{ 
	den->koord += 4;
	den->mas[den->i].x = xc - y;
	den->mas[den->i].y = yc - x;
	den->mas[150 - den->i].x = xc - x;
	den->mas[150 - den->i].y = yc - y;
	den->mas[den->i + 151].x = xc + x;
	den->mas[den->i + 151].y = yc - y;
	den->mas[300 - den->i].x = xc + y;
	den->mas[300 - den->i].y = yc - x;
	den->mas2[den->i].x = xc-y;
	den->mas2[den->i].y = yc+x;
	den->mas2[150 - den->i].x = xc-x;
	den->mas2[150 - den->i].y = yc+y;
	den->mas2[den->i + 150].x = xc+x;
	den->mas2[den->i + 150].y = yc+y;
	den->mas2[300 - den->i].x = xc+y;
	den->mas2[300 - den->i].y = yc+x;
	den->i += 1;
}

void	circle(t_hud *den, int xc, int yc)
{
	den->koord = 0;
	den->i = 0;
	int	r;

	den->rect.x = FOOTX - den->rad + 20;
	den->rect.y = FOOTY - den->rad + 2;
	den->rect.h = den->rad * 2 - 5;
	den->rect.w = den->rad * 2 - 5;
	r = den->rad;
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    drawCircle(den, xc, yc, x, y); 
    while (y >= x) 
    { 
        x++; 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
        drawCircle(den, xc, yc, x, y);  
    }
}

void	ft_line(t_sdl *iw, int x, int y, int color)
{
	int		i;

	i = x - 1;
	while (++i < x + 30)
		set_pixel(iw->sur, i, y, color);
}

void	make_health(t_hud *den, t_sdl *iw)
{
	int		i;
	char	*s;

	i = -1;
	while (++i < (3 * iw->p.health))
		ft_line(iw, den->mas[i].x, den->mas[i].y, 0xAA0000);
	i = -1;
	while (++i < (3 * den->shell))
		ft_line(iw, den->mas2[i].x, den->mas2[i].y, 0x0000AA);
	//SDL_BlitSurface(den->enot, NULL, iw->sur, &den->rect);
	ft_scaled_blit(den->enot, iw->sur, &den->rect);
	draw_text_blue(iw, "/", WINDOW_W - 50, 185);
	if (iw->guns.bullets_in_stock[iw->guns.gun_in_hands] > 0)
	{
		draw_text_blue(iw, (s = ft_itoa(iw->guns.bullets_in_stock[iw->guns.gun_in_hands])), WINDOW_W - 40, 185);
		free(s);
	}
}

//////////////////////////////////////////////////////////////////

void	draw_gun(t_sdl *iw)
{
	int		i;
	int		j;
	int		to_i;
	int		start_j;
	int		to_j;
	int		pixel;
	SDL_Rect	changed_rect;

	changed_rect = iw->guns.t_rect[iw->guns.t];
	changed_rect.x += iw->v.weapon_change_x;
	changed_rect.y += iw->v.weapon_change_y + iw->v.weapon_change_y_hide;
	to_i = changed_rect.y + changed_rect.h;
	if (to_i > WINDOW_H)
		to_i = WINDOW_H;
	if (changed_rect.y < 0)
		i = -1;
	else
		i = changed_rect.y - 1;
	if (changed_rect.x < 0)
		start_j = -1;
	else
		start_j = changed_rect.x - 1;
	to_j = changed_rect.w + changed_rect.x;
	if (to_j > WINDOW_W)
		to_j = WINDOW_W;
	while (++i < to_i)
	{
		j = start_j;
		while (++j < to_j)
		{
			pixel = get_pixel(iw->t_weap[iw->guns.t],
				(j - changed_rect.x) * iw->t_weap[iw->guns.t]->w / changed_rect.w,
				(i - changed_rect.y) * iw->t_weap[iw->guns.t]->h / changed_rect.h);
			if (pixel != 0x010000)
				set_pixel(iw->sur, j, i, get_light_color(pixel, iw->sectors[iw->d.cs].light));
		}
	}
}

//BACKPACK/////////////////////////////////////////////////////////
void	draw_icon_bag(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.x = 0;
	rect.y = WINDOW_H - 100;
	rect.w = 100;
	rect.h = 100;
	if (iw->bag.selected_item != 0)
		ft_scaled_blit(iw->t_pickup[iw->bag.selected_item->t_numb], iw->sur, &rect);
	else
		ft_scaled_blit(iw->t_weap[17], iw->sur, &rect);

}


void	draw_frame(t_sdl *iw, SDL_Surface *winsur, SDL_Rect *rect)
{
	int i;
	int j;
	int color;
	i = -1;
	while(++i < rect->w)
	{
		j = -1;
		while(++j < rect->h)
		{
			color = get_pixel(iw->bag.button[0], iw->bag.button[0]->w * i / rect->w, iw->bag.button[0]->h * j / rect->h);
            if (color != 0x010000)
                set_pixel(winsur, rect->x + i, rect->y + j, color);
		}
	}
}

void	draw_item(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;
    int     color;

	i = -1;
	while (++i < rect->w)
	{
		j = -1;
		while (++j < rect->h)
        {
            color = get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h);
            if (color != 0x010000)
                set_pixel(winsur, rect->x + j, rect->y + i, color);
        }
	}
	
}

void	drop_item(t_sdl *iw)
{
	// int x;
	// int y;
	int i;
	int flag = 0;

	// x = iw->p.x;
	// y = iw->p.y;
	i = 0;
	while(i < iw->bag.count_items)
	{
		if (iw->bag.selected_item == iw->bag.item_in_bag1[i])
			flag = 1;
		if (flag == 1)
			iw->bag.item_in_bag1[i] = iw->bag.item_in_bag1[i + 1];
		i++;
	}
	iw->bag.count_items--;
	iw->bag.selected_item->x = iw->p.x;
	iw->bag.selected_item->y = iw->p.y;
	iw->bag.selected_item->z = get_floor_z(iw, iw->p.x, iw->p.y);
	iw->bag.selected_item->num_sec = iw->d.cs;
	iw->bag.selected_item->taken = 0;
	//add_sprite(iw, x, y, 500, iw->bag.selected_item, iw->d.cs, 1, 0.1f);	
	iw->bag.selected_item = 0;
}

//////////////////// NEED TO DELETE SPRITE AND DO SOME...
void	use_item(t_sdl *iw)
{
	int i;
	int flag;

	flag = 0;
	i = 0;
	while(i < iw->bag.count_items)
	{
		if (iw->bag.selected_item == iw->bag.item_in_bag1[i])
			flag = 1;
		if (flag == 1)
			iw->bag.item_in_bag1[i] = iw->bag.item_in_bag1[i + 1];
		i++;
	}
	iw->bag.count_items--;
	iw->bag.selected_item = 0;
}

void	add_item(t_sdl *iw)
{
	if (iw->v.look_sprite != 0 && iw->v.look_sprite->taken == 0
		&& iw->v.look_sprite->type == 1 && iw->v.look_sprite->plen < BUTTON_PRESS_DIST)
	{
		iw->bag.item_in_bag1[iw->bag.count_items] = iw->v.look_sprite;
		iw->bag.count_items++;
		iw->v.look_sprite->taken = 1;

	}
}

void draw_button(t_sdl	*iw, SDL_Rect	*rect, t_packaging_texture *texture)
{
	int x;
	int y;
	int color;

	x = -1;
	while(++x < rect->w)
	{
		y = -1;
		while(++y < rect->h)
		{
			color = get_pixel(texture, texture->w * x / rect->w, texture->h * y / rect->h);
            if (color != 0xff00e3 && color != 0x000000)
                set_pixel(iw->sur, rect->x + x, rect->y + y, color);
		}
	}
}

void draw_butttons(t_sdl	*iw, SDL_Rect	*rect)
{
	rect->w = ((((WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5))) / 3) <
	((WINDOW_H - (WINDOW_H / 5)) - (WINDOW_H - (WINDOW_H / 2.5)))) ?
	(((WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5))) / 3) :
	((WINDOW_H - (WINDOW_H / 5)) - (WINDOW_H - (WINDOW_H / 2.5)));
	
	rect->h = rect->w;
	
	// rect->w = ((WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5))) / 3;
	// rect->h = (WINDOW_H - (WINDOW_H / 5)) - (WINDOW_H - (WINDOW_H / 2.5));
	rect->x = (WINDOW_W - (WINDOW_W / 2.5));
	rect->y = (WINDOW_H - (WINDOW_H / 2.6));
	draw_button(iw, rect, iw->bag.button[2]);
	if (iw->bag.click_x >= rect->x && iw->bag.click_x <= rect->x + rect->w && iw->bag.click_y >= rect->y && iw->bag.click_y <= rect->y + rect->h && iw->v.mouse_mode == 0)
		use_item(iw);
	rect->x = WINDOW_W - (WINDOW_W / 2.5) + rect->w * 2;
	draw_button(iw, rect, iw->bag.button[1]);
	if (iw->bag.click_x >= rect->x && iw->bag.click_x <= rect->x + rect->w && iw->bag.click_y >= rect->y && iw->bag.click_y <= rect->y + rect->h && iw->v.mouse_mode == 0)
		drop_item(iw);
}

void	draw_selected_item(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect, t_sdl *iw)
{
	int		i;
	int		j;
    int     color;

	i = -1;
	rect->w = (WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5));
	rect->h = (WINDOW_H - (WINDOW_H / 4.5)) - (WINDOW_H / 2.5);
	while (++i < (WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5)) - 1)
	{
		j = -1;
		while (++j < (WINDOW_H - (WINDOW_H / 4.5)) - (WINDOW_H / 2.5) - 1)
        {
			color = get_pixel(tex, tex->w * i / rect->w, tex->h * j / rect->h);
            if (color != 0xff00e3 && color != 0x010000)
                set_pixel(winsur, WINDOW_W - (WINDOW_W / 2.5) + i, rect->y + j, color);
        }
	}
	draw_butttons(iw, rect);
}

void	draw_items(t_sdl *iw)
{
	int		i;
	int w_transparency;
	SDL_Rect	rect;

	w_transparency = (WINDOW_W - (WINDOW_W / 2.5)) - (WINDOW_W / 4.5);
	rect.w = w_transparency * 0.15;
	rect.h = w_transparency * 0.15;
	iw->bag.num_item_in_line = w_transparency / rect.w - 1;
	iw->bag.indent = (w_transparency - (iw->bag.num_item_in_line * rect.w)) / iw->bag.num_item_in_line;
	(iw->bag.indent < 1) ? iw->bag.indent++ : 0;
	rect.x = (WINDOW_W / 4.5);
	rect.y = (WINDOW_H / 4.5);
	i = -1;
	while (++i < iw->bag.count_items && rect.x <= WINDOW_W - (WINDOW_W / 2.5))
	{
		if (rect.x + iw->bag.indent + rect.w > WINDOW_W - (WINDOW_W / 2.5))
		{
			rect.y += rect.h + iw->bag.indent;
			rect.x = (WINDOW_W / 4.5);
			if (rect.y + rect.h >= WINDOW_H - (WINDOW_H / 4.5))
				break ;
		}
		draw_item(iw->t_pickup[iw->bag.item_in_bag1[i]->t_numb], iw->sur, &rect);
		if (iw->bag.click_x >= rect.x && iw->bag.click_x <= rect.x + rect.w &&
			iw->bag.click_y >= rect.y && iw->bag.click_y <= rect.y + rect.h &&
			iw->v.mouse_mode == 0)
		{
			iw->bag.selected_item = iw->bag.item_in_bag1[i];
			draw_frame(iw, iw->sur, &rect);
		}
		rect.x += rect.w + iw->bag.indent;
	}
	rect.y = (WINDOW_H / 4.5);
	if (iw->bag.selected_item != 0)
		draw_selected_item(iw->t_pickup[iw->bag.selected_item->t_numb], iw->sur, &rect, iw);
}

void	transparency(t_sdl	*iw)
{
	int i;
	int j;
	Uint32 pix;
	int color;

	i = WINDOW_W / 5 - 10;
	while(i++ <= WINDOW_W - (WINDOW_W / 5) + 9)
	{
		j = WINDOW_H / 5 - 10;
		while (j++ <= WINDOW_H - (WINDOW_H / 5) + 9)
		{
			if ((i >= WINDOW_W / 5 - 10 && i <= WINDOW_W / 5) || (j >= WINDOW_H / 5 - 10 && j <= WINDOW_H / 5) || 
				(i <= WINDOW_W - (WINDOW_W / 5) + 10 && i >=  WINDOW_W - (WINDOW_W / 5)) || 
				(j <= WINDOW_H - (WINDOW_H / 5) + 10 && j >=  WINDOW_H - (WINDOW_H / 5)))
				set_pixel(iw->sur, i, j, 0x999999);
			else
			{
				pix = get_pixel_surface(iw->sur, i, j);
				color = (((int)((float)(pix >> 16) * 0.3)) << 16) +
						(((int)((float)((pix >> 8) - (pix >> 16 << 8)) * 0.3)) << 8) +
						(int)((float)(pix - (pix >> 8 << 8)) * 0.3);
				set_pixel(iw->sur, i, j, color);
			}
		}
	}
    draw_items(iw);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	update(t_sdl *iw)
{
	SDL_FillRect(iw->sur, &iw->winrect, 0x000000);
	draw(iw);
	
	draw_some_info(iw);
	if (iw->v.game_mode)
	{
		make_health(&iw->hud, iw);
	}
	draw_crosshair(iw);
	if (iw->bag.bag == 1)
		transparency(iw);
	
	draw_icon_bag(iw);
	draw_selected_tex(iw);
	draw_selected_sprite(iw);
	SDL_UpdateWindowSurface(iw->win);
	//printf("Update\n");
	//printf("update ret %d\n", ret);
	/*if (iw->v.look_sprite == 0)
		printf("SPRITE NOT SELECTED\n");
	else
		printf("SPRITE SELECTED\n");*/
}

int		get_picture_dist(t_sdl *iw, t_picture *pic)
{
	return ((int)sqrtf(powf((pic->x0 + pic->x1) / 2 - iw->p.x, 2.0f) +
		powf((pic->y0 + pic->y1) / 2 - iw->p.y, 2.0f) +
		powf((pic->zu + pic->zd) / 2 - iw->p.z, 2.0f)));
}

void	button_f_up(t_sdl *iw)
{
	if (iw->v.f_button_mode == 1 && *(iw->v.look_sector) != 0)
		(*(iw->v.look_sector))->light = (t_picture *)iw->v.f_button_pointer;
	else if (iw->v.f_button_mode == 2 && *(iw->v.look_sector) != 0)
	{
		iw->v.submenu_mode = 1;
		add_sector_animation(iw);
		iw->v.f_button_mode = 0;
		iw->v.f_button_pointer = 0;
		draw_submenu(iw);
	}
	else if (iw->v.f_button_mode == 3 && *(iw->v.look_wall) != 0)
		add_wall_to_wall_animation(iw);
	else if (iw->v.f_button_mode == 0 && *(iw->v.look_picture) != 0 && (*(iw->v.look_picture))->t == 17
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST)
		(*(iw->v.look_picture))->t = 18;
	else if (iw->v.f_button_mode == 0 && *(iw->v.look_picture) != 0 && (*(iw->v.look_picture))->t == 18
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST)
		(*(iw->v.look_picture))->t = 17;
	else if (iw->v.f_button_mode == 0 && *(iw->v.look_picture) != 0
		&& ((*(iw->v.look_picture))->t == 19 || (*(iw->v.look_picture))->t == 20)
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST)
	{
		(*(iw->v.look_picture))->t = (((*(iw->v.look_picture))->t == 19) ? 20 : 19);
		change_sector_animation_status(iw, *(iw->v.look_picture));
		change_wall_animation_status(iw, *(iw->v.look_picture));
	}
}

void	key_up(int code, t_sdl *iw)
{
	printf("keyup = %d\n", code);
	if (code == 41)
		exit_x(iw);
	else if (code == 79)
		iw->v.rot_right = 1;
	else if (code == 80)
		iw->v.rot_left = 1;
	else if (code == 26)
		iw->v.front = 1;
	else if (code == 22)
		iw->v.back = 1;
	else if (code == 4)
		iw->v.left = 1;
	else if (code == 7)
		iw->v.right = 1;
	else if (code == 82)
		iw->v.rot_up = 1;
	else if (code == 81)
		iw->v.rot_down = 1;
	else if (code == 47)
		iw->v.picture_changing = 0;
	else if (code == 19 && iw->v.mouse_mode == 1 &&
		*(iw->v.look_wall) != 0 && !iw->v.game_mode)
		add_picture(iw, *(iw->v.look_wall));
	else if (code == 9) // F
		button_f_up(iw);
	else if (code == 10) // G
	{
		if (iw->v.f_button_mode == 3 && iw->v.wall_anim != 0)
		{
			iw->v.submenu_mode = 5;
			draw_submenu(iw);
		}
		iw->v.f_button_mode = 0;
		iw->v.f_button_pointer = 0;
	}
	else if (code == 48)
		iw->v.sprite_editing = 0;
	else if (code == 25 && !iw->v.game_mode)
	{
		iw->v.fly_mode = ((iw->v.fly_mode == 2) ? 0 : iw->v.fly_mode + 1);
		iw->v.fall = 1;
		iw->v.fly_down = 1;
		iw->v.fly_up = 1;
	}
	else if (code == 44 && iw->v.fly_mode)
		iw->v.fly_up = 1;
	else if (code == 224 && iw->v.fly_mode)
		iw->v.fly_down = 1;
	else if (code == 6 && *iw->v.look_wall != 0 &&
		(*iw->v.look_wall)->t >= 0 && (*iw->v.look_wall)->t < TEXTURES_COUNT)
		iw->v.tex_to_fill = (*iw->v.look_wall)->t;

	else if (code == 32 && iw->guns.status == 0 && iw->guns.gun_in_hands != 0)
	{
		iw->guns.status = 3;
		iw->guns.gun_in_hands = 0;
		iw->hud.shell = 100 * iw->guns.bullets[0] / iw->guns.max_bullets[0];
	}
	else if (code == 31 && iw->guns.status == 0 && iw->guns.gun_in_hands != 2 && iw->guns.bullets[2] > 0)
	{
		iw->guns.status = 3;
		iw->guns.gun_in_hands = 2;
		iw->hud.shell = 100 * iw->guns.bullets[2] / iw->guns.max_bullets[2];
	}
	else if (code == 30 && iw->guns.status == 0 && iw->guns.gun_in_hands != 1 && iw->guns.bullets[1] > 0)
	{
		iw->guns.status = 3;
		iw->guns.gun_in_hands = 1;
		iw->hud.shell = 100 * iw->guns.bullets[1] / iw->guns.max_bullets[1];
	}
	// 	iw->v.edit_mode = (iw->v.edit_mode == 0) ? 1 : 0;
	printf("rot = %d px %d py %d pz %d rotup %d\n", iw->p.introt, iw->p.x, iw->p.y, iw->p.z, iw->p.rotup);
	//if (code == 8)
	//	iw->guns.t++;
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
	else if (code == 47 && *(iw->v.look_picture) != 0 &&
		*(iw->v.look_wall) != 0 && !iw->v.game_mode)
	{
		iw->v.picture_changing = *(iw->v.look_picture);
		iw->v.wall_picture_changing = *(iw->v.look_wall);
	}
	else if (code == 18)
	{
		iw->v.kernel = ((iw->v.kernel == 1) ? 0 : 1);
		if (iw->v.kernel)
			printf("OpenCL ON\n");
		else
			printf("OpenCL OFF\n");
	}
	else if (code == 43 && !iw->v.game_mode)
	{
		iw->v.mouse_mode = ((iw->v.mouse_mode == 1) ? 0 : 1);
		SDL_SetRelativeMouseMode(iw->v.mouse_mode);
	}
	else if (code == 55 && *(iw->v.look_sector) != 0 && iw->v.mouse_mode == 1 && !iw->v.game_mode)
		(*(iw->v.look_sector))->cl.t = iw->v.tex_to_fill;
	else if (code == 56 && *(iw->v.look_sector) != 0 && iw->v.mouse_mode == 1 && !iw->v.game_mode)
		(*(iw->v.look_sector))->fr.t = iw->v.tex_to_fill;
	else if (code == 54 && *(iw->v.look_sector) != 0 && iw->v.mouse_mode == 1 && !iw->v.game_mode)
		(*(iw->v.look_sector))->cl.t = -1;
	else if (code == 16 && iw->v.mouse_mode == 1 && *(iw->v.look_wall) != 0
		&& (*iw->v.look_wall)->nextsector == -1 && !iw->v.game_mode)
		(*(iw->v.look_wall))->t = -1;
	else if (code == 48 && iw->v.mouse_mode == 1)
		iw->v.sprite_editing = 1;
	else if (code == 44 && iw->v.fly_mode)
		iw->v.fly_up = clock();
	else if (code == 224 && iw->v.fly_mode)
		iw->v.fly_down = clock();
	else if (code == 21 && iw->v.game_mode && iw->guns.status == 0
		&& iw->guns.bullets[iw->guns.gun_in_hands] < iw->guns.max_bullets[iw->guns.gun_in_hands])
		iw->guns.status = 2;
	else if (code == 5 && iw->v.game_mode)
	{
		SDL_SetRelativeMouseMode(iw->bag.bag);
		iw->v.mouse_mode = iw->bag.bag;
		iw->bag.bag = ((iw->bag.bag == 1) ? 0 : 1);
	}
	else if (code == 9 && iw->v.game_mode)
		add_item(iw);
	else if (code == 8 && iw->v.game_mode)
		use_item(iw);
	/*else if (code == 9)
		check_animations(iw);*/
	printf("rot = %d px %d py %d pz %d rotup %d\n", iw->p.introt, iw->p.x, iw->p.y, iw->p.z, iw->p.rotup);
}

void	key_down_repeat(int code, t_sdl *iw)
{
	if (code == 44 && iw->v.jump_time == 1 && iw->v.fall == 1 && !iw->v.fly_mode)
	{
		iw->v.jump_time = clock();
		iw->v.jump = JUMP_HEIGHT;
	}
}

void	mouse_move(int xrel, int yrel, t_sdl *iw)
{
	int		len;

	if (iw->v.mouse_mode == 0)
		return;
	//printf("xrel %d yrel %d\n", xrel, yrel);
	if (iw->v.picture_changing == 0)
	{
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
	else
	{
		iw->v.picture_changing->left_plus += xrel;
		if (iw->v.picture_changing->left_plus < 0)
			iw->v.picture_changing->left_plus = 0;
		else if (iw->v.picture_changing->left_plus + iw->v.picture_changing->tw >
			(len = (int)sqrtf(powf(iw->v.wall_picture_changing->next->x - iw->v.wall_picture_changing->x, 2.0f)
				+ powf(iw->v.wall_picture_changing->next->y - iw->v.wall_picture_changing->y, 2.0f))))
			iw->v.picture_changing->left_plus = len - iw->v.picture_changing->tw;
		iw->v.picture_changing->zu -= yrel;
		calculate_picture(iw, iw->v.wall_picture_changing, iw->v.picture_changing);
	}
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

	iw->v.left_mouse_pressed = 0;
	if (iw->v.game_mode)
	{

	}
	else if (y > WINDOW_H && y < WINDOW_H + 100 && iw->v.mouse_mode == 0)
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
		else if (x > 332 && x < 430)
		{
			if (y < WINDOW_H + 135)
				draw_decor_tex_to_select(iw);
			else if (y < WINDOW_H + 170)
				draw_pickup_tex_to_select(iw);
			else
				draw_enemies_tex_to_select(iw);
		}
		else if (iw->v.submenu_mode == 1 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 430)
			{
				iw->v.sector_anim->type = 1;
				iw->v.submenu_mode = 2;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 395)
			{
				iw->v.sector_anim->type = 2;
				iw->v.submenu_mode = 2;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 355)
			{
				iw->v.sector_anim->type = 0;
				iw->v.submenu_mode = 2;
				draw_submenu(iw);
			}
			else if (x > WINDOW_W - 335 && x < WINDOW_W - 280)
				exit_editing_sector_animation(iw);
		}
		else if (iw->v.submenu_mode == 2 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 390)
			{
				do_sector_animation_step(iw, iw->v.sector_anim, 100);
				iw->v.sector_anim->dy += 100;
			}
			else if (x < WINDOW_W - 330)
			{
				do_sector_animation_step(iw, iw->v.sector_anim, -100);
				iw->v.sector_anim->dy -= 100;
			}
			else if (x < WINDOW_W - 280)
			{
				iw->v.submenu_mode = 3;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 220)
				exit_editing_sector_animation(iw);
		}
		else if (iw->v.submenu_mode == 3 && x > WINDOW_W - 340)
		{
			if (x < WINDOW_W - 320)
			{
				iw->v.sector_anim->speed += ((iw->v.sector_anim->speed < 9) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 295)
			{
				iw->v.sector_anim->speed -= ((iw->v.sector_anim->speed > 1) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 245)
			{
				do_sector_animation_step(iw, iw->v.sector_anim, -iw->v.sector_anim->dy);
				iw->v.sector_anim->next = iw->sector_animations;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
				iw->v.sector_anim->prev_clock = clock();
				iw->sector_animations = iw->v.sector_anim;
				iw->v.sector_anim = 0;
			}
			else if (x < WINDOW_W - 200)
				exit_editing_sector_animation(iw);
		}
		else if (iw->v.submenu_mode == 4 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 375)
			{
				iw->v.f_button_mode = 2;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 300)
			{
				iw->v.f_button_mode = 3;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 240)
			{
				iw->v.submenu_mode = 0;
				iw->v.f_button_pointer = 0;
				draw_submenu(iw);
			}
		}
		else if (iw->v.submenu_mode == 5 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 380)
			{
				iw->v.wall_anim->moving_type = 0;
				iw->v.submenu_mode = 6;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 305)
			{
				iw->v.wall_anim->moving_type = 1;
				iw->v.submenu_mode = 6;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 225)
			{
				iw->v.wall_anim->moving_type = 2;
				iw->v.submenu_mode = 6;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 165)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 6 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 380)
			{
				iw->v.wall_anim->priority = 0;
				iw->v.submenu_mode = 7;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 300)
			{
				iw->v.wall_anim->priority = 1;
				iw->v.submenu_mode = 7;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 225)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 7 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 390)
			{
				iw->v.wall_anim->dx += 100;
				do_wall_animation_step_dx(iw, iw->v.wall_anim, 100);
			}
			else if (x < WINDOW_W - 330)
			{
				iw->v.wall_anim->dx -= 100;
				do_wall_animation_step_dx(iw, iw->v.wall_anim, -100);
			}
			else if (x < WINDOW_W - 280)
			{
				iw->v.submenu_mode = 8;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 220)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 8 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 390)
			{
				iw->v.wall_anim->dy += 100;
				do_wall_animation_step_dy(iw, iw->v.wall_anim, 100);
			}
			else if (x < WINDOW_W - 330)
			{
				iw->v.wall_anim->dy -= 100;
				do_wall_animation_step_dy(iw, iw->v.wall_anim, -100);
			}
			else if (x < WINDOW_W - 280)
			{
				iw->v.submenu_mode = 9;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 220)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 9 && x > WINDOW_W - 340)
		{
			if (x < WINDOW_W - 320)
			{
				iw->v.wall_anim->speed += ((iw->v.wall_anim->speed < 9) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 295)
			{
				iw->v.wall_anim->speed -= ((iw->v.wall_anim->speed > 1) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 245)
			{
				do_wall_animation_step_dx(iw, iw->v.wall_anim, -iw->v.wall_anim->dx);
				do_wall_animation_step_dy(iw, iw->v.wall_anim, -iw->v.wall_anim->dy);
				iw->v.wall_anim->next = iw->wall_animations;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
				iw->v.wall_anim->prev_clock = clock();
				iw->wall_animations = iw->v.wall_anim;
				iw->v.wall_anim = 0;
			}
			else if (x < WINDOW_W - 200)
				exit_editing_wall_animation(iw);
		}
	}
	else if (y > WINDOW_H + 200 && y < WINDOW_H + 300 && iw->v.mouse_mode == 0)
	{
		if (iw->v.sprites_select_mode == 0)
		{
			i = x / 100 + iw->v.scroll_decor_sprites;
			if (i < DECOR_TEXTURES_COUNT)
			{
				iw->v.selected_sprite_type = 0;
				iw->v.selected_sprite = i;
			}
		}
		else if (iw->v.sprites_select_mode == 1)
		{
			i = x / 100 + iw->v.scroll_pickup_sprites;
			if (i < PICK_UP_TEXTURES_COUNT)
			{
				iw->v.selected_sprite_type = 1;
				iw->v.selected_sprite = i;
			}
		}
		else if (iw->v.sprites_select_mode == 2)
		{
			i = x / 100;
			if (i < COUNT_ENEMIES)
			{
				iw->v.selected_sprite_type = 2;
				iw->v.selected_sprite = i;
			}
		}
	}
	else if (iw->v.sprite_editing && iw->v.mouse_mode == 1 && iw->v.look_sprite != 0)
	{
		if (iw->v.selected_sprite_type == 0)
		{
			iw->v.look_sprite->type = 0;
			iw->v.look_sprite->t = iw->t_decor[iw->v.selected_sprite];
			iw->v.look_sprite->t_kernel = &iw->k.m_t_decor[iw->v.selected_sprite];
		}
		else if (iw->v.selected_sprite_type == 1)
		{
			iw->v.look_sprite->type = 1;
			iw->v.look_sprite->t = iw->t_pickup[iw->v.selected_sprite];
			iw->v.look_sprite->t_kernel = &iw->k.m_t_pickup[iw->v.selected_sprite];
		}
		else if (iw->v.selected_sprite_type == 2)
		{
			iw->v.look_sprite->type = 2;
			iw->v.look_sprite->e.enemy_numb = iw->v.selected_sprite;
			iw->v.look_sprite->e.status = 0;
			if (iw->v.selected_sprite == 0)
			{
				iw->v.look_sprite->e.health = 10;
				iw->v.look_sprite->e.damage = 5;
				iw->v.look_sprite->t = iw->t_enemies[0];
				iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[0];
			}
			else if (iw->v.selected_sprite == 1)
			{
				iw->v.look_sprite->e.health = 20;
				iw->v.look_sprite->e.damage = 8;
				iw->v.look_sprite->t = iw->t_enemies[8];
				iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[8];
			}
		}
	}
	else if (iw->v.mouse_mode == 1 && *(iw->v.look_picture) != 0 && *(iw->v.look_wall) != 0)
	{
		(*(iw->v.look_picture))->t = iw->v.tex_to_fill;
		calculate_picture(iw, *(iw->v.look_wall), *(iw->v.look_picture));
	}
	else if (iw->v.mouse_mode == 1 && *(iw->v.look_wall) != 0)
	{
		if (iw->v.look_portal == 0 || iw->v.look_portal->glass < 0)
			(*(iw->v.look_wall))->t = iw->v.tex_to_fill;
		else
			iw->v.look_portal->glass = -1;
	}
}

void	mouse_buttonright_up(int x, int y, t_sdl *iw)
{
	if (iw->v.game_mode)
	{

	}
	else if (iw->v.mouse_mode == 1 && iw->v.look_portal != 0)
		iw->v.look_portal->glass = iw->v.tex_to_fill;
	else if (iw->v.mouse_mode == 1 && *(iw->v.look_picture) != 0 && *(iw->v.look_wall) != 0)
		delete_picture(*(iw->v.look_wall), *(iw->v.look_picture), iw);
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
	else if (iw->v.mouse_mode == 1 && iw->v.picture_changing != 0)
	{
		if (e->wheel.y < 0 && iw->v.picture_changing->tw - 30 > 50)
			iw->v.picture_changing->tw -= 30;
		else if (e->wheel.y > 0)
			iw->v.picture_changing->tw += 30;
		calculate_picture(iw, iw->v.wall_picture_changing, iw->v.picture_changing);
	}
	else if (iw->v.mouse_mode == 1 && iw->v.sprite_editing && !iw->v.game_mode && iw->v.look_sprite != 0)
	{
		if (e->wheel.y < 0)
			iw->v.look_sprite->scale *= 1.1f;
		else if (iw->v.look_sprite->scale > 0.2f)
			iw->v.look_sprite->scale /= 1.1f;
	}
}

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
	if (nszu - nszd >= PLAYER_HEIGHT + PLAYER_HEAD_SIZE
		&& (nszd < iw->p.z || nszd - iw->p.z + PLAYER_HEIGHT < MAX_CLIMB_HEIGHT)
		&& (nszu >= iw->p.z + PLAYER_HEAD_SIZE)
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
	speed = MOVING_SPEED_PER_HALF_SEC * (float)(clock() - *time) / (float)CLKS_P_S;
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

void	get_wall_line2(t_wall *wall)
{
	wall->l.a = (float)(wall->next->y - wall->y);
	wall->l.b = (float)(wall->x - wall->next->x);
	wall->l.c = (float)(wall->next->x * wall->y -
		wall->x * wall->next->y);
}

void	do_sector_animations(t_sdl *iw)
{
	t_sector_animation	*tmp;

	tmp = iw->sector_animations;
	while (tmp)
	{
		if (clock() - tmp->prev_clock > CLKS_P_S / 50)
		{
			if (tmp->status == 1 && abs(tmp->curr_dy) < abs(tmp->dy))//tmp->curr_dy != tmp->dy)
			{
				do_sector_animation_step(iw, tmp, ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10);
				tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
			}
			else if (tmp->status == 0 && tmp->curr_dy != 0)
			{
				do_sector_animation_step(iw, tmp, ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10);
				tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
			}
			tmp->prev_clock = clock();
		}
		tmp = tmp->next;
	}
}

void	do_wall_animations(t_sdl *iw)
{
	t_wall_animation	*tmp;

	tmp = iw->wall_animations;
	while (tmp)
	{
		if (clock() - tmp->prev_clock > CLKS_P_S / 50)
		{
			if (tmp->status == 1)
			{
				if (tmp->priority == 0)
				{
					if (abs(tmp->curr_dx) < abs(tmp->dx))
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (abs(tmp->curr_dy) < abs(tmp->dy))
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
				else
				{
					if (abs(tmp->curr_dy) < abs(tmp->dy))
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (abs(tmp->curr_dx) < abs(tmp->dx))
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
			}
			else
			{
				if (tmp->priority == 0)
				{
					if (tmp->curr_dy != 0)
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (tmp->curr_dx != 0)
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
				else
				{
					if (tmp->curr_dx != 0)
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (tmp->curr_dy != 0)
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
			}

			tmp->prev_clock = clock();
		}
		tmp = tmp->next;
	}
}

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

// ENEMIES FUNCTIONS /////////////////////////////////////////////////////////////

int		esp_check_walls(t_sdl *iw, t_enemy_sees_player *esp)
{
	int		wall;

	wall = iw->sectors[esp->curr_sector].sw - 1;
	while (++wall < iw->sectors[esp->curr_sector].sw + iw->sectors[esp->curr_sector].nw)
	{
		if (iw->walls[wall].nextsector != -1)
			continue;
		esp->k1 = iw->walls[wall].l.a * (float)esp->px + iw->walls[wall].l.b * (float)esp->py + iw->walls[wall].l.c;
		esp->k2 = iw->walls[wall].l.a * (float)esp->ex + iw->walls[wall].l.b * (float)esp->ey + iw->walls[wall].l.c;
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
		{
			esp->k1 = esp->a * (float)iw->walls[wall].x + esp->b * (float)iw->walls[wall].y + esp->c;
			esp->k2 = esp->a * (float)iw->walls[wall].next->x + esp->b * (float)iw->walls[wall].next->y + esp->c;
			if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
				return (0);
		}
	}
	return (1);
}

int		esp_check_portal(t_sdl *iw, t_enemy_sees_player *esp, int portal)
{
	esp->k1 = iw->walls[portal].l.a * (float)esp->px + iw->walls[portal].l.b * (float)esp->py + iw->walls[portal].l.c;
	esp->k2 = iw->walls[portal].l.a * (float)esp->ex + iw->walls[portal].l.b * (float)esp->ey + iw->walls[portal].l.c;
	if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
	{
		esp->k1 = esp->a * (float)iw->walls[portal].x + esp->b * (float)iw->walls[portal].y + esp->c;
		esp->k2 = esp->a * (float)iw->walls[portal].next->x + esp->b * (float)iw->walls[portal].next->y + esp->c;
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
			return (1);
	}
	return (0);
}

void	esp_get_new_player_coordinates(t_sdl *iw, t_sector_way *way, t_enemy_sees_player *esp, t_sprite *s)
{
	esp->px = iw->p.x;
	esp->py = iw->p.y;
	esp->ex = s->x;
	esp->ey = s->y;
	esp->curr_sector = s->num_sec;
	while (way)
	{
		esp->px -= iw->walls[iw->walls[way->portal].nextsector_wall].x - iw->walls[way->portal].next->x;
		esp->py -= iw->walls[iw->walls[way->portal].nextsector_wall].y - iw->walls[way->portal].next->y;
		way = way->next;
	}
}

int		enemy_sees_player(t_sdl *iw, t_sprite *s)
{
	t_enemy_sees_player	esp;
	t_sector_ways		*ways;
	t_sector_way		*way;

	esp.px = iw->p.x;
	esp.py = iw->p.y;
	esp.ex = s->x;
	esp.ey = s->y;
	esp.curr_sector = s->num_sec;
	if (iw->d.cs == s->num_sec)
	{
		esp.a = (float)(esp.py - esp.ey);
		esp.b = (float)(esp.ex - esp.px);
		esp.c = (float)(esp.px * esp.ey - esp.ex * esp.py);
		if (!esp_check_walls(iw, &esp))
			return (-1);
		s->e.vis_esp = esp;
		return ((int)sqrtf(powf(esp.px - esp.ex, 2.0f) + powf(esp.py - esp.ey, 2.0f)));
	}
	// if (!iw->ways[s->num_sec][iw->d.cs])
	// 	return (-1);
	ways = iw->ways[s->num_sec][iw->d.cs];
	while (ways)
	{
		way = ways->way_start;
		esp_get_new_player_coordinates(iw, way, &esp, s);
		while (way)
		{
			esp.a = (float)(esp.py - esp.ey);
			esp.b = (float)(esp.ex - esp.px);
			esp.c = (float)(esp.px * esp.ey - esp.ex * esp.py);
			if (!esp_check_walls(iw, &esp) || !esp_check_portal(iw, &esp, way->portal))
				break;
			esp.px += iw->walls[iw->walls[way->portal].nextsector_wall].x - iw->walls[way->portal].next->x;
			esp.py += iw->walls[iw->walls[way->portal].nextsector_wall].y - iw->walls[way->portal].next->y;
			esp.ex += iw->walls[iw->walls[way->portal].nextsector_wall].x - iw->walls[way->portal].next->x;
			esp.ey += iw->walls[iw->walls[way->portal].nextsector_wall].y - iw->walls[way->portal].next->y;
			esp.curr_sector = iw->walls[way->portal].nextsector;
			way = way->next;
		}
		if (!way)
		{
			esp.a = (float)(esp.py - esp.ey);
			esp.b = (float)(esp.ex - esp.px);
			esp.c = (float)(esp.px * esp.ey - esp.ex * esp.py);
			if (esp_check_walls(iw, &esp))
			{
				s->e.vis_esp = esp;
				return ((int)sqrtf(powf(esp.px - esp.ex, 2.0f) + powf(esp.py - esp.ey, 2.0f)));
			}
		}
		ways = ways->next;
	}
	return (-1);
}

int		move_enemy_in_portal(t_sdl *iw, t_sprite *s, t_intpoint2d *vect)
{
	int		wall;
	float	k1;
	float	k2;

	wall = iw->sectors[s->num_sec].sw - 1;
	while (++wall < iw->sectors[s->num_sec].sw + iw->sectors[s->num_sec].nw)
	{
		if (iw->walls[wall].nextsector == -1 || iw->walls[wall].glass != -1)
			continue;
		k1 = iw->walls[wall].l.a * s->x + iw->walls[wall].l.b * s->y + iw->walls[wall].l.c;
		k2 = iw->walls[wall].l.a * (s->x + vect->x) + iw->walls[wall].l.b * (s->y + vect->y) + iw->walls[wall].l.c;
		if ((k1 < 0.0f && k2 > 0.0f) || (k1 > 0.0f && k2 < 0.0f))
		{
			s->e.vis_esp.a = (float)vect->y;
			s->e.vis_esp.b = (float)(-vect->x);
			s->e.vis_esp.c = (float)((s->x + vect->x) * s->y - s->x * (s->y + vect->y));
			k1 = s->e.vis_esp.a * iw->walls[wall].x + s->e.vis_esp.b * iw->walls[wall].y + s->e.vis_esp.c;
			k2 = s->e.vis_esp.a * iw->walls[wall].next->x + s->e.vis_esp.b * iw->walls[wall].next->y + s->e.vis_esp.c;
			if ((k1 < 0.0f && k2 > 0.0f) || (k1 > 0.0f && k2 < 0.0f))
			{
				s->x += vect->x + iw->walls[iw->walls[wall].nextsector_wall].x - iw->walls[wall].next->x;
				s->y += vect->y + iw->walls[iw->walls[wall].nextsector_wall].y - iw->walls[wall].next->y;
				s->num_sec = iw->walls[wall].nextsector;
				s->e.vis_esp.ex += iw->walls[iw->walls[wall].nextsector_wall].x - iw->walls[wall].next->x;
				s->e.vis_esp.ey += iw->walls[iw->walls[wall].nextsector_wall].y - iw->walls[wall].next->y;
				s->e.vis_esp.px += iw->walls[iw->walls[wall].nextsector_wall].x - iw->walls[wall].next->x;
				s->e.vis_esp.py += iw->walls[iw->walls[wall].nextsector_wall].y - iw->walls[wall].next->y;
				return (1);
			}
		}
	}
	return (0);
}

int		move_enemy(t_sdl *iw, t_sprite *s)
{
	t_point2d		vect_norm;
	t_intpoint2d	vect;
	float			len;
	clock_t			t;

	vect_norm.x = s->e.vis_esp.px - s->e.vis_esp.ex;
	vect_norm.y = s->e.vis_esp.py - s->e.vis_esp.ey;
	len = sqrtf(powf(vect_norm.x, 2.0f) + powf(vect_norm.y, 2.0f));
	vect_norm.x /= len;
	vect_norm.y /= len;
	t = clock();
	vect.x = vect_norm.x * (float)(t - s->e.prev_update_time) / (float)CLKS_P_S * 1000.0f;
	vect.y = vect_norm.y * (float)(t - s->e.prev_update_time) / (float)CLKS_P_S * 1000.0f;
	if (in_sec_xy(iw, s->num_sec, s->x + vect.x, s->y + vect.y))
	{
		s->x += vect.x;
		s->y += vect.y;
		return (1);
	}
	return (move_enemy_in_portal(iw, s, &vect));
}

void	sprite_physics(t_sdl *iw, t_sprite *s)
{
	int		tmp;

	if (s->fall_time != 1 /*&& s->e.enemy_numb != 0*/)
		s->z -= (int)(iw->v.accel * ((float)(clock() - s->fall_time) /
			(float)CLKS_P_S) * 50.0f);
	tmp = get_ceil_z_sec(iw, s->x, s->y, s->num_sec);
	if (s->z + SPRITE_HEIGHT > tmp)
		s->z = tmp - SPRITE_HEIGHT;
	tmp = get_floor_z_sec(iw, s->x, s->y, s->num_sec);
	if (s->z < tmp)
	{
		s->z = tmp;
		s->fall_time = 1;
	}
	else
		s->fall_time = clock() - CLKS_P_S / 10;
}

void	enemy_intelligence0(t_sdl *iw, t_sprite *s)
{
	int		i;

	if (s->e.status == 0)
	{
		if (clock() - s->e.previous_picture_change > CLKS_P_S / 3)
		{
			s->e.previous_picture_change = clock();
			if (s->t_numb == 0)
				s->t_numb = 2;
			else
				s->t_numb = 0;
			s->t = iw->t_enemies[s->t_numb];
			s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
		}
		if ((i = enemy_sees_player(iw, s)) != -1)
		{
			if (i < 10000 && i > 800)
				move_enemy(iw, s);
			else if (i <= 800)
				s->e.status = 3;
		}
		else if (s->e.vis_esp.curr_sector != -1)
		{
			i = (int)sqrtf(powf(s->x - s->e.vis_esp.px, 2.0f) + powf(s->y - s->e.vis_esp.py, 2.0f));
			if (i > 10)
				move_enemy(iw, s);
		}
	}
	else if (s->e.status == 3 && clock() - s->e.previous_picture_change > CLKS_P_S / 5)
	{
		s->e.previous_picture_change = clock();
		if (s->t_numb != 3 && s->t_numb != 4)
			s->t_numb = 3;
		else if (s->t_numb == 3)
		{
			s->t_numb = 4;
			iw->p.health -= 3;
		}
		else
		{
			s->t_numb = 0;
			s->e.status = 0;
		}
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}
	sprite_physics(iw, s);
}

void	check_enemies(t_sdl *iw)
{
	t_sprite	*tmp;

	tmp = *iw->sprite;
	while (tmp)
	{
		if (tmp->type == 2)
		{
			if (tmp->e.enemy_numb == 0)
				enemy_intelligence0(iw, tmp);
			tmp->e.prev_update_time = clock();
		}
		tmp = tmp->next;
	}
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	attack(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0 && iw->guns.status == 0 &&
		clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		iw->guns.status = 1;
		iw->guns.t = 18;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[0] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[0] / iw->guns.max_bullets[0];
	}
	else if (iw->guns.gun_in_hands == 1 && iw->guns.status == 0 && clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		iw->guns.status = 1;
		iw->guns.t = 1;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[1] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[1] / iw->guns.max_bullets[1];
	}
	else if (iw->guns.gun_in_hands == 2 && iw->guns.status == 0 && clock() - iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 1;
		iw->guns.t = 8;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[2] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[2] / iw->guns.max_bullets[2];
	}
}

void	reload_gun(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0)
	{
		iw->guns.bullets[iw->guns.gun_in_hands] = iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
	}
	else if (iw->guns.gun_in_hands == 1)
	{
		if (iw->guns.bullets_in_stock[1] < iw->guns.max_bullets[1])
		{
			iw->guns.gun_in_hands = 0;
			iw->guns.status = 3;
		}
		else if (iw->guns.t == 0)
			iw->guns.t = 2;
		else if (iw->guns.t < 6)
			iw->guns.t++;
		else
		{
			iw->guns.bullets_in_stock[1] -= iw->guns.max_bullets[1] - iw->guns.bullets[1];;
			iw->guns.bullets[iw->guns.gun_in_hands] = iw->guns.max_bullets[iw->guns.gun_in_hands];
			iw->hud.shell = 100;
			iw->guns.status = 0;
			iw->guns.t = 0;
			iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
		}
	}
	else if (iw->guns.gun_in_hands == 2)
	{
		if (iw->guns.bullets_in_stock[2] < iw->guns.max_bullets[2])
		{
			iw->guns.gun_in_hands = 0;
			iw->guns.status = 3;
		}
		else if (iw->guns.t == 7)
			iw->guns.t = 10;
		else if (iw->guns.t < 16)
			iw->guns.t++;
		else
		{
			iw->guns.bullets_in_stock[2] -= iw->guns.max_bullets[2] - iw->guns.bullets[2];
			iw->guns.bullets[iw->guns.gun_in_hands] = iw->guns.max_bullets[iw->guns.gun_in_hands];
			iw->hud.shell = 100;
			iw->guns.status = 0;
			iw->guns.t = 7;
			iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
		}
	}
	iw->guns.prev_update_time = clock();
}

void	guns_loop(t_sdl *iw)
{
	if (iw->guns.status == 1 && iw->guns.gun_in_hands == 0 && clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		iw->guns.status = 0;
		iw->guns.t = 17;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 1 && clock() - iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 0;
		iw->guns.t = 0;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 2 && clock() - iw->guns.prev_update_time > CLKS_P_S / 10)
	{
		if (iw->guns.t == 8)
			iw->guns.t = 9;
		else
		{
			if (!iw->v.left_mouse_pressed ||
				iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			{
				iw->guns.status = 0;
				iw->guns.t = 7;
			}
			else
			{
				iw->guns.t = 8;
				iw->guns.bullets[iw->guns.gun_in_hands]--;
				iw->hud.shell = 100 * iw->guns.bullets[iw->guns.gun_in_hands] /
					iw->guns.max_bullets[iw->guns.gun_in_hands];
			}
			if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
				iw->guns.status = 2;
		}
		iw->guns.prev_update_time = clock();
	}
}

void	guns_movements(t_sdl *iw)
{
	int		t;

	if (iw->v.game_mode &&
		(iw->v.front != 1 || iw->v.back != 1 || iw->v.left != 1 || iw->v.right != 1))
	{
		printf("FPS %d\n", iw->v.fps);
		if (iw->v.fps != 0)
			t = 300 / iw->v.fps;
		else
			t = 10;
		iw->v.weapon_change_x += ((iw->v.weapon_change_xdir > 0) ? t : -t);
		if (abs(iw->v.weapon_change_x) > WEAPONS_MOVING_CHANGE_VALUE)
		{
			iw->v.weapon_change_xdir *= -1;
			iw->v.weapon_change_x = ((iw->v.weapon_change_x > 0) ? WEAPONS_MOVING_CHANGE_VALUE : -WEAPONS_MOVING_CHANGE_VALUE);
		}
		iw->v.weapon_change_y += ((iw->v.weapon_change_ydir > 0) ? t : -t);
		if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE || iw->v.weapon_change_y < 10)
			iw->v.weapon_change_ydir *= -1;
		if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE)
			iw->v.weapon_change_y = WEAPONS_MOVING_CHANGE_VALUE;
		if (iw->v.weapon_change_y < 10)
			iw->v.weapon_change_y = 10;
	}
	if (iw->guns.status == 3)
	{
		iw->v.weapon_change_y_hide += 500 / iw->v.fps;
		if (iw->v.weapon_change_y_hide >= iw->guns.t_rect[iw->guns.t].h)
		{
			if (iw->guns.gun_in_hands == 0)
				iw->guns.t = 17;
			else if (iw->guns.gun_in_hands == 1)
				iw->guns.t = 0;
			else if (iw->guns.gun_in_hands == 2)
				iw->guns.t = 7;
			iw->guns.status = 0;
		}
	}
	else if (iw->v.weapon_change_y_hide > 0)
	{
		iw->v.weapon_change_y_hide -= 1000 / iw->v.fps;
		if (iw->v.weapon_change_y_hide < 0)
			iw->v.weapon_change_y_hide = 0;
	}
}

void	loop(t_sdl *iw)
{
	int		t;
	float	jsz;

	if ((double)(clock() - iw->loop_update_time) < (double)CLKS_P_S / (double)MAX_FPS)
		return;
	if (iw->v.left_mouse_pressed)
		attack(iw);
	if (iw->guns.status != 0)
		guns_loop(iw);
	if (iw->guns.status == 2 && clock() - iw->guns.prev_update_time > CLKS_P_S / 8)
		reload_gun(iw);
	guns_movements(iw);
	if (iw->v.rot_right != 1)
	{
		iw->p.rot += (ROTATION_SPEED_PER_HALF_SEC * (double)(clock() - iw->v.rot_right)
			/ (float)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot >= G360)
			iw->p.rot -= G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_right = clock();
	}
	if (iw->v.rot_left != 1)
	{
		iw->p.rot -= (ROTATION_SPEED_PER_HALF_SEC * (double)(clock() - iw->v.rot_left)
			/ (float)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot < 0.0f)
			iw->p.rot += G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_left = clock();
	}
	if (iw->v.rot_up != 1 && iw->p.rotup < 2 * WINDOW_H)
	{
		iw->p.rotup += 2 * WINDOW_H * (clock() - iw->v.rot_up) / CLKS_P_S;
		iw->v.rot_up = clock();
	}
	if (iw->v.rot_down != 1 && iw->p.rotup > -2 * WINDOW_H)
	{
		iw->p.rotup -= 2 * WINDOW_H * (clock() - iw->v.rot_down) / CLKS_P_S;
		iw->v.rot_down = clock();
	}
	//do_animations(iw);
	if (iw->d.cs >= 0)
	{
		iw->v.plrzu = get_ceil_z(iw, iw->p.x, iw->p.y);
		iw->v.plrzd = get_floor_z(iw, iw->p.x, iw->p.y);
		if (iw->v.front != 1)
		{
			move(iw, 0, &iw->v.front);
			//iw->v.front = clock();
		}
		if (iw->v.back != 1)
		{
			move(iw, 180, &iw->v.back);
			//iw->v.back = clock();
		}
		if (iw->v.left != 1)
		{
			move(iw, 270, &iw->v.left);
			//iw->v.left = clock();
		}
		if (iw->v.right != 1)
		{
			move(iw, 90, &iw->v.right);
			//iw->v.right = clock();
		}
		if (iw->v.fall != 1 && !iw->v.fly_mode)
		{
			t = clock();
			iw->p.z -= (int)(iw->v.accel * ((double)(t - iw->v.fall) /
				(double)CLKS_P_S) * 50.0f);
		}
		else if (iw->v.jump_time != 1)
		{
			jsz = (float)(clock() - iw->v.jump_time) / (float)CLKS_P_S * (float)JUMP_HEIGHT *
				iw->v.accel / 10.0f;
			if ((int)jsz >= iw->v.jump)
			{
				iw->p.z += iw->v.jump;
				iw->v.jump_time = 1;
			}
			else
			{
				iw->p.z += (int)jsz;
				iw->v.jump -= (int)jsz;
			}

		}
		if (iw->v.fall == 1 && iw->v.jump_time == 1
			&& (iw->p.z - iw->v.plrzd) > PLAYER_HEIGHT && !iw->v.fly_mode)
			iw->v.fall = clock();
		if (iw->v.fly_up != 1)
		{
			iw->p.z += (int)(FLY_SPEED * (float)(clock() - iw->v.fly_up) / (float)CLKS_P_S);
			iw->v.fly_up = clock();
		}
		if (iw->v.fly_down != 1)
		{
			iw->p.z -= (int)(FLY_SPEED * (float)(clock() - iw->v.fly_down) / (float)CLKS_P_S);
			iw->v.fly_down = clock();
		}
		if (iw->sectors[iw->d.cs].cl.t >= 0 && iw->p.z + PLAYER_HEAD_SIZE > iw->v.plrzu && iw->v.fly_mode != 2)
			iw->p.z = iw->v.plrzu - PLAYER_HEAD_SIZE;
		else if (iw->p.z - iw->v.plrzd < PLAYER_HEIGHT && iw->v.fly_mode != 2)
		{
			iw->p.z = iw->v.plrzd + PLAYER_HEIGHT;
			iw->v.fall = 1;
		}
		
	}
	else if (iw->v.fly_mode == 2)
	{
		if (iw->v.front != 1)
			move(iw, 0, &iw->v.front);
		if (iw->v.back != 1)
			move(iw, 180, &iw->v.back);
		if (iw->v.left != 1)
			move(iw, 270, &iw->v.left);
		if (iw->v.right != 1)
			move(iw, 90, &iw->v.right);
	}
	else
		iw->v.fall = 1;
	do_sector_animations(iw);
	do_wall_animations(iw);
	if (iw->v.fly_mode != 2)
		check_walls_collisions(iw);
	if (iw->v.game_mode)
		check_enemies(iw);
	update(iw);
	iw->v.fps = (int)((float)CLKS_P_S / (float)(clock() - iw->loop_update_time));
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
			{
				mouse_buttonleft_up(e.button.x, e.button.y, iw);
				iw->bag.click_x = e.button.x;
        		iw->bag.click_y = e.button.y;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && iw->v.game_mode)
				iw->v.left_mouse_pressed = 1;
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

int		in_sec(int sector, t_sdl *iw)
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
		if (in_sec(sec, iw) != 0)
			return (sec);
	sec = -1;
	while (++sec < iw->v.ls)
		if (in_sec(sec, iw) != 0)
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

int		get_floor_z_sec(t_sdl *iw, int x, int y, int sector)
{
	if (iw->sectors[sector].fr.n == 0)
		return (iw->sectors[sector].fr.z);
	else
		return ((iw->sectors[sector].fr.n->a * x + iw->sectors[sector].fr.n->b *
			y + iw->sectors[sector].fr.n->d) / (-iw->sectors[sector].fr.n->c));
}

int		get_ceil_z_sec(t_sdl *iw, int x, int y, int sector)
{
	if (iw->sectors[sector].cl.n == 0)
		return (iw->sectors[sector].cl.z);
	else
		return ((iw->sectors[sector].cl.n->a * x + iw->sectors[sector].cl.n->b *
			y + iw->sectors[sector].cl.n->d) / (-iw->sectors[sector].cl.n->c));
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
			set_pixel(iw->sur, left->x + j, i, 
				get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx, (int)d.ty), iw->sectors[iw->visited_sectors->sec].light));
			d.ty += d.dty;
			while (d.ty >= (float)iw->t[left->wall->t]->h)
				d.ty -= (float)iw->t[left->wall->t]->h;
		}
		iw->d.bottom[left->x + j] = tmp[j];
		d.ang += d.dang;
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		while (d.tx > (float)iw->t[left->wall->t]->w)
			d.tx -= (float)iw->t[left->wall->t]->w;
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
			set_pixel(iw->sur, left->x + j, i,
				get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx, (int)d.ty), iw->sectors[iw->visited_sectors->sec].light));
			d.ty += d.dty;
			while (d.ty >= (float)iw->t[left->wall->t]->h)
				d.ty -= (float)iw->t[left->wall->t]->h;
		}
		iw->d.top[left->x + j] = tmp[j];
		d.ang += d.dang;
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
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
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
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
					set_pixel(iw->sur, left->x + j, i,
						get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
						((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
						((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
							((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
						((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
					), iw->sectors[iw->d.cs].light));
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
					set_pixel(iw->sur, j + left->x, i,
						get_pixel(iw->t[iw->l.skybox],
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
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
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
					set_pixel(iw->sur, left->x + j, i,
						get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
						((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
						((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
							((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
						((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
					), iw->sectors[iw->d.cs].light));
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
					set_pixel(iw->sur, j + left->x, i,
						get_pixel(iw->t[iw->l.skybox],
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
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
				), iw->sectors[iw->d.cs].light));
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h),
					iw->sectors[iw->d.cs].light));
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
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
				), iw->sectors[iw->d.cs].light));
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
				set_pixel(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h),
					iw->sectors[iw->d.cs].light));
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
	// lz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_floor_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_floor_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
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
	// lz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_ceil_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_ceil_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
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
	// lz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_floor_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_floor_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(bottom, l);
	/*draw_between_sectors_bot_tex(iw, left, right, tmp);*/

	/*lz = get_ceil_z(iw, left->wall->x, left->wall->y);
	rz = get_ceil_z(iw, right->wall->x, right->wall->y);*/
	// lz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_ceil_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_ceil_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
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

void	change_saved_top_bot_between_lines(t_sdl *iw, int len)
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
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
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
			if (d.pixel != 0x010000)
				set_pixel(iw->sur, left->x + j, i, get_light_color(d.pixel, iw->sectors[iw->d.cs].light));
			d.ty += d.dty;
		}
		d.ang += d.dang;
	}
}

// int		portal_not_visited(t_sdl *iw, t_wall *wall)
// {
// 	t_visited_portal	*p;

// 	p = iw->visited_portals;
// 	while (p)
// 	{
// 		if (wall == p->wall)
// 			return (0);
// 		p = p->next;
// 	}
// 	return (1);
// }

int		sector_visited(t_sdl *iw, int sec)
{
	t_visited_sector	*s;

	s = iw->visited_sectors;
	while (s)
	{
		if (sec == s->sec)
			return (1);
		s = s->next;
	}
	return (0);
}

void	draw_glass_sprites(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (tmp1->num_sec == iw->d.cs && tmp1->draweble)
		{
			draw_sprite(iw, tmp1);
			tmp1->draweble = 0;
		}
		tmp1 = tmp1->next; 
	}
}

void	draw_start(t_sdl *iw);
void	draw_skybox(t_sdl *iw);
void	draw_next_sector(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_sdl	iw2;
	t_visited_sector	*tmp;
	int					i;

	//printf("next_sector\n");
	iw2 = *iw;
	//	if (iw->sectors[iw->d.cs].cl.t < 0)
	fill_top_skybox(&iw2, left, right->x - left->x + 1);
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	iw->d.save_bot_betw = get_between_sectors_walls(&iw2, left, right, &iw->d.save_top_betw);
	draw_between_sectors_walls(&iw2, left, right);
	i = left->x - 1;
	while (++i < right->x)
	{
		iw->d.top[i] = iw2.d.top[i];
		iw->d.bottom[i] = iw2.d.bottom[i];
	}
	if (sector_visited(iw, iw2.d.cs))
	{
		free(iw->d.save_bot_betw);
		free(iw->d.save_top_betw);
		return;
	}
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

	iw->sectors[iw2.d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw2.d.cs;
	tmp->next = iw2.visited_sectors;
	iw2.visited_sectors = tmp;
	draw_start(&iw2);
	iw2.visited_sectors = iw2.visited_sectors->next;
	free(tmp);

	if (left->wall->glass >= 0)
	{
		if (iw->sectors[iw2.d.cs].cl.t < 0)
			draw_skybox(&iw2);
		sort_sprites(iw);
		draw_glass_sprites(&iw2);
		change_saved_top_bot_between_lines(iw, right->x - left->x + 1);
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
	t_visited_sector	*tmp;

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
		(iw->d.wallTop[0] >= WINDOW_H && iw->d.wallTop[len - 1] >= WINDOW_H)
		|| sector_visited(iw, iw2.d.cs))
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

	iw->sectors[iw2.d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw2.d.cs;
	tmp->next = iw2.visited_sectors;
	iw2.visited_sectors = tmp;
	draw_start(&iw2);
	iw2.visited_sectors = iw2.visited_sectors->next;
	free(tmp);

	if (left->wall->glass >= 0)
	{
		if (iw->sectors[iw2.d.cs].cl.t < 0)
			draw_skybox_kernel(&iw2);
		sort_sprites(iw);
		draw_glass_sprites_kernel(&iw2);
		change_saved_top_bot_between_lines(iw, len);
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
	while (i++ <= d.rx0 && i <= WINDOW_W)
	{
		if (iw->d.top_save[i] >= iw->d.bottom_save[i])
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
			while (j++ < d.ry1_down + (float)d.dy_down && j < iw->d.bottom_save[i])
			{
				d.pixel = get_pixel(iw->t[pic->t], (int)d.pic_x, (int)d.pic_y);
				if (d.pixel != 0x010000)
					set_pixel(iw->sur, i, j, get_light_color(d.pixel, 
						iw->sectors[iw->d.cs].light));
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
				iw->k.m_save_top2, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
			clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_bottom,
				iw->k.m_save_bottom2, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
		}
		if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_wall_floor_ceil_tex_kernel(iw, left, right, len);
		else
			draw_inclined_wall_floor_ceil_tex_kernel(iw, left, right, len);
		if (left->wall->p != 0)
			draw_pictures_kernel(iw, left);
	}
	else
		draw_next_sector_kernel(iw, left, right, len);
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

int		check_look_picture(t_sdl *iw)
{
	t_picture	*pic;

	if (*(iw->v.look_wall) == 0 || *(iw->v.look_picture) == 0)
		return (0);
	pic = (*(iw->v.look_wall))->p;
	while (pic)
	{
		if (pic == *(iw->v.look_picture))
			return (1);
		pic = pic->next;
	}
	return (0);
}

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
	d.dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
	d.sky_x = d.rot * ((float)iw->t[iw->l.skybox]->w) / G360
		+ d.dx * (float)iw->d.screen_left;
	d.dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	j = iw->d.screen_left - 1;
	while (++j < iw->d.screen_right)
	{
		d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
		d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
		d.sky_y += d.dy * iw->d.top[j];
		i = iw->d.top[j] - 1;
		while (++i <= iw->d.bottom[j] && i < WINDOW_H)
		{
			set_pixel(iw->sur, j, i,
				get_pixel(iw->t[iw->l.skybox],
				(int)d.sky_x, (int)d.sky_y));
			d.sky_y += d.dy;
		}
		d.sky_x += d.dx;
		if (d.sky_x >= iw->t[iw->l.skybox]->w)
			d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
		iw->d.bottom[j] = iw->d.top[j];
	}
}

int		get_max(int i1, int i2)
{
	if (i1 > i2)
		return (i1);
	else
		return (i2);
}

int		get_min(int i1, int i2)
{
	if (i1 < i2)
		return (i1);
	else
		return (i2);
}
// SPRITES DRAW /////////////////////////////////////////////////////////////////////////////////////////////////////
void	draw_sprite(t_sdl *iw, t_sprite *sprite)
{
	int		i;
	int		j;
	int		stripe;
	int		y;
	float	koef;
	int		texX;
	int		texY;
	int		colour;

	sprite->spriteheight = 2 * sprite->spritewidth * sprite->t->h / sprite->t->w;
	if (abs(sprite->z - iw->p.z) > 1500)
		sprite->spriteheight = (float)sprite->spriteheight * 1.0f /
		(float)(abs(sprite->z - iw->p.z) / 1500.0f);
	sprite->ey = WINDOW_H * (iw->p.z + (int)sprite->pplen / 2 - sprite->z) / (int)sprite->pplen + iw->p.rotup;
	sprite->sy = sprite->ey - sprite->spriteheight;
	i = 0;
	stripe = sprite->sx - 1;
	if (stripe < -1)
	{
		i -= stripe + 1;
		stripe = -1;
	}
	while (++stripe < sprite->ex && stripe < WINDOW_W)
	{
		j = 0;
		koef = (float)sprite->spritewidth * 2.0f / (float)sprite->t->w;
		texX = (int)fabsf((float)i / koef);
		if ((sprite->top[stripe] < sprite->bottom[stripe])
			&& sprite->top[stripe] != -1)
		{
			y = sprite->sy - 1;
			if (y < -1)
			{
				j -= y + 1;
				y = -1;
			}
			while (++y < sprite->ey && y < WINDOW_H)
			{
				if (sprite->sy < WINDOW_H && sprite->bottom[stripe] >y && sprite->top[stripe] < y)
				{
					koef = (float)sprite->spriteheight / sprite->t->h;
					texY = (int)(j / koef);
					colour = get_pixel(sprite->t, texX, texY);
					if (colour != 0x010000)
						set_pixel(iw->sur, stripe, y,
							get_light_color(colour, iw->sectors[sprite->num_sec].light));
				}
				j++;
			}
	  	}
		i++;
	}
}

void	draw_sprites(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (iw->sectors[tmp1->num_sec].visited && tmp1->draweble && tmp1->taken == 0)
		{
			if (tmp1->top[WINDOW_W / 2] != -1 && tmp1->sx < WINDOW_W / 2 && tmp1->ex > WINDOW_W / 2 &&
				tmp1->top[WINDOW_W / 2] < WINDOW_H / 2 && tmp1->bottom[WINDOW_W / 2] > WINDOW_H / 2
				&& tmp1->sy < WINDOW_H / 2 && tmp1->ey > WINDOW_H / 2)
				iw->v.look_sprite = tmp1;
			draw_sprite(iw, tmp1);
		}
		tmp1 = tmp1->next; 
	}
}

int		find_point(t_save_wall_pairs *tmp, t_sprite *tmp1)
{
	int		i;
	int ax = tmp->right->p.x-tmp->left->p.x;
	int ay = tmp->right->p.y-tmp->left->p.y;
	int bx = tmp1->x - tmp->left->p.x;
	int by = tmp1->y - tmp->left->p.y;
	i = ax * by - ay * bx;
	if (i > 0)
		return (0);
	else 
		return (1);
}

void	calculate_sprites_once(t_sdl *iw)
{
	t_sprite    *tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (!iw->sectors[tmp1->num_sec].visited || tmp1->draweble == 1)
		{
			tmp1 = tmp1->next;
			continue;
		}
		float lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(tmp1->x - iw->p.x ), (float)(tmp1->y - iw->p.y ));
		float rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
				(float)(tmp1->x - iw->p.x ), (float)(tmp1->y - iw->p.y ));
		tmp1->x_s = (int)(lang * (float)WINDOW_W / (2.0f * iw->v.angle));
		
		if (rang > 2.0f * iw->v.angle)
			tmp1->x_s = -tmp1->x_s;
		//	printf("x_s = %d\n", tmp1->x_s);
		
		tmp1->plen = sqrtf(powf((float)(iw->p.x - tmp1->x), 2.0f) +  powf((float)(iw->p.y - tmp1->y), 2.0f)) + 1.0f;
		tmp1->pplen = fabsf(iw->d.screen.a * (float)tmp1->x + iw->d.screen.b * (float)tmp1->y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b) + 1.0f;
		//printf("div  %f\n", tmp1->pplen / tmp1->plen);
		if (tmp1->pplen / tmp1->plen >= 0.5f)
			tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w) / tmp1->pplen) * tmp1->scale);
		else
			tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w) / tmp1->plen) * tmp1->scale);
		tmp1->sx = tmp1->x_s - tmp1->spritewidth;
		tmp1->ex = tmp1->x_s + tmp1->spritewidth;
		if (!(tmp1->sx > iw->d.screen_right || tmp1->ex < iw->d.screen_left))
			tmp1->draweble = 1;
		tmp1 = tmp1->next;
	}
}

void	get_sprites_top_bottom(t_sdl *iw, t_save_wall_pairs	*tmp)
{
	t_sprite    *tmp1;
	int j;
	int	jend;

	if (iw->v.kernel)
	{
		clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_top, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
		clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_bottom, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	}
	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (!iw->sectors[tmp1->num_sec].visited || !tmp1->draweble || iw->d.cs != tmp1->num_sec)
		{
			tmp1 = tmp1->next;
			continue;
		}
		if ((tmp1->sx >= tmp->left->x && tmp1->sx <= tmp->right->x)
			|| (tmp1->ex >= tmp->left->x && tmp1->ex<= tmp->right->x)
			|| (tmp1->sx <= tmp->left->x && tmp1->ex >= tmp->right->x))
			if (find_point(tmp, tmp1) == 1)
			{
				j = get_max(tmp1->sx, get_max(tmp->left->x, iw->d.screen_left)) - 1;
				jend = get_min(tmp1->ex, get_min(tmp->right->x, iw->d.screen_right));
				while (++j <= jend)
					if (tmp1->top[j] == -1)
					{
						tmp1->top[j] = iw->d.top[j];
						tmp1->bottom[j] = iw->d.bottom[j];
					}
			}
		tmp1 = tmp1->next;
	}
}

void	switch_nexts_sprites(t_sprite *s1, t_sprite *s2)
{
	t_sprite	*tmp;
	t_sprite	*tmp2;

	if (!s1 || !s2)
		return;
	if (s2->next == s1)
	{
		s2->next = s2->next->next;
		s1->next = s1->next->next;
		s2->next->next = s1;
	}
	else
	{
		tmp = s1->next->next;
		s1->next->next = s2->next->next;
		tmp2 = s2->next;
		s2->next = s1->next;
		s1->next = tmp2;
		tmp2->next = tmp;
	}
}

void	sort_sprites(t_sdl *iw)
{
	t_sprite	head;
	t_sprite	*tmp1;
	t_sprite	*tmp2;
	t_sprite	*max;

	if (!*iw->sprite)
		return;
	head.next = *iw->sprite;
	tmp1 = &head;
	while (tmp1->next->next != 0)
	{
		if (!iw->sectors[tmp1->next->num_sec].visited || !tmp1->next->draweble)
		{
			tmp1 = tmp1->next;
			continue;
		}
		max = 0;
		tmp2 = tmp1->next;
		while (tmp2->next != 0)
		{
			if (iw->sectors[tmp2->next->num_sec].visited && tmp2->next->draweble)
				//if (max == 0 || max->next->plen < tmp2->next->plen)
				if (tmp2->next->plen > tmp1->next->plen && (max == 0 || max->next->plen < tmp2->next->plen))
					max = tmp2;
			tmp2 = tmp2->next;
		}
		switch_nexts_sprites(max, tmp1);
		tmp1 = tmp1->next;
	}
	*iw->sprite = head.next;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	save_walls(t_sdl *iw)
{
	t_save_wall	*tmp;

	if (!iw->d.vw)
		return;
	tmp = iw->d.vw;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = *(iw->vw_save);
	*(iw->vw_save) = iw->d.vw;
	iw->d.vw = 0;
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
	calculate_sprites_once(iw);
	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		get_sprites_top_bottom(iw, tmp);
		draw_left_right(iw, tmp->left, tmp->right);
		tmp = tmp->next;
		/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_sur, CL_TRUE, 0,
			WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);*/
		//SDL_UpdateWindowSurface(iw->win);
		// char s[500];
		// read(0, s, 10);
		//system("PAUSE");
	}

	save_walls(iw);
	//free_walls(iw);
	free_pairs(iw);
}

void	draw(t_sdl *iw)
{
	t_visited_sector	*tmp;

	t_sprite *tmp1;
	int j;

	j = 0;
	tmp1 = *iw->sprite;
	while(tmp1 != 0)
	{
		tmp1->draweble = 0;
		j = 0;
		while (j < WINDOW_W)
		{
			tmp1->top[j] = -1;
			j++;
		}
		tmp1 = tmp1->next;
	}

	j = -1;
	while (++j < iw->v.sc)
		iw->sectors[j].visited = 0;

	set_top_bottom(iw);
	if ((iw->d.cs = get_sector(iw)) == -1)
		return;
	get_direction(iw);
	get_screen_line(iw);
	get_left_right_lines_points(iw);
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
		iw->v.look_sprite = 0;
	}
	iw->sectors[iw->d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw->d.cs;
	tmp->next = iw->visited_sectors;
	iw->visited_sectors = tmp;
	draw_start(iw);
	iw->visited_sectors = iw->visited_sectors->next;
	free(tmp);
	if (!check_look_picture(iw))
		*(iw->v.look_picture) = 0;
	if (!iw->v.kernel)
		draw_skybox(iw);
	else
		draw_skybox_kernel(iw);

	sort_sprites(iw);
	if (iw->v.kernel)
		draw_sprites_kernel(iw);
	else
		draw_sprites(iw);
	
	if (iw->v.game_mode && !iw->v.kernel)
		draw_gun(iw);
	else if (iw->v.game_mode)
		draw_gun_kernel(iw);
	if (iw->v.kernel)
		iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_sur, CL_TRUE, 0,
			WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	if (iw->v.wall_anim != 0)
		draw_selected_walls_to_be_animated(iw);
	iw->d.vw = *(iw->vw_save);
	*(iw->vw_save) = 0;
	free_walls(iw);

	//draw_gun(iw);
}

void	read_textures(t_sdl *iw)
{
	iw->hud.enot_sur = SDL_LoadBMP("HUD/enot.bmp");
	iw->t_sur[0] = SDL_LoadBMP("textures/0.bmp");
	iw->tsz[0] = 1.0f;
	iw->t_sur[1] = SDL_LoadBMP("textures/1.bmp");
	iw->tsz[1] = 1.0f;
	iw->t_sur[2] = SDL_LoadBMP("textures/2.bmp");
	iw->tsz[2] = 1.0f;
	iw->t_sur[3] = SDL_LoadBMP("textures/3.bmp");
	iw->tsz[3] = 1.0f;
	iw->t_sur[4] = SDL_LoadBMP("textures/4.bmp");
	iw->tsz[4] = 1.0f;
	iw->t_sur[5] = SDL_LoadBMP("textures/5.bmp");
	iw->tsz[5] = 1.0f;
	iw->t_sur[6] = SDL_LoadBMP("textures/6.bmp");
	iw->tsz[6] = 1.0f;
	iw->t_sur[7] = SDL_LoadBMP("textures/7.bmp");
	iw->tsz[7] = 1.0f;
	iw->t_sur[8] = SDL_LoadBMP("textures/8.bmp");
	iw->tsz[8] = 1.0f;
	iw->t_sur[9] = SDL_LoadBMP("textures/9.bmp");
	iw->tsz[9] = 1.0f;
	iw->t_sur[10] = SDL_LoadBMP("textures/10.bmp");
	iw->tsz[10] = 1.0f;
	iw->t_sur[11] = SDL_LoadBMP("textures/11.bmp");
	iw->tsz[11] = 1.0f;
	iw->t_sur[12] = SDL_LoadBMP("textures/12.bmp");
	iw->tsz[12] = 1.0f;
	iw->t_sur[13] = SDL_LoadBMP("textures/13.bmp");
	iw->tsz[13] = 1.0f;
	iw->t_sur[14] = SDL_LoadBMP("textures/14.bmp");
	iw->tsz[14] = 1.0f;
	iw->t_sur[15] = SDL_LoadBMP("textures/15.bmp");
	iw->tsz[15] = 1.0f;
	iw->t_sur[16] = SDL_LoadBMP("textures/16.bmp");
	iw->tsz[16] = 1.0f;
	iw->t_sur[17] = SDL_LoadBMP("textures/17.bmp");
	iw->tsz[17] = 1.0f;
	iw->t_sur[18] = SDL_LoadBMP("textures/18.bmp");
	iw->tsz[18] = 1.0f;
	iw->t_sur[19] = SDL_LoadBMP("textures/19.bmp");
	iw->tsz[19] = 1.0f;
	iw->t_sur[20] = SDL_LoadBMP("textures/20.bmp");
	iw->tsz[20] = 1.0f;
	// iw->t[17] = SDL_LoadBMP("textures/17.bmp");
	// iw->tsz[17] = 1.0f;
	//iw->t[18] = SDL_LoadBMP("textures/19.bmp");
	//iw->tsz[18] = 1.0f;
	//Uint8 *target_pixel = (Uint8 *)(iw->t)[0]->pixels;
	//set_pixel((iw->t)[0], 0, 0, 0xFF0000);
	//printf("%d\n", read_pixel((iw->t)[0], 2, 0));
	iw->bag.button_sur[0] = SDL_LoadBMP("backpack/frame.bmp");
	iw->bag.button_sur[1] = SDL_LoadBMP("backpack/del.bmp");
	iw->bag.button_sur[2] = SDL_LoadBMP("backpack/ok.bmp");
}

void	read_sprites_textures(t_sdl *iw)
{
	iw->t_decor_sur[0] = SDL_LoadBMP("sprites/decorations/0.bmp");
	iw->t_decor_sur[1] = SDL_LoadBMP("sprites/decorations/1.bmp");
	/*iw->t_decor[1] = SDL_LoadBMP("sprites/decorations/1.bmp");
	iw->t_decor[2] = SDL_LoadBMP("sprites/decorations/2.bmp");
	iw->t_decor[3] = SDL_LoadBMP("sprites/decorations/3.bmp");*/

	iw->t_enemies_sur[0] = SDL_LoadBMP("sprites/enemies/0.bmp");
	iw->t_enemies_sur[1] = SDL_LoadBMP("sprites/enemies/1.bmp");
	iw->t_enemies_sur[2] = SDL_LoadBMP("sprites/enemies/2.bmp");
	iw->t_enemies_sur[3] = SDL_LoadBMP("sprites/enemies/3.bmp");
	iw->t_enemies_sur[4] = SDL_LoadBMP("sprites/enemies/4.bmp");
	iw->t_enemies_sur[5] = SDL_LoadBMP("sprites/enemies/5.bmp");
	iw->t_enemies_sur[6] = SDL_LoadBMP("sprites/enemies/6.bmp");
	iw->t_enemies_sur[7] = SDL_LoadBMP("sprites/enemies/7.bmp");
	iw->t_enemies_sur[8] = SDL_LoadBMP("sprites/enemies/8.bmp");
	iw->t_enemies_sur[9] = SDL_LoadBMP("sprites/enemies/9.bmp");
	iw->t_enemies_sur[10] = SDL_LoadBMP("sprites/enemies/10.bmp");
	iw->t_enemies_sur[11] = SDL_LoadBMP("sprites/enemies/11.bmp");
	iw->t_enemies_sur[12] = SDL_LoadBMP("sprites/enemies/12.bmp");
	iw->t_enemies_sur[13] = SDL_LoadBMP("sprites/enemies/13.bmp");
	iw->t_enemies_sur[14] = SDL_LoadBMP("sprites/enemies/14.bmp");
	iw->t_enemies_sur[15] = SDL_LoadBMP("sprites/enemies/15.bmp");
	iw->t_enemies_sur[16] = SDL_LoadBMP("sprites/enemies/16.bmp");
	iw->t_enemies_sur[17] = SDL_LoadBMP("sprites/enemies/17.bmp");
	iw->t_enemies_sur[18] = SDL_LoadBMP("sprites/enemies/18.bmp");
	iw->t_enemies_sur[19] = SDL_LoadBMP("sprites/enemies/19.bmp");

	iw->t_pickup_sur[0] = SDL_LoadBMP("sprites/to_pick_up/0.bmp");
	iw->t_pickup_sur[1] = SDL_LoadBMP("sprites/to_pick_up/1.bmp");
	iw->t_pickup_sur[2] = SDL_LoadBMP("sprites/to_pick_up/2.bmp");
	iw->t_pickup_sur[3] = SDL_LoadBMP("sprites/to_pick_up/3.bmp");
	iw->t_pickup_sur[4] = SDL_LoadBMP("sprites/to_pick_up/4.bmp");
	iw->t_pickup_sur[5] = SDL_LoadBMP("sprites/to_pick_up/5.bmp");
	iw->t_pickup_sur[6] = SDL_LoadBMP("sprites/to_pick_up/6.bmp");
	iw->t_pickup_sur[7] = SDL_LoadBMP("sprites/to_pick_up/7.bmp");
	iw->t_pickup_sur[8] = SDL_LoadBMP("sprites/to_pick_up/8.bmp");
	iw->t_pickup_sur[9] = SDL_LoadBMP("sprites/to_pick_up/9.bmp");
	iw->t_pickup_sur[10] = SDL_LoadBMP("sprites/to_pick_up/10.bmp");
	iw->t_pickup_sur[11] = SDL_LoadBMP("sprites/to_pick_up/11.bmp");
}

void	read_weapons_textures(t_sdl *iw)
{
	iw->t_weap_sur[0] = SDL_LoadBMP("weapons/0.bmp");
	iw->t_weap_sur[1] = SDL_LoadBMP("weapons/1.bmp");
	iw->t_weap_sur[2] = SDL_LoadBMP("weapons/2.bmp");
	iw->t_weap_sur[3] = SDL_LoadBMP("weapons/3.bmp");
	iw->t_weap_sur[4] = SDL_LoadBMP("weapons/4.bmp");
	iw->t_weap_sur[5] = SDL_LoadBMP("weapons/5.bmp");
	iw->t_weap_sur[6] = SDL_LoadBMP("weapons/6.bmp");
	iw->t_weap_sur[7] = SDL_LoadBMP("weapons/7.bmp");
	iw->t_weap_sur[8] = SDL_LoadBMP("weapons/8.bmp");
	iw->t_weap_sur[9] = SDL_LoadBMP("weapons/9.bmp");
	iw->t_weap_sur[10] = SDL_LoadBMP("weapons/10.bmp");
	iw->t_weap_sur[11] = SDL_LoadBMP("weapons/11.bmp");
	iw->t_weap_sur[12] = SDL_LoadBMP("weapons/12.bmp");
	iw->t_weap_sur[13] = SDL_LoadBMP("weapons/13.bmp");
	iw->t_weap_sur[14] = SDL_LoadBMP("weapons/14.bmp");
	iw->t_weap_sur[15] = SDL_LoadBMP("weapons/15.bmp");
	iw->t_weap_sur[16] = SDL_LoadBMP("weapons/16.bmp");
	iw->t_weap_sur[17] = SDL_LoadBMP("weapons/17.bmp");
	iw->t_weap_sur[18] = SDL_LoadBMP("weapons/18.bmp");
}

void add_sprite(t_sdl *iw, int x, int y, int z, int t, int num, int type, float scale)
{
	t_sprite *tmp;

	tmp = (t_sprite *)malloc(sizeof(t_sprite));
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;
	//tmp->t = t;
	tmp->num_sec = num;
	tmp->next = *iw->sprite;
	tmp->type = type;
	tmp->scale = scale;
	tmp->e.vis_esp.curr_sector = -1;
	tmp->e.previous_picture_change = 1;
	tmp->e.prev_update_time = 1;
	tmp->fall_time = 1;

	tmp->taken = 0;

	//tmp->e.previous_move_to_player_time = 1;
	(*iw->sprite) = tmp;
	tmp->t_numb = t;
	if (type == 0)
	{
		tmp->t = iw->t_decor[t];
		tmp->t_kernel = &iw->k.m_t_decor[t];
	}
	else if (type == 1)
	{
		tmp->t = iw->t_pickup[t];
		tmp->t_kernel = &iw->k.m_t_pickup[t];
	}
	else if (type == 2)
	{
		tmp->t = iw->t_enemies[t];
		tmp->t_kernel = &iw->k.m_t_enemies[t];
	}
	
}

void	get_def(t_sdl *iw)
{
	iw->p.x = 511;
	iw->p.y = 1684; //-2360
	iw->p.z = 100;
	iw->p.introt = 3;
	iw->p.rot = (float)iw->p.introt * G1;
	iw->p.rotup = 2; //550
	iw->v.ls = 0;
	iw->v.angle = (float)WINDOW_W / (float)WINDOW_H * 22.0f * G1;// 0.698132f;
	iw->v.kernel = 1;
	load_kernel(&iw->k, iw);
	//fill_floor_coefficients(iw);
	iw->v.front = 1;
	iw->v.back = 1;
	iw->v.left = 1;
	iw->v.right = 1;
	iw->v.rot_left = 1;
	iw->v.rot_right = 1;
	iw->v.rot_up = 1;
	iw->v.rot_down = 1;
	iw->v.fall = 1;
	iw->v.jump_time = 1;
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
	iw->v.look_sprite = 0;
	iw->v.look_portal = 0;
	iw->v.changing_fc = 0;
	iw->v.chang_fc_rect.h = 100;
	iw->v.chang_fc_rect.w = 40;
	iw->v.chang_fc_rect.x = 0;
	iw->v.chang_fc_rect.y = WINDOW_H + 100;
	iw->l.skybox = 13;
	iw->v.picture_changing = 0;
	//iw->v.edit_mode = 0;
	//iw->visited_portals = 0;
	iw->visited_sectors = 0;

	iw->sprite = (t_sprite **)malloc(sizeof(t_sprite *));
	*iw->sprite = 0;

	iw->v.f_button_mode = 0;
	iw->sector_animations = 0;
	iw->v.submenu_mode = 0;
	iw->v.sector_anim = 0;

	iw->wall_animations = 0;
	iw->v.wall_anim = 0;
	iw->vw_save = (t_save_wall **)malloc(sizeof(t_save_wall *));
	*(iw->vw_save) = 0;

	iw->v.sprites_select_mode = 0;
	iw->v.scroll_decor_sprites = 0;
	iw->v.scroll_pickup_sprites = 0;
	iw->v.selected_sprite_type = 0;
	iw->v.selected_sprite = 0;
	iw->v.sprite_editing = 0;

	iw->v.fly_mode = 0;
	iw->v.fly_down = -1;
	iw->v.fly_up = -1;

	iw->hud.rad = 100;
	iw->p.health = 100;
	iw->hud.shell = 100;

	iw->v.weapon_change_x = 0;
	iw->v.weapon_change_y = 20;
	iw->v.weapon_change_xdir = 1;
	iw->v.weapon_change_ydir = 1;
	iw->v.left_mouse_pressed = 0;
	iw->v.weapon_change_y_hide = 0;

	iw->bag.bag = 0;
	iw->bag.count_items = 0;
	iw->bag.selected_item = 0;
}

void	get_kernel_mem(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < TEXTURES_COUNT)
	{
		iw->k.m_t[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t[i]->pitch * iw->t[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t[i], CL_TRUE, 0,
			iw->t[i]->pitch * iw->t[i]->h, iw->t[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
	{
		iw->k.m_t_decor[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_decor[i]->pitch * iw->t_decor[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_decor[i], CL_TRUE, 0,
			iw->t_decor[i]->pitch * iw->t_decor[i]->h, iw->t_decor[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
	{
		iw->k.m_t_enemies[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_enemies[i]->pitch * iw->t_enemies[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_enemies[i], CL_TRUE, 0,
			iw->t_enemies[i]->pitch * iw->t_enemies[i]->h, iw->t_enemies[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
	{
		iw->k.m_t_pickup[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_pickup[i]->pitch * iw->t_pickup[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_pickup[i], CL_TRUE, 0,
			iw->t_pickup[i]->pitch * iw->t_pickup[i]->h, iw->t_pickup[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
	{
		iw->k.m_t_weap[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_weap[i]->pitch * iw->t_weap[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_weap[i], CL_TRUE, 0,
			iw->t_weap[i]->pitch * iw->t_weap[i]->h, iw->t_weap[i]->pixels, 0, NULL, NULL);
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
		26 * sizeof(int), NULL, &iw->k.ret);
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
	iw->k.m_save_top2 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom2 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top3 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom3 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
}

void	calculate_not_squared_wall_picture(t_wall *wall, t_picture *pic)
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
		calculate_not_squared_wall_picture(wall, pic);

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

//	GET_SECTOR_WAYS_FUNCTIONS //////////////////////////////////////////////////////////

void	free_way(t_sector_ways **way)
{
	t_sector_way	*tmp;
	t_sector_way	*tmp2;

	if (*way == 0)
		return;
	tmp = (*way)->way_start;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	free(*way);
	*way = 0;
}

int		sector_in_way(t_sdl *iw, t_sector_ways *way, int sector)
{
	t_sector_way	*tmp;

	if (way == 0)
		return (0);
	tmp = way->way_start;
	while (tmp)
	{
		if (iw->walls[tmp->portal].nextsector == sector)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	add_portal_in_way2(t_sector_ways *tmp, int portal)
{
	t_sector_way	*nw;
	t_sector_way	*w;

	nw = (t_sector_way *)malloc(sizeof(t_sector_way));
	nw->portal = portal;
	nw->next = 0;
	if (tmp->way_start == 0)
		tmp->way_start = nw;
	else
	{
		w = tmp->way_start;
		while (w->next)
			w = w->next;
		w->next = nw;
	}
}

t_sector_ways	*add_portal_in_way(t_sector_ways *current_way, int portal)
{
	t_sector_ways	*tmp;
	t_sector_way	*way;

	tmp = (t_sector_ways *)malloc(sizeof(t_sector_ways));
	tmp->way_start = 0;
	if (current_way == 0)
	{
		tmp->way_start = (t_sector_way *)malloc(sizeof(t_sector_way));
		tmp->way_start->next = 0;
		tmp->way_start->portal = portal;
		return (tmp);
	}
	way = current_way->way_start;
	while (way)
	{
		add_portal_in_way2(tmp, way->portal);
		way = way->next;
	}
	add_portal_in_way2(tmp, portal);
	return (tmp);
}

void	go_in_sector_way(t_sdl *iw, t_get_sectors_ways *g, t_sector_ways *current_way)
{
	int				wall;
	int				save;

	if (g->current == g->to)
	{
		current_way->next = g->ways;
		g->ways = current_way;
		return;
	}
	wall = iw->sectors[g->current].sw - 1;
	while (++wall < iw->sectors[g->current].sw + iw->sectors[g->current].nw)
	{
		if (iw->walls[wall].nextsector == -1 ||
			sector_in_way(iw, current_way, iw->walls[wall].nextsector) ||
			iw->walls[wall].nextsector == g->from)
			continue;
		save = g->current;
		g->current = iw->walls[wall].nextsector;
		go_in_sector_way(iw, g, add_portal_in_way(current_way, wall));
		g->current = save;
	}
	free_way(&current_way);
}

void	get_sectors_ways2(t_sdl *iw)
{
	t_get_sectors_ways	g;

	g.from = -1;
	while (++g.from < iw->v.sc)
	{
		g.to = -1;
		while (++g.to < iw->v.sc)
		{
			if (g.from == g.to)
				continue;
			g.ways = 0;
			g.current = g.from;
			go_in_sector_way(iw, &g, 0);
			iw->ways[g.from][g.to] = g.ways;
		}
	}
}

void	get_sectors_ways(t_sdl *iw)
{
	int		i;
	int		j;

	iw->ways = (t_sector_ways ***)malloc(iw->v.sc * sizeof(t_sector_ways **));
	i = -1;
	while (++i < iw->v.sc)
	{
		iw->ways[i] = (t_sector_ways **)malloc(iw->v.sc * sizeof(t_sector_ways *));
		j = -1;
		while (++j < iw->v.sc)
			iw->ways[i][j] = 0;
	}
	get_sectors_ways2(iw);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	get_packaging_texture(t_packaging_texture **pack, SDL_Surface *sur)
{
	*pack = (t_packaging_texture *)malloc(sizeof(t_packaging_texture));
	(*pack)->w = sur->w;
	(*pack)->h = sur->h;
	(*pack)->pitch = sur->pitch;
	(*pack)->bpp = sur->format->BytesPerPixel;
	(*pack)->pixels = malloc(sur->pitch * sur->h);
	ft_memcpy((*pack)->pixels, sur->pixels, sur->pitch * sur->h);
	SDL_FreeSurface(sur);
}

void	get_packaging_textures(t_sdl *iw)
{
	int		i;

	get_packaging_texture(&iw->hud.enot, iw->hud.enot_sur);
	i = -1;
	while (++i < TEXTURES_COUNT)
		get_packaging_texture(&iw->t[i], iw->t_sur[i]);
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_decor[i], iw->t_decor_sur[i]);
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_enemies[i], iw->t_enemies_sur[i]);
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_pickup[i], iw->t_pickup_sur[i]);
	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_weap[i], iw->t_weap_sur[i]);
	get_packaging_texture(&iw->bag.button[0], iw->bag.button_sur[0]);
	get_packaging_texture(&iw->bag.button[1], iw->bag.button_sur[1]);
	get_packaging_texture(&iw->bag.button[2], iw->bag.button_sur[2]);
}

void	set_sprites_z(t_sdl *iw)
{
	t_sprite	*s;

	s = *(iw->sprite);
	while (s)
	{
		if (s->type != 2 || s->e.enemy_numb != 0)
			s->z = get_floor_z_sec(iw, s->x, s->y, s->num_sec);
		s = s->next;
	}
}

void	get_guns_center(t_sdl *iw, int i, int scale)
{
	iw->guns.t_rect[i].w = iw->t_weap[i]->w * WINDOW_W / scale;
	iw->guns.t_rect[i].h = iw->guns.t_rect[i].w * iw->t_weap[i]->h / iw->t_weap[i]->w;
	iw->guns.t_rect[i].x = (WINDOW_W - iw->guns.t_rect[i].w) / 2;
	iw->guns.t_rect[i].y = (WINDOW_H - iw->guns.t_rect[i].h) / 2;
}

void	get_guns_center_down(t_sdl *iw, int i, int scale)
{
	iw->guns.t_rect[i].w = iw->t_weap[i]->w * WINDOW_W / scale;
	iw->guns.t_rect[i].h = iw->guns.t_rect[i].w * iw->t_weap[i]->h / iw->t_weap[i]->w;
	iw->guns.t_rect[i].x = (WINDOW_W - iw->guns.t_rect[i].w) / 2;
	iw->guns.t_rect[i].y = WINDOW_H - iw->guns.t_rect[i].h;
}

void	get_guns(t_sdl *iw)
{
	iw->guns.t = 17;
	iw->guns.max_bullets[0] = 1;
	iw->guns.max_bullets[1] = 4;
	iw->guns.max_bullets[2] = 10;
	iw->guns.max_bullets_in_stock[0] = 0;
	iw->guns.max_bullets_in_stock[1] = 40;
	iw->guns.max_bullets_in_stock[2] = 100;
	iw->guns.bullets[0] = iw->guns.max_bullets[0];
	iw->guns.bullets[1] = iw->guns.max_bullets[1];//0;
	iw->guns.bullets[2] = iw->guns.max_bullets[2];//0;
	iw->guns.bullets_in_stock[0] = 0;
	iw->guns.bullets_in_stock[1] = 8;//iw->guns.max_bullets_in_stock[1];
	iw->guns.bullets_in_stock[2] = 10;// iw->guns.max_bullets_in_stock[2];

	iw->guns.gun_in_hands = 0;
	iw->guns.status = 0;

	get_guns_center_down(iw, 17, 1000);
	get_guns_center_down(iw, 18, 1000);
	get_guns_center_down(iw, 0, 3000);
	iw->guns.t_rect[0].x += iw->guns.t_rect[0].w / 3 * 2;
	get_guns_center_down(iw, 1, 3000);
	iw->guns.t_rect[1].x += iw->guns.t_rect[1].w * 4 / 5;
	get_guns_center(iw, 2, 1700);
	iw->guns.t_rect[2].x += WINDOW_W / 10;
	iw->guns.t_rect[2].y -= WINDOW_H / 5;
	get_guns_center(iw, 3, 1700);
	iw->guns.t_rect[3].x += WINDOW_W / 9;
	iw->guns.t_rect[3].y -= WINDOW_H / 3;
	get_guns_center(iw, 4, 2200);
	iw->guns.t_rect[4].x += WINDOW_W / 8;
	iw->guns.t_rect[4].y -= WINDOW_H / 3;
	get_guns_center(iw, 5, 2200);
	iw->guns.t_rect[5].x += WINDOW_W / 7;
	iw->guns.t_rect[5].y -= WINDOW_H / 5;
	get_guns_center(iw, 6, 2200);
	iw->guns.t_rect[6].x += WINDOW_W / 6;
	iw->guns.t_rect[6].y += WINDOW_H / 10;
	get_guns_center_down(iw, 7, 2500);
	iw->guns.t_rect[7].x += iw->guns.t_rect[7].w * 3 / 5;
	get_guns_center_down(iw, 8, 2500);
	iw->guns.t_rect[8].x += iw->guns.t_rect[8].w * 3 / 5;
	get_guns_center_down(iw, 9, 2500);
	iw->guns.t_rect[9].x += iw->guns.t_rect[9].w * 3 / 5;
	get_guns_center_down(iw, 10, 1500);
	get_guns_center_down(iw, 11, 1500);
	get_guns_center_down(iw, 12, 1500);
	get_guns_center_down(iw, 13, 1500);
	get_guns_center_down(iw, 14, 1500);
	get_guns_center_down(iw, 15, 1500);
	get_guns_center_down(iw, 16, 1500);

	iw->guns.prev_update_time = clock();
}

int		main(void)
{
	t_sdl	iw;
	
	iw.v.game_mode = 1;
	get_def(&iw);
	read_textures(&iw);
	read_sprites_textures(&iw);
	read_weapons_textures(&iw);
	get_packaging_textures(&iw);
	get_kernel_mem(&iw);
	get_kernels(&iw);
	get_guns(&iw);
	//enemy
	add_sprite(&iw, 4700, -900, 0, 0, 0, 0, 2.0f);
	(*iw.sprite)->type = 2;
	(*iw.sprite)->e.enemy_numb = 0;
	(*iw.sprite)->e.health = 10;
	(*iw.sprite)->e.damage = 5;
	(*iw.sprite)->e.status = 0;
	(*iw.sprite)->t = iw.t_enemies[0];
	(*iw.sprite)->t_kernel = &iw.k.m_t_enemies[0];
	//pickup

	// add_sprite(&iw, 7240, 2640, 200, 0, 1, 0, 0.5f);
	// add_sprite(&iw, 6520, 2298, 200, 0, 1, 2, 0.5f);

	add_sprite(&iw, 8640, 2200, 200, 0, 1, 1, 0.1f);
	add_sprite(&iw, 7500, 2000, 200, 1, 1, 1, 0.5f);
	add_sprite(&iw, 7500, 3000, 200, 2, 1, 1, 0.5f);
	// add_sprite(&iw, 7500, 3000, 200, 3, 1, 1, 0.5f);

	add_sprite(&iw, 8500, 1500, 200, 3, 1, 1, 0.5f);
	add_sprite(&iw, 9000, 5500, 200, 4, 1, 1, 0.5f);
	add_sprite(&iw, 10000, 500, 200, 5, 1, 1, 0.5f);

	add_sprite(&iw, 9500, 5000, 200, 6, 1, 1, 0.5f);
	add_sprite(&iw, 7500, 3000, 200, 7, 1, 1, 0.5f);
	add_sprite(&iw, 7500, 3000, 200, 8, 1, 1, 0.5f);

	add_sprite(&iw, 8500, 1000, 200, 9, 1, 1, 0.5f);
	add_sprite(&iw, 8000, 5500, 200, 10, 1, 1, 0.5f);

	/*add_sprite(&iw,8640,2200,400, 0, 1, 0, 1.0f);
	add_sprite(&iw,6520,2298,200, 0, 1, 0, 1.0f);*/


	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	iw.arial_font = TTF_OpenFont("fonts/ARIAL.TTF", 24);
	SDL_SetRelativeMouseMode(iw.v.mouse_mode);
	if (!iw.v.game_mode)
		iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_W, WINDOW_H + 300, SDL_WINDOW_SHOWN);
	else
		iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	//SDL_SetWindowFullscreen(iw.win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	//iw.ren = SDL_CreateRenderer(iw.win, -1, 0);
	iw.sur = SDL_GetWindowSurface(iw.win);
	circle(&iw.hud, FOOTX, FOOTY);
	draw_tex_to_select(&iw);
	draw_decor_tex_to_select(&iw);
	draw_menu(&iw);
	// draw
	get_map(&iw);
	set_sprites_z(&iw);
	get_sectors_ways(&iw);
	add_picture1(&iw); ///////////////
	create_map(&iw);
	draw(&iw);
	SDL_UpdateWindowSurface(iw.win);
	main_loop(&iw);
	SDL_FreeSurface(iw.sur);
	SDL_DestroyWindow(iw.win);
	TTF_Quit();
	SDL_Quit();
	// system("PAUSE");
	return (0);
}
