/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:34:53 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/20 13:34:54 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	blouk_player(t_doom *den)
{
	char		*cmp;
	SDL_Color	color_text;

	clean_string(den);
	//den->font = TTF_OpenFont("frameworks/ARIAL.TTF", 20);
	color_text.r = 255;
	color_text.g = 0;
	color_text.b = 0;
	cmp = "please put a player\n";
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, cmp,
		color_text, 200);
	den->rect.x = 1770;
	den->rect.y = 380;
	den->rect.h = 50;
	den->rect.w = 30;
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	//TTF_CloseFont(den->font);
	SDL_UpdateWindowSurface(den->window);
}

void	player(t_doom *den)
{
	den->cursor = SDL_CreateColorCursor(den->icon[7], 0, 0);
	SDL_SetCursor(den->cursor);
	den->blockwall = 1;
}

void	rotate_image_help(t_doom *den, t_rotate *rot)
{
	rot->hwd = den->player.x * XSKAPE;
	rot->hhd = den->player.y * YSKAPE;
	rot->hws = den->icon[9]->w / 2;
	rot->hhs = den->icon[9]->h / 2;
	rot->a = den->angle / 180.0 * M_PI;
	rot->r = sqrt(rot->hws * rot->hws + rot->hhs * rot->hhs);
	rot->b = atan2(1.0 * rot->hhs, rot->hws);
	rot->x = -1 - rot->hwd;
}

void	rotate_image(t_doom *den)
{
	t_rotate	rot;

	rotate_image_help(den, &rot);
	if (den->player.x * XSKAPE < den->border && den->player.y * YSKAPE <
			WIDTH && den->player.x * XSKAPE > 0 && den->player.y * YSKAPE > 0)
	{
		while (++rot.x < rot.hwd)
		{
			rot.y = -1 - rot.hhd;
			while (++rot.y < rot.hhd)
			{
				rot.i = rot.x * cos(rot.a) - rot.y * sin(rot.a) +
														rot.r * cos(rot.b);
				rot.j = rot.x * sin(rot.a) + rot.y * cos(rot.a) +
														rot.r * sin(rot.b);
				if (rot.i < den->icon[9]->w && rot.i >= 0 && rot.j <
												den->icon[9]->h && rot.j >= 0)
					set_pixel(den->bmp, rot.x + rot.hwd, rot.y +
							rot.hhd, get_pixel2(den->icon[9], rot.i, rot.j));
			}
		}
	}
}

void	change_way(t_doom *den, int i, t_col *vec)
{
	if (i == 1)
	{
		den->angle -= 10;
		if (den->angle < 0)
			den->angle = 360;
		den->player.introt += 10;
		if (den->player.introt > 359)
			den->player.introt = 0;
	}
	retry_write(den, vec);
}
