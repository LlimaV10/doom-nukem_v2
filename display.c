/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 15:43:24 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/01/30 15:43:26 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	board_display_three(t_doom *den)
{
	den->rect.x = 1815;
	den->rect.y = 455;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[8], NULL, den->bmp, &den->rect);
	den->rect.x = 1874;
	den->rect.y = 445;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[10], NULL, den->bmp, &den->rect);
	den->rect.x = 1937;
	den->rect.y = 450;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[11], NULL, den->bmp, &den->rect);
}

void	board_display_two(t_doom *den)
{
	den->rect.x = 1840;
	den->rect.y = 280;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[4], NULL, den->bmp, &den->rect);
	den->rect.x = 1840;
	den->rect.y = 680;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[5], NULL, den->bmp, &den->rect);
	den->rect.x = 1770;
	den->rect.y = 300;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[6], NULL, den->bmp, &den->rect);
	den->rect.x = 1755;
	den->rect.y = 450;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[7], NULL, den->bmp, &den->rect);
	board_display_three(den);
}

void	board_display(t_doom *den)
{
	den->rect.x = 1755;
	den->rect.y = 500;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[0], NULL, den->bmp, &den->rect);
	den->rect.x = 1810;
	den->rect.y = 510;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[1], NULL, den->bmp, &den->rect);
	den->rect.x = 1875;
	den->rect.y = 500;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[2], NULL, den->bmp, &den->rect);
	den->rect.x = 1935;
	den->rect.y = 510;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->icon[3], NULL, den->bmp, &den->rect);
	board_display_two(den);
}

void	clean_display(t_doom *den)
{
	int i;
	int	j;

	j = -1;
	while (++j < HEIGHT / 2)
	{
		i = den->border;
		while (++i < WIDTH)
			set_pixel(den->bmp, i, j, 0);
	}
}

void	blouk_wall(t_doom *den, t_wals *tmp, t_col *vec)
{
	t_wals	*rmp;
	int		i;

	vec = NULL;
	rmp = tmp;
	i = -1;
	while (++i <= den->sec)
	{
		while (tmp->next)
		{
			if (tmp->sec != i)
				break ;
			tmp = tmp->next;
		}
	}
	tmp = rmp;
}
