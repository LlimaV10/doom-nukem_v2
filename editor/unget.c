/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unget.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 14:45:55 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/05 14:45:56 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	give_date(t_doom *den, t_sdl *iw)
{
	t_sprite	*tmp;

	if (den->player.x && den->player.y)
	{
		iw->l.start_x = den->player.x * MAP_SCALE;
		if (iw->l.start_x % 2 == 0)
			iw->l.start_x += 1;
		iw->l.start_y = -den->player.y * MAP_SCALE;
		if (iw->l.start_y % 2 == 0)
			iw->l.start_y += 1;
	}
	iw->l.start_rot = den->player.introt;
	iw->l.win_sector = den->finish;
	tmp = den->sprite;
	while (tmp)
	{
		tmp->x *= MAP_SCALE;
		tmp->y *= -MAP_SCALE;
		tmp = tmp->next;
	}
}

void	unget_map_help(t_sdl *iw, t_wals *tmp, t_sector_list *sl)
{
	int		i;
	int		j;

	i = -1;
	while (++i < iw->v.sc)
	{
		sl->fr.z = iw->sectors[i].fr.z;
		sl->fr.n = iw->sectors[i].fr.n;
		sl->fr.t = iw->sectors[i].fr.t;
		sl->cl.z = iw->sectors[i].cl.z;
		sl->cl.n = iw->sectors[i].cl.n;
		sl->cl.t = iw->sectors[i].cl.t;
		sl->light = iw->sectors[i].light;
		j = iw->sectors[i].sw - 1;
		while (++j < iw->sectors[i].sw + iw->sectors[i].nw)
		{
			tmp->tex = iw->walls[j].t;
			tmp->glass = iw->walls[j].glass;
			tmp->p = iw->walls[j].p;
			tmp = tmp->next;
		}
		sl = sl->next;
	}
}

void	unget_map(t_sdl *iw, t_doom *den, t_wals *tmp)
{
	t_sector_list	*sl;
	t_sprite		*tmp2;

	sl = den->sectors;
	unget_map_help(iw, tmp, sl);
	den->sprite = *den->iw.sprite;
	tmp2 = den->sprite;
	while (tmp2)
	{
		tmp2->x /= 400;
		tmp2->y /= -400;
		tmp2 = tmp2->next;
	}
}
