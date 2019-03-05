/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backpack3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:18:17 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:18:17 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

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
