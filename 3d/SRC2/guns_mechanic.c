/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guns_mechanic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:07:13 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 12:07:16 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	damaging_enemy(t_sdl *iw, int damage, int max_distance)
{
	if (iw->v.look_portal != 0 && iw->v.look_portal->glass != -1
		&& iw->guns.gun_in_hands != 0)
		iw->v.look_portal->glass = -1;
	if (iw->v.look_sprite == 0 || iw->v.look_sprite->type != 2
		|| iw->v.look_sprite->e.status >= 4)
		return ;
	if (iw->v.look_sprite->plen < max_distance)
		iw->v.look_sprite->e.health -= damage;
	else
		iw->hud.miss_time = clock();
}

void	attack2(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 1 && iw->guns.status == 0
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		damaging_enemy(iw, 3, 5000);
		Mix_PlayChannel(3, iw->sounds.env[6], 0);
		iw->guns.status = 1;
		iw->guns.t = 1;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[1] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[1] / iw->guns.max_bullets[1];
	}
	else if (iw->guns.gun_in_hands == 2 && iw->guns.status == 0 && clock()
		- iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 1;
		iw->guns.t = 8;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[2] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[2] / iw->guns.max_bullets[2];
	}
}

void	attack(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0 && iw->guns.status == 0 &&
		clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		damaging_enemy(iw, 3, 1000);
		iw->guns.status = 1;
		iw->guns.t = 18;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[0] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[0] / iw->guns.max_bullets[0];
	}
	else
		attack2(iw);
}

void	reload_gun1(t_sdl *iw)
{
	if (iw->guns.bullets_in_stock[1] < iw->guns.max_bullets[1])
	{
		iw->guns.gun_in_hands = 0;
		iw->guns.status = 3;
	}
	else if (iw->guns.t == 0)
	{
		Mix_PlayChannel(3, iw->sounds.env[7], 0);
		iw->guns.t = 2;
	}
	else if (iw->guns.t < 6)
		iw->guns.t++;
	else
	{
		iw->guns.bullets_in_stock[1] -= iw->guns.max_bullets[1]
			- iw->guns.bullets[1];
		iw->guns.bullets[iw->guns.gun_in_hands] =
			iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
		iw->guns.t = 0;
		iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
	}
}

void	reload_gun2(t_sdl *iw)
{
	if (iw->guns.bullets_in_stock[2] < iw->guns.max_bullets[2])
	{
		iw->guns.gun_in_hands = 0;
		iw->guns.status = 3;
	}
	else if (iw->guns.t == 7 || iw->guns.t == 8 || iw->guns.t == 9)
	{
		Mix_PlayChannel(3, iw->sounds.env[8], 0);
		iw->guns.t = 10;
	}
	else if (iw->guns.t < 16)
		iw->guns.t++;
	else
	{
		iw->guns.bullets_in_stock[2] -= iw->guns.max_bullets[2]
			- iw->guns.bullets[2];
		iw->guns.bullets[iw->guns.gun_in_hands] =
			iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
		iw->guns.t = 7;
		iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
	}
}
