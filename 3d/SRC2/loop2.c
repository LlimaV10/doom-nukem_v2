/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:42:02 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:42:03 by dbolilyi         ###   ########.fr       */
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
