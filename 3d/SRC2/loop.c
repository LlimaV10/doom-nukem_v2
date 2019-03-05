/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:07:23 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 12:11:18 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	loop1(t_sdl *iw)
{
	if (iw->guns.status != 0)
		guns_loop(iw);
	if (iw->guns.status == 2 &&
		clock() - iw->guns.prev_update_time > CLKS_P_S / 8)
		reload_gun(iw);
	guns_movements(iw);
	if (iw->v.game_mode)
		check_checkpoints(iw);
	if (iw->v.jetpack != 1 && clock() - iw->v.jetpack > JETPACK_TIME)
	{
		iw->v.jetpack = 1;
		iw->v.fly_mode = 0;
		iw->v.fall = 1;
		iw->v.fly_down = 1;
		iw->v.fly_up = 1;
	}
	if (iw->v.rot_right != 1)
	{
		iw->p.rot += (ROTATION_SPEED_PER_HALF_SEC *
			USELESS3 / (float)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot >= G360)
			iw->p.rot -= G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_right = clock();
	}
}

void	loop2(t_sdl *iw)
{
	if (iw->v.rot_left != 1)
	{
		iw->p.rot -= (ROTATION_SPEED_PER_HALF_SEC * USELESS2
			/ (float)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot < 0.0f)
			iw->p.rot += G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_left = clock();
	}
	if (iw->v.rot_up != 1 && iw->p.rotup < 2 * WINDOW_H)
	{
		iw->p.rotup += 2 * WINDOW_H * (clock() - iw->v.rot_up) / CLKS_P_S;
		iw->v.rot_up = clock();
	}
	if (iw->v.rot_down != 1 && iw->p.rotup > -2 * WINDOW_H)
	{
		iw->p.rotup -= 2 * WINDOW_H * (clock() - iw->v.rot_down) / CLKS_P_S;
		iw->v.rot_down = clock();
	}
	death(iw);
	if (iw->v.game_mode && iw->d.cs == iw->l.win_sector)
	{
		image_loop(iw, iw->hud.win);
		exit_x(iw);
	}
}

void	loop3(t_sdl *iw, clock_t t, float jsz)
{
	if (iw->v.right != 1)
		move(iw, 90, &iw->v.right);
	if (iw->v.fall != 1 && !iw->v.fly_mode)
	{
		t = clock();
		iw->p.z -= (int)(iw->l.accel * ((double)(t - iw->v.fall) /
			(double)CLKS_P_S) * 50.0f);
	}
	else if (iw->v.jump_time != 1)
	{
		jsz = (float)(clock() - iw->v.jump_time)
			/ (float)CLKS_P_S * (float)JUMP_HEIGHT *
			iw->l.accel / 10.0f;
		if ((int)jsz >= iw->v.jump)
		{
			iw->p.z += iw->v.jump;
			iw->v.jump_time = 1;
		}
		else
		{
			iw->p.z += (int)jsz;
			iw->v.jump -= (int)jsz;
		}
	}
}

void	loop4(t_sdl *iw)
{
	if (iw->v.fall == 1 && iw->v.jump_time == 1
		&& (iw->p.z - iw->v.plrzd) > PLAYER_HEIGHT && !iw->v.fly_mode)
		iw->v.fall = clock();
	if (iw->v.fly_up != 1)
	{
		iw->p.z += (int)(FLY_SPEED * USELESS1 / (float)CLKS_P_S);
		iw->v.fly_up = clock();
	}
	if (iw->v.fly_down != 1)
	{
		iw->p.z -= (int)(FLY_SPEED * (float)(clock()
			- iw->v.fly_down) / (float)CLKS_P_S);
		iw->v.fly_down = clock();
	}
	if (iw->sectors[iw->d.cs].cl.t >= 0 && iw->p.z +
		PLAYER_HEAD_SIZE - iw->v.crouch > iw->v.plrzu && iw->v.fly_mode != 2)
		iw->p.z = iw->v.plrzu - PLAYER_HEAD_SIZE + iw->v.crouch;
	else if (iw->p.z - iw->v.plrzd < PLAYER_HEIGHT && iw->v.fly_mode != 2)
	{
		iw->p.z = iw->v.plrzd + PLAYER_HEIGHT;
		iw->v.fall = 1;
	}
}

void	loop6(t_sdl *iw)
{
	if (iw->v.fly_mode == 2)
	{
		if (iw->v.front != 1)
			move(iw, 0, &iw->v.front);
		if (iw->v.back != 1)
			move(iw, 180, &iw->v.back);
		if (iw->v.left != 1)
			move(iw, 270, &iw->v.left);
		if (iw->v.right != 1)
			move(iw, 90, &iw->v.right);
	}
	else
		iw->v.fall = 1;
}

void	loop5(t_sdl *iw)
{
	if (iw->d.cs >= 0)
	{
		iw->v.plrzu = get_ceil_z(iw, iw->p.x, iw->p.y);
		iw->v.plrzd = get_floor_z(iw, iw->p.x, iw->p.y);
		if (iw->v.crouch != 0 && iw->v.crouch_pressed == 0
			&& iw->v.plrzu - iw->v.plrzd >= PLAYER_HEIGHT + PLAYER_HEAD_SIZE)
			iw->v.crouch = 0;
		if (iw->v.front != 1)
			move(iw, 0, &iw->v.front);
		if (iw->v.back != 1)
			move(iw, 180, &iw->v.back);
		if (iw->v.left != 1)
			move(iw, 270, &iw->v.left);
		loop3(iw, 0, 0.0f);
		loop4(iw);
	}
	else
		loop6(iw);
}

void	loop(t_sdl *iw)
{
	if ((double)(clock() - iw->loop_update_time)
		< (double)CLKS_P_S / (double)MAX_FPS)
		return ;
	sound_loop(iw);
	environment_loop(iw);
	if (iw->v.left_mouse_pressed && iw->v.mouse_mode == 1)
		attack(iw);
	loop1(iw);
	loop2(iw);
	loop5(iw);
	do_sector_animations(iw);
	do_wall_animations(iw);
	if (iw->v.fly_mode != 2)
		check_walls_collisions(iw);
	if (iw->v.game_mode)
		check_enemies(iw);
	update(iw);
	iw->v.fps = (int)((float)CLKS_P_S /
		(float)(clock() - iw->loop_update_time));
	iw->loop_update_time = clock();
}
