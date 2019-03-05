/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hud2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:21:09 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:21:11 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_selected_sprite(t_sdl *iw)
{
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
	rect.x = WINDOW_W - 110;
	rect.y = 130;
	rect.w = 100;
	rect.h = 100;
	if (iw->v.selected_sprite_type == 0)
		ft_scaled_blit(iw->t_decor[iw->v.selected_sprite], iw->sur, &rect);
	else if (iw->v.selected_sprite_type == 1)
		ft_scaled_blit(iw->t_pickup[iw->v.selected_sprite], iw->sur, &rect);
	else if (iw->v.selected_sprite_type == 2)
	{
		if (iw->v.selected_sprite == 0)
			ft_scaled_blit(iw->t_enemies[0], iw->sur, &rect);
		else if (iw->v.selected_sprite == 1)
			ft_scaled_blit(iw->t_enemies[8], iw->sur, &rect);
		else if (iw->v.selected_sprite == 2)
			ft_scaled_blit(iw->t_enemies[20], iw->sur, &rect);
	}
}

void	draw_submenu4(t_sdl *iw)
{
	char	*s;

	if (iw->v.submenu_mode == 8)
	{
		draw_text(iw, "Set DY of walls animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 9)
	{
		draw_text(iw, "Set speed of walls animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Speed:", WINDOW_W - 450, WINDOW_H + 135);
		draw_text(iw, (s = ft_itoa(iw->v.wall_anim->speed)),
				WINDOW_W - 370, WINDOW_H + 135);
		free(s);
		draw_text(iw, "+  -  OK  Exit", WINDOW_W - 340, WINDOW_H + 135);
	}
}

void	draw_submenu3(t_sdl *iw)
{
	if (iw->v.submenu_mode == 5)
	{
		draw_text(iw, "Select walls moving type:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Same  Inv_X  Inv_Y   Exit",
				WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 7)
	{
		draw_text(iw, "Set DX of walls animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else
		draw_submenu4(iw);
}

void	draw_submenu2(t_sdl *iw)
{
	char		*s;

	if (iw->v.submenu_mode == 3)
	{
		draw_text(iw, "Set speed of sector animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Speed:", WINDOW_W - 450, WINDOW_H + 135);
		draw_text(iw, (s = ft_itoa(iw->v.sector_anim->speed)),
				WINDOW_W - 370, WINDOW_H + 135);
		free(s);
		draw_text(iw, "+  -  OK  Exit", WINDOW_W - 340, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 4)
	{
		draw_text(iw, "Choose animation:", WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "Sector  Walls   Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 6)
	{
		draw_text(iw, "Select walls moving priority:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "DXDY  DYDX   Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else
		draw_submenu3(iw);
}

void	draw_submenu(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.x = WINDOW_W - 500;
	rect.y = WINDOW_H + 100;
	rect.w = 500;
	rect.h = 100;
	SDL_FillRect(iw->sur, &rect, 0x000000);
	if (iw->v.submenu_mode == 1)
	{
		draw_text(iw, "Choose type of sector animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "F  C  FC    Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else if (iw->v.submenu_mode == 2)
	{
		draw_text(iw, "Set DZ of sector animation:",
				WINDOW_W - 500, WINDOW_H + 110);
		draw_text(iw, "+100  -100  OK  Exit", WINDOW_W - 450, WINDOW_H + 135);
	}
	else
		draw_submenu2(iw);
}
