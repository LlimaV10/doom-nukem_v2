/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:46:03 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 16:48:25 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void			drawcircle(t_hud *den, t_draw_circle *c, int x, int y)
{
	den->koord += 4;
	den->mas[den->i].x = c->xc - y;
	den->mas[den->i].y = c->yc - x;
	den->mas[150 - den->i].x = c->xc - x;
	den->mas[150 - den->i].y = c->yc - y;
	den->mas[den->i + 151].x = c->xc + x;
	den->mas[den->i + 151].y = c->yc - y;
	den->mas[300 - den->i].x = c->xc + y;
	den->mas[300 - den->i].y = c->yc - x;
	den->mas2[den->i].x = c->xc - y;
	den->mas2[den->i].y = c->yc + x;
	den->mas2[150 - den->i].x = c->xc - x;
	den->mas2[150 - den->i].y = c->yc + y;
	den->mas2[den->i + 150].x = c->xc + x;
	den->mas2[den->i + 150].y = c->yc + y;
	den->mas2[300 - den->i].x = c->xc + y;
	den->mas2[300 - den->i].y = c->yc + x;
	den->i += 1;
}

t_draw_circle	circle2(t_hud *den, int xc, int yc)
{
	t_draw_circle	c;

	c.xc = xc;
	c.yc = yc;
	den->koord = 0;
	den->i = 0;
	den->rect.x = FOOTX - den->rad + 20;
	den->rect.y = FOOTY - den->rad + 2;
	den->rect.h = den->rad * 2 - 5;
	den->rect.w = den->rad * 2 - 5;
	return (c);
}

void			circle(t_hud *den, int xc, int yc)
{
	int				r;
	int				x;
	int				y;
	int				d;
	t_draw_circle	c;

	c = circle2(den, xc, yc);
	r = den->rad;
	x = 0;
	y = r;
	d = 3 - 2 * r;
	drawcircle(den, &c, x, y);
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
		drawcircle(den, &c, x, y);
	}
}

void			ft_line2(t_sdl *iw, int x, int y, int color)
{
	int		i;

	i = x - 1;
	while (++i < x + 30)
		set_pixel2(iw->sur, i, y, color);
}

void			make_health(t_hud *den, t_sdl *iw)
{
	int		i;
	char	*s;

	i = -1;
	while (++i < (3 * iw->p.health))
		ft_line2(iw, den->mas[i].x, den->mas[i].y, 0xAA0000);
	i = -1;
	while (++i < (3 * den->shell))
		ft_line2(iw, den->mas2[i].x, den->mas2[i].y, 0x0000AA);
	ft_scaled_blit(den->enot, iw->sur, &den->rect);
	draw_text_blue(iw, "/", WINDOW_W - 50, 185);
	if (iw->guns.bullets_in_stock[iw->guns.gun_in_hands] > 0)
	{
		draw_text_blue(iw, (s = ft_itoa(iw->guns.bullets_in_stock[
			iw->guns.gun_in_hands])), WINDOW_W - 40, 185);
		free(s);
	}
}

void			draw_miss(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.w = WINDOW_W / 10;
	rect.h = rect.w * iw->hud.miss->h / iw->hud.miss->w;
	rect.x = WINDOW_W / 2 - rect.w / 2;
	rect.y = WINDOW_H / 3 - WINDOW_H / 10 *
		(clock() - iw->hud.miss_time) / CLKS_P_S;
	ft_scaled_blit(iw->hud.miss, iw->sur, &rect);
}

void			draw_save(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.w = WINDOW_W / 4;
	rect.h = rect.w * iw->hud.saved->h / iw->hud.saved->w;
	rect.x = WINDOW_W / 2 - rect.w / 2;
	rect.y = 20 + WINDOW_H / 20 * (clock() - iw->hud.saved_time) / CLKS_P_S;
	ft_scaled_blit(iw->hud.saved, iw->sur, &rect);
}
