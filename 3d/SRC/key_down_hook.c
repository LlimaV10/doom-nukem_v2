/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_down_hook.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 17:13:30 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 17:17:08 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	key_down4(int code, t_sdl *iw)
{
	if (code == 224 && iw->v.fly_mode == 0)
	{
		iw->v.crouch = CROUCH_SIZE;
		iw->v.crouch_pressed = 1;
		iw->v.sprint = 1;
	}
}

void	key_down3(int code, t_sdl *iw)
{
	if (code == 224 && iw->v.fly_mode)
		iw->v.fly_down = clock();
	else if (code == 21 && iw->v.game_mode && iw->guns.status == 0
		&& iw->guns.bullets[iw->guns.gun_in_hands] <
		iw->guns.max_bullets[iw->guns.gun_in_hands])
		iw->guns.status = 2;
	else if (code == 43 && iw->v.game_mode)
	{
		SDL_SetRelativeMouseMode(iw->bag.bag);
		iw->v.mouse_mode = iw->bag.bag;
		iw->bag.bag = ((iw->bag.bag == 1) ? 0 : 1);
	}
	else if (code == 9 && iw->v.game_mode)
		add_item(iw);
	else if (code == 8 && iw->v.game_mode && iw->bag.selected_item != 0)
		use_item(iw);
	else if (code == 20 && iw->v.game_mode)
		drop_item(iw);
	else if (code == 57 && iw->v.have_clocks)
		iw->map.back = ((iw->map.back == 1) ? 0 : 1);
	else if (code == 225 && iw->v.crouch == 0)
		iw->v.sprint = 2;
	else
		key_down4(code, iw);
}

void	key_down2(int code, t_sdl *iw)
{
	if (code == 18)
		iw->v.kernel = ((iw->v.kernel == 1) ? 0 : 1);
	else if (code == 43 && !iw->v.game_mode)
	{
		iw->v.mouse_mode = ((iw->v.mouse_mode == 1) ? 0 : 1);
		SDL_SetRelativeMouseMode(iw->v.mouse_mode);
	}
	else if (code == 55 && *(iw->v.look_sector) != 0
			&& iw->v.mouse_mode == 1 && !iw->v.game_mode)
		(*(iw->v.look_sector))->cl.t = iw->v.tex_to_fill;
	else if (code == 56 && *(iw->v.look_sector) != 0
			&& iw->v.mouse_mode == 1 && !iw->v.game_mode)
		(*(iw->v.look_sector))->fr.t = iw->v.tex_to_fill;
	else if (code == 54 && *(iw->v.look_sector) != 0
			&& iw->v.mouse_mode == 1 && !iw->v.game_mode)
		(*(iw->v.look_sector))->cl.t = -1;
	else if (code == 16 && iw->v.mouse_mode == 1 && *(iw->v.look_wall) != 0
		&& (*iw->v.look_wall)->nextsector == -1 && !iw->v.game_mode)
		(*(iw->v.look_wall))->t = -1;
	else if (code == 48 && iw->v.mouse_mode == 1)
		iw->v.sprite_editing = 1;
	else if (code == 44 && iw->v.fly_mode)
		iw->v.fly_up = clock();
	else
		key_down3(code, iw);
}

void	key_down(int code, t_sdl *iw)
{
	if (code == 79)
		iw->v.rot_right = clock();
	else if (code == 80)
		iw->v.rot_left = clock();
	else if (code == 26)
		iw->v.front = clock();
	else if (code == 22)
		iw->v.back = clock();
	else if (code == 4)
		iw->v.left = clock();
	else if (code == 7)
		iw->v.right = clock();
	else if (code == 82)
		iw->v.rot_up = clock();
	else if (code == 81)
		iw->v.rot_down = clock();
	else if (code == 47 && *(iw->v.look_picture) != 0 &&
		*(iw->v.look_wall) != 0 && !iw->v.game_mode)
	{
		iw->v.picture_changing = *(iw->v.look_picture);
		iw->v.wall_picture_changing = *(iw->v.look_wall);
	}
	else
		key_down2(code, iw);
}

void	key_down_repeat(int code, t_sdl *iw)
{
	if (code == 44 && iw->v.jump_time == 1
			&& iw->v.fall == 1 && !iw->v.fly_mode)
	{
		iw->v.jump_time = clock();
		iw->v.jump = JUMP_HEIGHT;
		Mix_PlayChannel(0, iw->sounds.env[1], 0);
	}
	else if (code == 87 && !iw->v.game_mode && iw->l.accel < 9.9f)
		iw->l.accel += 0.1f;
	else if (code == 86 && !iw->v.game_mode && iw->l.accel > 0.2f)
		iw->l.accel -= 0.1f;
}
