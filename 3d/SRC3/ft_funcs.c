/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_funcs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:04:21 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:04:22 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		ft_min(int p1, int p2)
{
	if (p1 < p2)
		return (p1);
	return (p2);
}

int		ft_max(int p1, int p2)
{
	if (p1 >= p2)
		return (p1);
	return (p2);
}

void	ft_scaled_blit(t_packaging_texture *tex,
	SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;
	int		p;

	i = -1;
	while (++i < rect->h)
	{
		j = -1;
		while (++j < rect->w)
		{
			p = get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h);
			if (p != 0x010000)
				set_pixel2(winsur, rect->x + j, rect->y + i, p);
		}
	}
}

void	ft_scaled_blit2(t_packaging_texture *tex,
	SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;

	i = -1;
	while (++i < rect->h)
	{
		j = -1;
		while (++j < rect->w)
			set_pixel2(winsur, rect->x + j, rect->y + i,
				get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h));
	}
}
