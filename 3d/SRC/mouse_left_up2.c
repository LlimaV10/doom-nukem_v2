/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_left_up2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:28:24 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:28:25 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	mouse_buttonleft_up1_12_1(int x, t_sdl *iw)
{
	if (x < WINDOW_W - 245)
	{
		do_wall_animation_step_dx(iw,
				iw->v.wall_anim, -iw->v.wall_anim->dx);
		do_wall_animation_step_dy(iw,
				iw->v.wall_anim, -iw->v.wall_anim->dy);
		iw->v.wall_anim->next = iw->wall_animations;
		iw->v.submenu_mode = 0;
		draw_submenu(iw);
		iw->v.wall_anim->prev_clock = clock();
		iw->wall_animations = iw->v.wall_anim;
		iw->v.wall_anim = 0;
	}
	else if (x < WINDOW_W - 200)
		exit_editing_wall_animation(iw);
}

void	mouse_buttonleft_up1_12(int x, t_sdl *iw)
{
	if (x < WINDOW_W - 320)
	{
		iw->v.wall_anim->speed +=
			((iw->v.wall_anim->speed < 9) ? 1 : 0);
		draw_submenu(iw);
	}
	else if (x < WINDOW_W - 295)
	{
		iw->v.wall_anim->speed -=
			((iw->v.wall_anim->speed > 1) ? 1 : 0);
		draw_submenu(iw);
	}
	else
		mouse_buttonleft_up1_12_1(x, iw);
}

void	mouse_buttonleft_up1_11(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 8 && x > WINDOW_W - 450)
	{
		if (x < WINDOW_W - 390)
		{
			iw->v.wall_anim->dy += 100;
			do_wall_animation_step_dy(iw, iw->v.wall_anim, 100);
		}
		else if (x < WINDOW_W - 330)
		{
			iw->v.wall_anim->dy -= 100;
			do_wall_animation_step_dy(iw, iw->v.wall_anim, -100);
		}
		else if (x < WINDOW_W - 280)
		{
			iw->v.submenu_mode = 9;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 220)
			exit_editing_wall_animation(iw);
	}
	else if (iw->v.submenu_mode == 9 && x > WINDOW_W - 340)
		mouse_buttonleft_up1_12(x, iw);
}

void	mouse_buttonleft_up1_10(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 7 && x > WINDOW_W - 450)
	{
		if (x < WINDOW_W - 390)
		{
			iw->v.wall_anim->dx += 100;
			do_wall_animation_step_dx(iw, iw->v.wall_anim, 100);
		}
		else if (x < WINDOW_W - 330)
		{
			iw->v.wall_anim->dx -= 100;
			do_wall_animation_step_dx(iw, iw->v.wall_anim, -100);
		}
		else if (x < WINDOW_W - 280)
		{
			iw->v.submenu_mode = 8;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 220)
			exit_editing_wall_animation(iw);
	}
	else
		mouse_buttonleft_up1_11(x, iw);
}

void	mouse_buttonleft_up1_9(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 6 && x > WINDOW_W - 450)
	{
		if (x < WINDOW_W - 380)
		{
			iw->v.wall_anim->priority = 0;
			iw->v.submenu_mode = 7;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 300)
		{
			iw->v.wall_anim->priority = 1;
			iw->v.submenu_mode = 7;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 225)
			exit_editing_wall_animation(iw);
	}
	else
		mouse_buttonleft_up1_10(x, iw);
}
