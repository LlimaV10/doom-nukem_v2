/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move_right_wheel.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:02:07 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/01 15:08:20 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	mouse_move1(int xrel, int yrel, t_sdl *iw, int len)
{
	iw->v.picture_changing->left_plus += xrel;
	if (iw->v.picture_changing->left_plus < 0)
		iw->v.picture_changing->left_plus = 0;
	else if (iw->v.picture_changing->left_plus + iw->v.picture_changing->tw
			> (len = (int)sqrtf(powf(iw->v.wall_picture_changing->next->x
						- iw->v.wall_picture_changing->x, 2.0f)
			+ powf(iw->v.wall_picture_changing->next->y
				- iw->v.wall_picture_changing->y, 2.0f))))
		iw->v.picture_changing->left_plus =
			len - iw->v.picture_changing->tw;
	iw->v.picture_changing->zu -= yrel;
	calculate_picture(iw, iw->v.wall_picture_changing,
			iw->v.picture_changing);
}

void	mouse_move(int xrel, int yrel, t_sdl *iw)
{
	if (iw->v.mouse_mode == 0)
		return ;
	if (iw->v.picture_changing == 0)
	{
		iw->p.rot += MOUSE_SENSIVITY * (float)xrel;
		if (iw->p.rot < 0.0f)
			iw->p.rot += G360;
		else if (iw->p.rot >= G360)
			iw->p.rot -= G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->p.rotup -= MOUSE_UP_DOWN_SENSIVITY * yrel;
		if (iw->p.rotup > 2 * WINDOW_H)
			iw->p.rotup = 2 * WINDOW_H;
		else if (iw->p.rotup < -2 * WINDOW_H)
			iw->p.rotup = -2 * WINDOW_H;
	}
	else
		mouse_move1(xrel, yrel, iw, 0);
}

void	mouse_buttonright_up(t_sdl *iw)
{
	if (iw->v.game_mode)
	{
	}
	else if (iw->v.mouse_mode == 1 && iw->v.look_portal != 0)
		iw->v.look_portal->glass = iw->v.tex_to_fill;
	else if (iw->v.mouse_mode == 1 &&
			*(iw->v.look_picture) != 0 && *(iw->v.look_wall) != 0)
		delete_picture(*(iw->v.look_wall), *(iw->v.look_picture), iw);
	else if (iw->v.look_sprite != 0 &&
		iw->v.look_sprite->type == 2 && iw->v.sprite_editing)
	{
		if (iw->v.look_sprite->e.status == 0)
		{
			iw->v.look_sprite->e.status = 1;
			iw->v.look_sprite->t_numb++;
		}
		else
		{
			iw->v.look_sprite->e.status = 0;
			iw->v.look_sprite->t_numb--;
		}
		iw->v.look_sprite->t = iw->t_enemies[iw->v.look_sprite->t_numb];
		iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[iw->v.look_sprite->t_numb];
	}
}

void	mouse_wheel1(SDL_Event *e, t_sdl *iw)
{
	if (iw->v.mouse_mode == 1 && iw->v.picture_changing != 0)
	{
		if (e->wheel.y < 0 && iw->v.picture_changing->tw - 30 > 50)
			iw->v.picture_changing->tw -= 30;
		else if (e->wheel.y > 0)
			iw->v.picture_changing->tw += 30;
		calculate_picture(iw,
			iw->v.wall_picture_changing, iw->v.picture_changing);
	}
	else if (iw->v.mouse_mode == 1 && iw->v.sprite_editing
			&& !iw->v.game_mode && iw->v.look_sprite != 0)
	{
		if (e->wheel.y < 0)
			iw->v.look_sprite->scale *= 1.1f;
		else if (iw->v.look_sprite->scale > 0.2f)
			iw->v.look_sprite->scale /= 1.1f;
	}
}

void	mouse_wheel(SDL_Event *e, t_sdl *iw)
{
	if (iw->v.mouse_y > WINDOW_H && iw->v.mouse_y
			< WINDOW_H + 100 && iw->v.mouse_mode == 0)
	{
		iw->v.scroll_first_tex -= e->wheel.y;
		if (iw->v.scroll_first_tex < 0)
			iw->v.scroll_first_tex = 0;
		if (iw->v.scroll_first_tex >= TEXTURES_COUNT)
			iw->v.scroll_first_tex = TEXTURES_COUNT - 1;
		SDL_FillRect(iw->sur, &iw->v.scroll_tex_rect, 0x000000);
		draw_tex_to_select(iw);
	}
	else if (iw->v.mouse_y > WINDOW_H + 200 && iw->v.mouse_y < WINDOW_H + 300
		&& iw->v.mouse_mode == 0 && iw->v.sprites_select_mode == 1)
	{
		iw->v.scroll_pickup_sprites -= e->wheel.y;
		if (iw->v.scroll_pickup_sprites < 0)
			iw->v.scroll_pickup_sprites = 0;
		if (iw->v.scroll_pickup_sprites >= PICK_UP_TEXTURES_COUNT)
			iw->v.scroll_pickup_sprites = PICK_UP_TEXTURES_COUNT - 1;
		draw_pickup_tex_to_select(iw);
	}
	else
		mouse_wheel1(e, iw);
}
