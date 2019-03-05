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
