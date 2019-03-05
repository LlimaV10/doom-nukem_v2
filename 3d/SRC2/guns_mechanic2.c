/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guns_mechanic2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:43:27 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:43:28 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	reload_gun(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0)
	{
		iw->guns.bullets[iw->guns.gun_in_hands] =
			iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
	}
	else if (iw->guns.gun_in_hands == 1)
		reload_gun1(iw);
	else if (iw->guns.gun_in_hands == 2)
		reload_gun2(iw);
	iw->guns.prev_update_time = clock();
}

void	guns_loop2(t_sdl *iw)
{
	if (iw->guns.t == 8)
	{
		damaging_enemy(iw, 2, 10000);
		Mix_PlayChannel(3, iw->sounds.env[5], 0);
		iw->guns.t = 9;
	}
	else
	{
		if (!iw->v.left_mouse_pressed ||
			iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
		{
			iw->guns.status = 0;
			iw->guns.t = 7;
		}
		else
		{
			iw->guns.t = 8;
			iw->guns.bullets[iw->guns.gun_in_hands]--;
			iw->hud.shell = 100 * iw->guns.bullets[iw->guns.gun_in_hands] /
				iw->guns.max_bullets[iw->guns.gun_in_hands];
		}
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
	}
	iw->guns.prev_update_time = clock();
}

void	guns_loop(t_sdl *iw)
{
	if (iw->guns.status == 1 && iw->guns.gun_in_hands == 0
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		iw->guns.status = 0;
		iw->guns.t = 17;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 1
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 0;
		iw->guns.t = 0;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 2
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 10)
		guns_loop2(iw);
}

void	guns_movements2(t_sdl *iw, int t)
{
	if (iw->v.fps != 0)
		t = 150 / iw->v.fps;
	else
		t = 10;
	iw->v.weapon_change_x += iw->v.sprint *
		((iw->v.weapon_change_xdir > 0) ? t : -t);
	if (abs(iw->v.weapon_change_x) > WEAPONS_MOVING_CHANGE_VALUE)
	{
		iw->v.weapon_change_xdir *= -1;
		iw->v.weapon_change_x = ((iw->v.weapon_change_x > 0) ?
			WEAPONS_MOVING_CHANGE_VALUE : -WEAPONS_MOVING_CHANGE_VALUE);
	}
	iw->v.weapon_change_y += iw->v.sprint *
		((iw->v.weapon_change_ydir > 0) ? t : -t);
	if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE
		|| iw->v.weapon_change_y < 10)
		iw->v.weapon_change_ydir *= -1;
	if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE)
		iw->v.weapon_change_y = WEAPONS_MOVING_CHANGE_VALUE;
	if (iw->v.weapon_change_y < 10)
		iw->v.weapon_change_y = 10;
}

void	guns_movements(t_sdl *iw)
{
	if (iw->v.game_mode && (iw->v.front != 1 ||
		iw->v.back != 1 || iw->v.left != 1 || iw->v.right != 1))
		guns_movements2(iw, 0);
	if (iw->guns.status == 3)
	{
		iw->v.weapon_change_y_hide += 500 / iw->v.fps;
		if (iw->v.weapon_change_y_hide >= iw->guns.t_rect[iw->guns.t].h)
		{
			if (iw->guns.gun_in_hands == 0)
				iw->guns.t = 17;
			else if (iw->guns.gun_in_hands == 1)
				iw->guns.t = 0;
			else if (iw->guns.gun_in_hands == 2)
				iw->guns.t = 7;
			iw->guns.status = 0;
		}
	}
	else if (iw->v.weapon_change_y_hide > 0)
	{
		iw->v.weapon_change_y_hide -= 1000 / iw->v.fps;
		if (iw->v.weapon_change_y_hide < 0)
			iw->v.weapon_change_y_hide = 0;
	}
}
