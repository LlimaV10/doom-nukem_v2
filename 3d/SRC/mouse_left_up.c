/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_left_up.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 14:15:41 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/01 14:21:25 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	mouse_buttonleft_up2(int x, t_sdl *iw, int i)
{
	if (iw->v.sprites_select_mode == 0)
	{
		i = x / 100 + iw->v.scroll_decor_sprites;
		if (i < DECOR_TEXTURES_COUNT)
		{
			iw->v.selected_sprite_type = 0;
			iw->v.selected_sprite = i;
		}
	}
	else if (iw->v.sprites_select_mode == 1)
	{
		i = x / 100 + iw->v.scroll_pickup_sprites;
		if (i < PICK_UP_TEXTURES_COUNT)
		{
			iw->v.selected_sprite_type = 1;
			iw->v.selected_sprite = i;
		}
	}
	else if (iw->v.sprites_select_mode == 2)
	{
		if (!((i = x / 100) < COUNT_ENEMIES))
			return ;
		iw->v.selected_sprite_type = 2;
		iw->v.selected_sprite = i;
	}
}

void	mouse_buttonleft_up3_1(t_sdl *iw)
{
	iw->v.look_sprite->type = 2;
	iw->v.look_sprite->e.enemy_numb = iw->v.selected_sprite;
	iw->v.look_sprite->e.status = 0;
	if (iw->v.selected_sprite == 0)
	{
		iw->v.look_sprite->e.health = ENEMY_HEALTH0;
		iw->v.look_sprite->t_numb = 0;
		iw->v.look_sprite->t = iw->t_enemies[0];
		iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[0];
	}
	else if (iw->v.selected_sprite == 1)
	{
		iw->v.look_sprite->e.health = ENEMY_HEALTH1;
		iw->v.look_sprite->t_numb = 8;
		iw->v.look_sprite->t = iw->t_enemies[8];
		iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[8];
	}
	else if (iw->v.selected_sprite == 2)
	{
		iw->v.look_sprite->e.health = ENEMY_HEALTH2;
		iw->v.look_sprite->t_numb = 20;
		iw->v.look_sprite->t = iw->t_enemies[20];
		iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[20];
	}
}

void	mouse_buttonleft_up3(t_sdl *iw)
{
	if (iw->v.selected_sprite_type == 0)
	{
		iw->v.look_sprite->type = 0;
		iw->v.look_sprite->t_numb = iw->v.selected_sprite;
		iw->v.look_sprite->t = iw->t_decor[iw->v.selected_sprite];
		iw->v.look_sprite->t_kernel =
			&iw->k.m_t_decor[iw->v.selected_sprite];
		if (iw->v.selected_sprite == 1)
			add_checkpoint(iw, iw->v.look_sprite);
	}
	else if (iw->v.selected_sprite_type == 1)
	{
		iw->v.look_sprite->type = 1;
		iw->v.look_sprite->t_numb = iw->v.selected_sprite;
		iw->v.look_sprite->t = iw->t_pickup[iw->v.selected_sprite];
		iw->v.look_sprite->t_kernel =
			&iw->k.m_t_pickup[iw->v.selected_sprite];
	}
	else if (iw->v.selected_sprite_type == 2)
		mouse_buttonleft_up3_1(iw);
}

void	mouse_buttonleft_up4(t_sdl *iw)
{
	if (iw->v.mouse_mode == 1 && *(iw->v.look_picture) != 0
		&& *(iw->v.look_wall) != 0 && !iw->v.game_mode)
	{
		(*(iw->v.look_picture))->t = iw->v.tex_to_fill;
		calculate_picture(iw, *(iw->v.look_wall), *(iw->v.look_picture));
	}
	else if (iw->v.mouse_mode == 1 &&
			*(iw->v.look_wall) != 0 && !iw->v.game_mode)
	{
		if (iw->v.look_portal == 0 || iw->v.look_portal->glass < 0)
			(*(iw->v.look_wall))->t = iw->v.tex_to_fill;
		else
			iw->v.look_portal->glass = -1;
	}
}

void	mouse_buttonleft_up(int x, int y, t_sdl *iw)
{
	int		i;

	iw->v.left_mouse_pressed = 0;
	if (y > WINDOW_H && y < WINDOW_H + 100 && iw->v.mouse_mode == 0)
	{
		i = x / 100 + iw->v.scroll_first_tex;
		if (i < TEXTURES_COUNT)
			iw->v.tex_to_fill = i;
	}
	else if (y > WINDOW_H + 100 && y < WINDOW_H + 200
		&& iw->v.mouse_mode == 0 && *(iw->v.look_wall) != 0)
		mouse_buttonleft_up1(x, y, iw);
	else if (y > WINDOW_H + 200 && y < WINDOW_H + 300 && iw->v.mouse_mode == 0)
		mouse_buttonleft_up2(x, iw, 0);
	else if (iw->v.sprite_editing &&
			iw->v.mouse_mode == 1 && iw->v.look_sprite != 0)
		mouse_buttonleft_up3(iw);
	else
		mouse_buttonleft_up4(iw);
}
