/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:24:31 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 16:29:48 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_crosshair(t_sdl *iw)
{
	int		i;
	int		j;
	int		to;

	j = WINDOW_W / 2;
	i = WINDOW_H / 2 - 4;
	to = WINDOW_H / 2 - 15;
	while (--i > to)
		set_pixel2(iw->sur, j, i, 0x00FF00);
	i = WINDOW_H / 2 + 4;
	to = WINDOW_H / 2 + 15;
	while (++i < to)
		set_pixel2(iw->sur, j, i, 0x00FF00);
	i = WINDOW_H / 2;
	j = WINDOW_W / 2 + 4;
	to = WINDOW_W / 2 + 15;
	while (++j < to)
		set_pixel2(iw->sur, j, i, 0x00FF00);
	j = WINDOW_W / 2 - 4;
	to = WINDOW_W / 2 - 15;
	while (--j > to)
		set_pixel2(iw->sur, j, i, 0x00FF00);
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

void	draw_some_info2(t_sdl *iw)
{
	char		*s;

	if (iw->v.f_button_mode == 1)
	{
		draw_text(iw, "Select Sectors to control light by pressing F", 50, 150);
		draw_text(iw, "Press G to exit this mode", 50, 175);
	}
	else if (iw->v.f_button_mode == 2)
	{
		draw_text(iw, "Select Sector to be animated by pressing F", 50, 150);
		draw_text(iw, "Press G to exit this mode", 50, 175);
	}
	else if (iw->v.f_button_mode == 3)
	{
		draw_text(iw, "Select walls to be animated by pressing F", 50, 150);
		draw_text(iw, "Press G to exit this mode", 50, 175);
		if (iw->v.wall_anim != 0)
		{
			draw_text(iw, "Selected walls: ", 50, 200);
			draw_text(iw, (s = ft_itoa(iw->v.wall_anim->count_walls)),
					250, 200);
			free(s);
		}
	}
}

void	draw_some_info(t_sdl *iw)
{
	t_draw_info	d;

	if (iw->v.game_mode)
		return ;
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
	d.rect.y = 100;
	draw_text_number(iw, &d, "Story texture: ", iw->l.story);
	d.rect.y = 75;
	draw_text_number(iw, &d, "Lunar gravity: ", (int)(iw->l.accel));
	d.rect.x = 165;
	draw_text_number(iw, &d, ".", (int)(iw->l.accel * 100.0f) % 100);
	draw_some_info2(iw);
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
		set_pixel2(iw->sur, i, (int)((float)b - sq) - 20, MENU_COLOR);
		set_pixel2(iw->sur, i, (int)(sq + (float)b) + 20, MENU_COLOR);
	}
	i = -1;
	while (++i < r / 3)
	{
		set_pixel2(iw->sur, a + r + i - 2, b - 20 - i, MENU_COLOR);
		set_pixel2(iw->sur, a + r - i - 2, b - 20 - i, MENU_COLOR);
		set_pixel2(iw->sur, a - r + i + 2, b + 20 + i, MENU_COLOR);
		set_pixel2(iw->sur, a - r - i + 2, b + 20 + i, MENU_COLOR);
	}
	draw_text(iw, t, a - 6, WINDOW_H + 135);
}

void	draw_up_down_menu_arrows(t_sdl *iw, int x)
{
	int		i;

	i = WINDOW_H + 140;
	while (--i > WINDOW_H + 110)
		set_pixel2(iw->sur, x, i, MENU_COLOR);
	i = WINDOW_H + 160;
	while (++i < WINDOW_H + 190)
		set_pixel2(iw->sur, x, i, MENU_COLOR);
	i = -1;
	while (++i < 10)
	{
		set_pixel2(iw->sur, x + i, WINDOW_H + 110 + i, MENU_COLOR);
		set_pixel2(iw->sur, x - i, WINDOW_H + 110 + i, MENU_COLOR);
		set_pixel2(iw->sur, x + i, WINDOW_H + 190 - i, MENU_COLOR);
		set_pixel2(iw->sur, x - i, WINDOW_H + 190 - i, MENU_COLOR);
	}
}

void	draw_menu(t_sdl *iw)
{
	if (iw->v.game_mode)
		return ;
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

void	draw_tex_to_select(t_sdl *iw)
{
	int			i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
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
	int			i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
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
	int			i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
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
		return ;
	clear_sprites_tex_to_select(iw);
	iw->v.sprites_select_mode = 2;
	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = 100;
	rect.h = 100;
	ft_scaled_blit(iw->t_enemies[0], iw->sur, &rect);
	rect.x += 100;
	ft_scaled_blit(iw->t_enemies[8], iw->sur, &rect);
	rect.x += 100;
	ft_scaled_blit(iw->t_enemies[20], iw->sur, &rect);
}

void	draw_selected_tex(t_sdl *iw)
{
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
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
		return ;
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
		else if (iw->v.selected_sprite == 2)
			ft_scaled_blit(iw->t_enemies[20], iw->sur, &rect);
	}
}

void	draw_submenu4(t_sdl *iw)
{
	char	*s;

	if (iw->v.submenu_mode == 8)
	{
		draw_text(iw, "Set DY of walls animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 9)
	{
		draw_text(iw, "Set speed of walls animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Speed:", WINDOW_W - 450, WINDOW_H + 135);
		draw_text(iw, (s = ft_itoa(iw->v.wall_anim->speed)),
				WINDOW_W - 370, WINDOW_H + 135);
		free(s);
		draw_text(iw, "+  -  OK  Exit", WINDOW_W - 340, WINDOW_H + 135);
	}
}

void	draw_submenu3(t_sdl *iw)
{
	if (iw->v.submenu_mode == 5)
	{
		draw_text(iw, "Select walls moving type:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Same  Inv_X  Inv_Y   Exit",
				WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 7)
	{
		draw_text(iw, "Set DX of walls animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else
		draw_submenu4(iw);
}

void	draw_submenu2(t_sdl *iw)
{
	char		*s;

	if (iw->v.submenu_mode == 3)
	{
		draw_text(iw, "Set speed of sector animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Speed:", WINDOW_W - 450, WINDOW_H + 135);
		draw_text(iw, (s = ft_itoa(iw->v.sector_anim->speed)),
				WINDOW_W - 370, WINDOW_H + 135);
		free(s);
		draw_text(iw, "+  -  OK  Exit", WINDOW_W - 340, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 4)
	{
		draw_text(iw, "Choose animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Sector  Walls   Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 6)
	{
		draw_text(iw, "Select walls moving priority:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "DXDY  DYDX   Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else
		draw_submenu3(iw);
}

void	draw_submenu(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.x = WINDOW_W - 500;
	rect.y = WINDOW_H + 100;
	rect.w = 500;
	rect.h = 100;
	SDL_FillRect(iw->sur, &rect, 0x000000);
	if (iw->v.submenu_mode == 1)
	{
		draw_text(iw, "Choose type of sector animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "F  C  FC    Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 2)
	{
		draw_text(iw, "Set DZ of sector animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else
		draw_submenu2(iw);
}
