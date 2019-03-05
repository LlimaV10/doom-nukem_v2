/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ways_exit_x.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:04:14 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:04:15 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	free_sector_ways2(t_free_sector_ways *d)
{
	d->tmp2 = d->tmp->way_start;
	while (d->tmp2)
	{
		d->tmp2_f = d->tmp2;
		d->tmp2 = d->tmp2->next;
		free(d->tmp2_f);
	}
	d->tmp_f = d->tmp;
	d->tmp = d->tmp->next;
	free(d->tmp_f);
}

void	free_sector_ways(t_sdl *iw)
{
	t_free_sector_ways	d;

	d.i = -1;
	while (++d.i < iw->v.sc)
	{
		d.j = -1;
		while (++d.j < iw->v.sc)
		{
			d.tmp = iw->ways[d.i][d.j];
			while (d.tmp)
				free_sector_ways2(&d);
		}
		free(iw->ways[d.i]);
	}
	free(iw->ways);
}

void	exit_x(t_sdl *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	TTF_Quit();
	SDL_Quit();
	system("leaks doom-nukem");
	exit(0);
}
