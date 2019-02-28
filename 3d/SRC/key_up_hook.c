/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_up_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 17:54:06 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 17:55:05 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	key_up4(int code, t_sdl *iw)
{
	if (code == 30 && iw->guns.status == 0 &&
			iw->guns.gun_in_hands != 1 && iw->guns.bullets[1] > 0)
	{
		iw->guns.status = 3;
		iw->guns.gun_in_hands = 1;
		iw->hud.shell = 100 * iw->guns.bullets[1] / iw->guns.max_bullets[1];
	}
	else if (code == 85 && !iw->v.game_mode)
		iw->l.skybox = iw->v.tex_to_fill;
	else if (code == 84 && !iw->v.game_mode)
		iw->l.story = iw->v.tex_to_fill;
	else if (code == 225)
		iw->v.sprint = 1;
	else if (code == 224)
		iw->v.crouch_pressed = 0;
}

void	key_up3(int code, t_sdl *iw)
{
	if (code == 44 && iw->v.fly_mode)
		iw->v.fly_up = 1;
	else if (code == 224 && iw->v.fly_mode)
		iw->v.fly_down = 1;
	else if (code == 6 && *iw->v.look_wall != 0 &&
		(*iw->v.look_wall)->t >= 0 && (*iw->v.look_wall)->t < TEXTURES_COUNT)
		iw->v.tex_to_fill = (*iw->v.look_wall)->t;
	else if (code == 32 && iw->guns.status == 0 && iw->guns.gun_in_hands != 0)
	{
		iw->guns.status = 3;
		iw->guns.gun_in_hands = 0;
		iw->hud.shell = 100 * iw->guns.bullets[0] / iw->guns.max_bullets[0];
	}
	else if (code == 31 && iw->guns.status == 0 &&
			iw->guns.gun_in_hands != 2 && iw->guns.bullets[2] > 0)
	{
		iw->guns.status = 3;
		iw->guns.gun_in_hands = 2;
		iw->hud.shell = 100 * iw->guns.bullets[2] / iw->guns.max_bullets[2];
	}
	else
		key_up4(code, iw);
}

void	key_up2(int code, t_sdl *iw)
{
	if (code == 9)
		button_f_up(iw);
	else if (code == 10)
	{
		if (iw->v.f_button_mode == 3 && iw->v.wall_anim != 0)
		{
			iw->v.submenu_mode = 5;
			draw_submenu(iw);
		}
		iw->v.f_button_mode = 0;
		iw->v.f_button_pointer = 0;
	}
	else if (code == 48)
		iw->v.sprite_editing = 0;
	else if (code == 25 && !iw->v.game_mode)
	{
		iw->v.fly_mode = ((iw->v.fly_mode == 2) ? 0 : iw->v.fly_mode + 1);
		Mix_HaltChannel(0);
		iw->v.fall = 1;
		iw->v.fly_down = 1;
		iw->v.fly_up = 1;
	}
	else
		key_up3(code, iw);
}

void	key_up(int code, t_sdl *iw)
{
	if (code == 41)
		iw->quit = 1;
	else if (code == 79)
		iw->v.rot_right = 1;
	else if (code == 80)
		iw->v.rot_left = 1;
	else if (code == 26)
		iw->v.front = 1;
	else if (code == 22)
		iw->v.back = 1;
	else if (code == 4)
		iw->v.left = 1;
	else if (code == 7)
		iw->v.right = 1;
	else if (code == 82)
		iw->v.rot_up = 1;
	else if (code == 81)
		iw->v.rot_down = 1;
	else if (code == 47)
		iw->v.picture_changing = 0;
	else if (code == 19 && iw->v.mouse_mode == 1 &&
		*(iw->v.look_wall) != 0 && !iw->v.game_mode)
		add_picture(iw, *(iw->v.look_wall));
	else
		key_up2(code, iw);
}
