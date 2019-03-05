/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_game2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:14:46 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:14:56 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void			draw_miss(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.w = WINDOW_W / 10;
	rect.h = rect.w * iw->hud.miss->h / iw->hud.miss->w;
	rect.x = WINDOW_W / 2 - rect.w / 2;
	rect.y = WINDOW_H / 3 - WINDOW_H / 10 *
		(clock() - iw->hud.miss_time) / CLKS_P_S;
	ft_scaled_blit(iw->hud.miss, iw->sur, &rect);
}

void			draw_save(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.w = WINDOW_W / 4;
	rect.h = rect.w * iw->hud.saved->h / iw->hud.saved->w;
	rect.x = WINDOW_W / 2 - rect.w / 2;
	rect.y = 20 + WINDOW_H / 20 * (clock() - iw->hud.saved_time) / CLKS_P_S;
	ft_scaled_blit(iw->hud.saved, iw->sur, &rect);
}
