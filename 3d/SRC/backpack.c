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

void	use_item_delete(t_sdl *iw)
{
	int		i;
	int		flag;

	Mix_PlayChannel(2, iw->sounds.env[17], 0);
	flag = 0;
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
	delete_used_sprite(iw, iw->bag.selected_item);
	iw->bag.selected_item = 0;
}

void	use_item2(t_sdl *iw)
{
	if (iw->bag.selected_item->t_numb == 5)
	{
		iw->guns.bullets[1] = iw->guns.max_bullets[1];
		iw->guns.bullets_in_stock[1] = iw->guns.max_bullets_in_stock[1];
		use_item_delete(iw);
	}
	else if (iw->bag.selected_item->t_numb == 10)
	{
		iw->v.fly_mode = 1;
		Mix_HaltChannel(0);
		iw->v.fall = 1;
		iw->v.fly_down = 1;
		iw->v.fly_up = 1;
		iw->v.jetpack = clock();
		use_item_delete(iw);
	}
}

void	use_item(t_sdl *iw)
{
	if (iw->bag.selected_item == 0)
		return ;
	if (iw->bag.selected_item->t_numb == 2)
	{
		iw->p.health = 100;
		use_item_delete(iw);
	}
	else if (iw->bag.selected_item->t_numb == 3)
	{
		iw->p.health += 50;
		if (iw->p.health > 100)
			iw->p.health = 100;
		use_item_delete(iw);
	}
	else if (iw->bag.selected_item->t_numb == 4)
	{
		iw->guns.bullets[2] = iw->guns.max_bullets[2];
		iw->guns.bullets_in_stock[2] = iw->guns.max_bullets_in_stock[2];
		use_item_delete(iw);
	}
	else
		use_item2(iw);
}

void	add_item2(t_sdl *iw)
{
	if (iw->v.look_sprite->t_numb == 0)
	{
		iw->guns.bullets[2] = iw->guns.max_bullets[2];
		iw->guns.bullets_in_stock[2] = iw->guns.max_bullets_in_stock[2];
		if (iw->guns.status == 0)
		{
			iw->guns.gun_in_hands = 2;
			iw->guns.status = 3;
		}
		delete_used_sprite(iw, iw->v.look_sprite);
	}
	else
	{
		if (iw->v.look_sprite->t_numb == 6)
			iw->v.have_clocks = 1;
		iw->bag.item_in_bag1[iw->bag.count_items] = iw->v.look_sprite;
		iw->bag.count_items++;
		iw->v.look_sprite->taken = 1;
	}
}

void	add_item(t_sdl *iw)
{
	if (!(iw->v.look_sprite != 0 && iw->v.look_sprite->type == 1
		&& iw->v.look_sprite->taken == 0 &&
		iw->v.look_sprite->plen < BUTTON_PRESS_DIST))
		return ;
	Mix_PlayChannel(2, iw->sounds.env[3], 0);
	if (iw->v.look_sprite->t_numb == 1)
	{
		iw->guns.bullets[1] = iw->guns.max_bullets[1];
		iw->guns.bullets_in_stock[1] = iw->guns.max_bullets_in_stock[1];
		if (iw->guns.status == 0)
		{
			iw->guns.gun_in_hands = 1;
			iw->guns.status = 3;
			iw->hud.shell = 100;
		}
		delete_used_sprite(iw, iw->v.look_sprite);
	}
	else
		add_item2(iw);
}

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
