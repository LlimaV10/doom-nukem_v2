/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   using_cards.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 17:49:22 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/01 17:49:48 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int			get_picture_dist(t_sdl *iw, t_picture *pic)
{
	return ((int)sqrtf(powf((pic->x0 + pic->x1) / 2 - iw->p.x, 2.0f) +
		powf((pic->y0 + pic->y1) / 2 - iw->p.y, 2.0f) +
		powf((pic->zu + pic->zd) / 2 - iw->p.z, 2.0f)));
}

t_sprite	*get_card_from_bag(t_sdl *iw, int t_numb)
{
	int		i;

	i = -1;
	while (++i < iw->bag.count_items)
		if (iw->bag.item_in_bag1[i]->t_numb == t_numb)
			return (iw->bag.item_in_bag1[i]);
	return (0);
}

void		button_f_up_cards2(t_sdl *iw, t_sprite *tmp)
{
	int		i;

	i = 1;
	if (iw->v.f_button_mode == 0 && *(iw->v.look_picture) != 0
		&& (*iw->v.look_picture)->t == 23
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST
		&& (tmp = get_card_from_bag(iw, 7)) != 0)
		i = 0;
	else if (iw->v.f_button_mode == 0 && *(iw->v.look_picture) != 0
		&& (*iw->v.look_picture)->t == 24
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST
		&& (tmp = get_card_from_bag(iw, 8)) != 0)
		i = 0;
	if (i)
		return ;
	(*iw->v.look_picture)->t = 21;
	iw->bag.selected_item = tmp;
	use_item_delete(iw);
	change_sector_animation_status(iw, *(iw->v.look_picture));
	change_wall_animation_status(iw, *(iw->v.look_picture));
	delete_used_sprite(iw, tmp);
	Mix_PlayChannel(1, iw->sounds.env[2], 0);
}

void		button_f_up_cards(t_sdl *iw)
{
	t_sprite	*tmp;

	if (iw->v.f_button_mode == 0 && *(iw->v.look_picture) != 0
		&& (*iw->v.look_picture)->t == 22
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST
		&& (tmp = get_card_from_bag(iw, 9)) != 0)
	{
		(*iw->v.look_picture)->t = 21;
		iw->bag.selected_item = tmp;
		use_item_delete(iw);
		change_sector_animation_status(iw, *(iw->v.look_picture));
		change_wall_animation_status(iw, *(iw->v.look_picture));
		delete_used_sprite(iw, tmp);
		Mix_PlayChannel(1, iw->sounds.env[2], 0);
	}
	else
		button_f_up_cards2(iw, 0);
}
