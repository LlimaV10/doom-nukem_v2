/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hud4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:22:31 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:22:33 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

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
