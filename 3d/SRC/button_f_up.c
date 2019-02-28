/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_f_up.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:28:11 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 15:29:47 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	button_f_up2(t_sdl *iw)
{
	if (iw->v.f_button_mode == 0 &&
		*(iw->v.look_picture) != 0 && (*(iw->v.look_picture))->t == 18
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST)
	{
		Mix_PlayChannel(2, iw->sounds.env[16], 0);
		(*(iw->v.look_picture))->t = 17;
	}
	else if (iw->v.f_button_mode == 0 && *(iw->v.look_picture) != 0
		&& ((*(iw->v.look_picture))->t == 19
			|| (*(iw->v.look_picture))->t == 20)
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST)
	{
		(*(iw->v.look_picture))->t =
			(((*(iw->v.look_picture))->t == 19) ? 20 : 19);
		change_sector_animation_status(iw, *(iw->v.look_picture));
		change_wall_animation_status(iw, *(iw->v.look_picture));
		Mix_PlayChannel(2, iw->sounds.env[12], 0);
	}
	else
		button_f_up_cards(iw);
}

void	button_f_up(t_sdl *iw)
{
	if (iw->v.f_button_mode == 1 && *(iw->v.look_sector) != 0)
		(*(iw->v.look_sector))->light = (t_picture *)iw->v.f_button_pointer;
	else if (iw->v.f_button_mode == 2 && *(iw->v.look_sector) != 0)
	{
		iw->v.submenu_mode = 1;
		add_sector_animation(iw);
		iw->v.f_button_mode = 0;
		iw->v.f_button_pointer = 0;
		draw_submenu(iw);
	}
	else if (iw->v.f_button_mode == 3 && *(iw->v.look_wall) != 0)
		add_wall_to_wall_animation(iw);
	else if (iw->v.f_button_mode == 0 &&
		*(iw->v.look_picture) != 0 && (*(iw->v.look_picture))->t == 17
		&& get_picture_dist(iw, *(iw->v.look_picture)) < BUTTON_PRESS_DIST)
	{
		Mix_PlayChannel(2, iw->sounds.env[16], 0);
		(*(iw->v.look_picture))->t = 18;
	}
	else
		button_f_up2(iw);
}
