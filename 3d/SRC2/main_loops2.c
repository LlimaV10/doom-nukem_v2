/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loops2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:44:37 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:44:38 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	game_start_menu(t_sdl *iw)
{
	SDL_Rect player;
	SDL_Rect zast;
	SDL_Rect diff;

	game_start_menu1(iw, &player, &zast, &diff);
	if (iw->menu.count == 1)
		ft_scaled_blit(iw->menu.icons[2], iw->sur, &zast);
	else
		ft_scaled_blit(iw->menu.icons[1], iw->sur, &zast);
	ft_scaled_blit(iw->menu.icons[3], iw->sur, &diff);
	zast.y += zast.h + WINDOW_H / 10;
	diff.y = zast.y + (zast.h - diff.h) / 2;
	if (iw->menu.count == 2)
		ft_scaled_blit(iw->menu.icons[2], iw->sur, &zast);
	else
		ft_scaled_blit(iw->menu.icons[1], iw->sur, &zast);
	ft_scaled_blit(iw->menu.icons[4], iw->sur, &diff);
	zast.y += zast.h + WINDOW_H / 10;
	diff.y = zast.y + (zast.h - diff.h) / 2;
	if (iw->menu.count == 3)
		ft_scaled_blit(iw->menu.icons[2], iw->sur, &zast);
	else
		ft_scaled_blit(iw->menu.icons[1], iw->sur, &zast);
	ft_scaled_blit(iw->menu.icons[5], iw->sur, &diff);
	SDL_UpdateWindowSurface(iw->win);
}
