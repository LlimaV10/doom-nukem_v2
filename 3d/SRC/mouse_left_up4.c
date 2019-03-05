/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_left_up4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:28:35 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:28:36 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	mouse_buttonleft_up1_4(int x, t_sdl *iw)
{
	if (iw->v.submenu_mode == 1 && x > WINDOW_W - 450)
	{
		if (x < WINDOW_W - 430)
		{
			iw->v.sector_anim->type = 1;
			iw->v.submenu_mode = 2;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 395)
		{
			iw->v.sector_anim->type = 2;
			iw->v.submenu_mode = 2;
			draw_submenu(iw);
		}
		else if (x < WINDOW_W - 355)
		{
			iw->v.sector_anim->type = 0;
			iw->v.submenu_mode = 2;
			draw_submenu(iw);
		}
		else if (x > WINDOW_W - 335 && x < WINDOW_W - 280)
			exit_editing_sector_animation(iw);
	}
	else
		mouse_buttonleft_up1_5(x, iw);
}

void	mouse_buttonleft_up1_3(int x, int y, t_sdl *iw)
{
	if (x < 230)
	{
		if (iw->v.changing_fc)
		{
			free((*(iw->v.look_sector))->cl.n);
			(*(iw->v.look_sector))->cl.n = 0;
		}
		else
		{
			free((*(iw->v.look_sector))->fr.n);
			(*(iw->v.look_sector))->fr.n = 0;
		}
	}
	else if (x > 332 && x < 430)
	{
		if (y < WINDOW_H + 135)
			draw_decor_tex_to_select(iw);
		else if (y < WINDOW_H + 170)
			draw_pickup_tex_to_select(iw);
		else
			draw_enemies_tex_to_select(iw);
	}
	else
		mouse_buttonleft_up1_4(x, iw);
}

void	mouse_buttonleft_up1_2(int x, int y, t_sdl *iw)
{
	if (x < 210)
	{
		if (y < WINDOW_H + 150)
		{
			if (iw->v.changing_fc)
				rotate_fc(&(*(iw->v.look_sector))->cl, 1, 1);
			else
				rotate_fc(&(*(iw->v.look_sector))->fr, 1, 1);
		}
		else
		{
			if (iw->v.changing_fc)
				rotate_fc(&(*(iw->v.look_sector))->cl, 1, -1);
			else
				rotate_fc(&(*(iw->v.look_sector))->fr, 1, -1);
		}
	}
	else
		mouse_buttonleft_up1_3(x, y, iw);
}

void	mouse_buttonleft_up1_1(int y, t_sdl *iw)
{
	if (y < WINDOW_H + 150)
	{
		if (iw->v.changing_fc)
			(*(iw->v.look_sector))->cl.z += 100;
		else if ((*(iw->v.look_sector))->cl.z -
				(*(iw->v.look_sector))->fr.z > 200)
			(*(iw->v.look_sector))->fr.z += 100;
	}
	else if (!iw->v.changing_fc)
		(*(iw->v.look_sector))->fr.z -= 100;
	else if ((*(iw->v.look_sector))->cl.z
			- (*(iw->v.look_sector))->fr.z > 200)
		(*(iw->v.look_sector))->cl.z -= 100;
	if (iw->v.changing_fc && (*(iw->v.look_sector))->cl.n != 0)
		(*(iw->v.look_sector))->cl.n->d = (*(iw->v.look_sector))->
			cl.n->a * (*(iw->v.look_sector))->cl.x -
		(*(iw->v.look_sector))->cl.n->b * (*(iw->v.look_sector))->cl.y -
		(*(iw->v.look_sector))->cl.n->c * (*(iw->v.look_sector))->cl.z;
	else if (!iw->v.changing_fc && (*(iw->v.look_sector))->fr.n != 0)
		(*(iw->v.look_sector))->fr.n->d = (*(iw->v.look_sector))->
			fr.n->a * (*(iw->v.look_sector))->fr.x -
		(*(iw->v.look_sector))->fr.n->b * (*(iw->v.look_sector))->fr.y -
		(*(iw->v.look_sector))->fr.n->c * (*(iw->v.look_sector))->fr.z;
}

void	mouse_buttonleft_up1(int x, int y, t_sdl *iw)
{
	if (x < 40)
	{
		iw->v.changing_fc = ((iw->v.changing_fc) ? 0 : 1);
		SDL_FillRect(iw->sur, &iw->v.chang_fc_rect, 0x000000);
		if (iw->v.changing_fc)
			draw_text(iw, "C", 15, WINDOW_H + 135);
		else
			draw_text(iw, "F", 15, WINDOW_H + 135);
	}
	else if (x < 70)
		mouse_buttonleft_up1_1(y, iw);
	else if (x < 140)
	{
		if (y < WINDOW_H + 150)
			rotate_fc(((iw->v.changing_fc) ? &(*(iw->v.look_sector))->cl
				: &(*(iw->v.look_sector))->fr), 0, 1);
		else if (iw->v.changing_fc)
			rotate_fc(&(*(iw->v.look_sector))->cl, 0, -1);
		else
			rotate_fc(&(*(iw->v.look_sector))->fr, 0, -1);
	}
	else
		mouse_buttonleft_up1_2(x, y, iw);
}
