/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hud3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:21:15 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:21:17 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	clear_sprites_tex_to_select(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = WINDOW_W;
	rect.h = 100;
	SDL_FillRect(iw->sur, &rect, 0x000000);
}

void	draw_decor_tex_to_select(t_sdl *iw)
{
	int			i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
	clear_sprites_tex_to_select(iw);
	iw->v.sprites_select_mode = 0;
	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = 100;
	rect.h = 100;
	i = iw->v.scroll_decor_sprites - 1;
	while (++i < DECOR_TEXTURES_COUNT && rect.x < WINDOW_W)
	{
		ft_scaled_blit(iw->t_decor[i], iw->sur, &rect);
		rect.x += 100;
	}
}

void	draw_pickup_tex_to_select(t_sdl *iw)
{
	int			i;
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
	clear_sprites_tex_to_select(iw);
	iw->v.sprites_select_mode = 1;
	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = 100;
	rect.h = 100;
	i = iw->v.scroll_pickup_sprites - 1;
	while (++i < PICK_UP_TEXTURES_COUNT && rect.x < WINDOW_W)
	{
		ft_scaled_blit(iw->t_pickup[i], iw->sur, &rect);
		rect.x += 100;
	}
}

void	draw_enemies_tex_to_select(t_sdl *iw)
{
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
	clear_sprites_tex_to_select(iw);
	iw->v.sprites_select_mode = 2;
	rect.x = 0;
	rect.y = WINDOW_H + 200;
	rect.w = 100;
	rect.h = 100;
	ft_scaled_blit(iw->t_enemies[0], iw->sur, &rect);
	rect.x += 100;
	ft_scaled_blit(iw->t_enemies[8], iw->sur, &rect);
	rect.x += 100;
	ft_scaled_blit(iw->t_enemies[20], iw->sur, &rect);
}

void	draw_selected_tex(t_sdl *iw)
{
	SDL_Rect	rect;

	if (iw->v.game_mode)
		return ;
	rect.x = WINDOW_W - 110;
	rect.y = 10;
	rect.w = 100;
	rect.h = 100;
	ft_scaled_blit(iw->t[iw->v.tex_to_fill], iw->sur, &rect);
}
