/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backpack2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:18:07 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:18:14 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_butttons(t_sdl *iw, SDL_Rect *rect)
{
	rect->w = ((((WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5)))
	/ 3) < ((WINDOW_H - (WINDOW_H / 5)) - (WINDOW_H - (WINDOW_H / 2.5)))) ?
	(((WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5))) / 3) :
	((WINDOW_H - (WINDOW_H / 5)) - (WINDOW_H - (WINDOW_H / 2.5)));
	rect->h = rect->w;
	rect->x = (WINDOW_W - (WINDOW_W / 2.5));
	rect->y = (WINDOW_H - (WINDOW_H / 2.6));
	ft_scaled_blit(iw->bag.button[2], iw->sur, rect);
	if (iw->bag.click_x >= rect->x && iw->bag.click_x <= rect->x + rect->w &&
			iw->bag.click_y >= rect->y && iw->bag.click_y <=
			rect->y + rect->h && iw->v.mouse_mode == 0)
		use_item(iw);
	rect->x = WINDOW_W - (WINDOW_W / 2.5) + rect->w * 2;
	ft_scaled_blit(iw->bag.button[1], iw->sur, rect);
	if (iw->bag.click_x >= rect->x && iw->bag.click_x <= rect->x + rect->w &&
			iw->bag.click_y >= rect->y && iw->bag.click_y <=
			rect->y + rect->h && iw->v.mouse_mode == 0)
		drop_item(iw);
}

void	draw_selected_item(t_packaging_texture *tex, SDL_Surface *winsur,
							SDL_Rect *rect, t_sdl *iw)
{
	int		i;
	int		j;
	int		color;

	i = -1;
	rect->w = (WINDOW_W - (WINDOW_W / 4.5)) - (WINDOW_W - (WINDOW_W / 2.5));
	rect->h = (WINDOW_H - (WINDOW_H / 4.5)) - (WINDOW_H / 2.5);
	while (++i < (WINDOW_W - (WINDOW_W / 4.5))
		- (WINDOW_W - (WINDOW_W / 2.5)) - 1)
	{
		j = -1;
		while (++j < (WINDOW_H - (WINDOW_H / 4.5)) - (WINDOW_H / 2.5) - 1)
		{
			color = get_pixel(tex, tex->w * i / rect->w, tex->h * j / rect->h);
			if (color != 0xff00e3 && color != 0x010000)
				set_pixel2(winsur, WINDOW_W - (WINDOW_W / 2.5) + i,
					rect->y + j, color);
		}
	}
	draw_butttons(iw, rect);
}

int		draw_item(t_sdl *iw, SDL_Rect *rect, int i)
{
	if (rect->x + iw->bag.indent + rect->w > WINDOW_W - (WINDOW_W / 2.5))
	{
		rect->y += rect->h + iw->bag.indent;
		rect->x = (WINDOW_W / 4.5);
		if (rect->y + rect->h >= WINDOW_H - (WINDOW_H / 4.5))
			return (0);
	}
	ft_scaled_blit(iw->t_pickup[iw->bag.item_in_bag1[i]->t_numb],
		iw->sur, rect);
	if (iw->bag.click_x >= rect->x && iw->bag.click_x <= rect->x + rect->w &&
		iw->bag.click_y >= rect->y && iw->bag.click_y <= rect->y + rect->h &&
		iw->v.mouse_mode == 0)
	{
		iw->bag.selected_item = iw->bag.item_in_bag1[i];
		draw_frame(iw, iw->sur, rect);
	}
	rect->x += rect->w + iw->bag.indent;
	return (1);
}

void	draw_items(t_sdl *iw)
{
	int			i;
	int			w_transparency;
	SDL_Rect	rect;

	w_transparency = (WINDOW_W - (WINDOW_W / 2.5)) - (WINDOW_W / 4.5);
	rect.w = w_transparency * 0.15;
	rect.h = w_transparency * 0.15;
	iw->bag.num_item_in_line = w_transparency / rect.w - 1;
	iw->bag.indent = (w_transparency - (iw->bag.num_item_in_line * rect.w))
		/ iw->bag.num_item_in_line;
	(iw->bag.indent < 1) ? iw->bag.indent++ : 0;
	rect.x = (WINDOW_W / 4.5);
	rect.y = (WINDOW_H / 4.5);
	i = -1;
	while (++i < iw->bag.count_items && rect.x <= WINDOW_W - (WINDOW_W / 2.5))
		if (draw_item(iw, &rect, i) == 0)
			break ;
	rect.y = (WINDOW_H / 4.5);
	if (iw->bag.selected_item != 0)
		draw_selected_item(iw->t_pickup[iw->bag.selected_item->t_numb],
			iw->sur, &rect, iw);
}
