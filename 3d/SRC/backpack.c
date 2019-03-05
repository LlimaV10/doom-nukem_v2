/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backpack.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:04:24 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 15:17:56 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_icon_bag(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.x = WINDOW_W / 120;
	rect.y = WINDOW_H - WINDOW_W / 12;
	rect.w = WINDOW_W / 14;
	rect.h = WINDOW_W / 14;
	if (iw->bag.selected_item != 0)
		ft_scaled_blit(iw->t_pickup[iw->bag.selected_item->t_numb],
				iw->sur, &rect);
	else
		ft_scaled_blit(iw->t_weap[17], iw->sur, &rect);
	rect.x = 0;
	rect.y = WINDOW_H - WINDOW_W / 11;
	rect.w = WINDOW_W / 11;
	rect.h = WINDOW_W / 11;
	ft_scaled_blit(iw->bag.button[0], iw->sur, &rect);
}

void	draw_frame(t_sdl *iw, SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;
	int		color;

	i = -1;
	while (++i < rect->w)
	{
		j = -1;
		while (++j < rect->h)
		{
			color = get_pixel(iw->bag.button[0], iw->bag.button[0]->w
					* i / rect->w, iw->bag.button[0]->h * j / rect->h);
			if (color != 0x010000)
				set_pixel2(winsur, rect->x + i, rect->y + j, color);
		}
	}
}

void	check_drop_clock(t_sdl *iw)
{
	if (iw->bag.selected_item->t_numb == 6)
	{
		iw->v.have_clocks = 0;
		iw->map.back = 0;
	}
}

void	drop_item(t_sdl *iw)
{
	int		i;
	int		flag;

	flag = 0;
	if (iw->bag.selected_item == 0)
		return ;
	Mix_PlayChannel(2, iw->sounds.env[4], 0);
	check_drop_clock(iw);
	i = 0;
	while (i < iw->bag.count_items)
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
	iw->bag.selected_item = 0;
}

void	delete_used_sprite(t_sdl *iw, t_sprite *s)
{
	t_sprite	head;
	t_sprite	*tmp;
	t_sprite	*tmp2;

	head.next = *iw->sprite;
	tmp = &head;
	while (tmp->next)
	{
		if (tmp->next == s)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
			break ;
		}
		tmp = tmp->next;
	}
	*iw->sprite = head.next;
}
