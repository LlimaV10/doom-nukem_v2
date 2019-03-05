/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_left_up3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:28:29 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:28:30 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	mouse_buttonleft_up1_8(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 5 && x > WINDOW_W - 450)
	{
		if (x < WINDOW_W - 380)
		{
			iw->v.wall_anim->moving_type = 0;
			iw->v.submenu_mode = 6;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 305)
		{
			iw->v.wall_anim->moving_type = 1;
			iw->v.submenu_mode = 6;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 225)
		{
			iw->v.wall_anim->moving_type = 2;
			iw->v.submenu_mode = 6;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 165)
			exit_editing_wall_animation(iw);
	}
	else
		mouse_buttonleft_up1_9(x, iw);
}

void	mouse_buttonleft_up1_7(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 4 && x > WINDOW_W - 450)
	{
		if (x < WINDOW_W - 375)
		{
			iw->v.f_button_mode = 2;
			iw->v.submenu_mode = 0;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 300)
		{
			iw->v.f_button_mode = 3;
			iw->v.submenu_mode = 0;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 240)
		{
			iw->v.submenu_mode = 0;
			iw->v.f_button_pointer = 0;
			draw_submenu(iw);
		}
	}
	else
		mouse_buttonleft_up1_8(x, iw);
}

void	mouse_buttonleft_up1_6_1(int x, t_sdl *iw)
{
	if (x < WINDOW_W - 245)
	{
		do_sector_animation_step(iw,
				iw->v.sector_anim, -iw->v.sector_anim->dy);
		iw->v.sector_anim->next = iw->sector_animations;
		iw->v.submenu_mode = 0;
		draw_submenu(iw);
		iw->v.sector_anim->prev_clock = clock();
		iw->sector_animations = iw->v.sector_anim;
		iw->v.sector_anim = 0;
	}
	else if (x < WINDOW_W - 200)
		exit_editing_sector_animation(iw);
}

void	mouse_buttonleft_up1_6(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 3 && x > WINDOW_W - 340)
	{
		if (x < WINDOW_W - 320)
		{
			iw->v.sector_anim->speed +=
				((iw->v.sector_anim->speed < 9) ? 1 : 0);
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 295)
		{
			iw->v.sector_anim->speed -=
				((iw->v.sector_anim->speed > 1) ? 1 : 0);
			draw_submenu(iw);
		}
		else
			mouse_buttonleft_up1_6_1(x, iw);
	}
	else
		mouse_buttonleft_up1_7(x, iw);
}

void	mouse_buttonleft_up1_5(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 2 && x > WINDOW_W - 450)
	{
		if (x < WINDOW_W - 390)
		{
			do_sector_animation_step(iw, iw->v.sector_anim, 100);
			iw->v.sector_anim->dy += 100;
		}
		else if (x < WINDOW_W - 330)
		{
			do_sector_animation_step(iw, iw->v.sector_anim, -100);
			iw->v.sector_anim->dy -= 100;
		}
		else if (x < WINDOW_W - 280)
		{
			iw->v.submenu_mode = 3;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 220)
			exit_editing_sector_animation(iw);
	}
	else
		mouse_buttonleft_up1_6(x, iw);
}
