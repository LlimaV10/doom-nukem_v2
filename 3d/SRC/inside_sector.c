/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inside_sector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 17:07:12 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 17:07:35 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		in_sec_xy(t_sdl *iw, int sector, int x, int y)
{
	t_in_sec	d;
	int			i;

	d.wallCrossed = 0;
	i = iw->sectors[sector].sw - 1;
	while (++i < iw->sectors[sector].sw + iw->sectors[sector].nw)
	{
		d.y1 = iw->walls[i].y - y;
		d.y2 = iw->walls[i].next->y - y;
		if ((d.y1 ^ d.y2) < 0)
		{
			d.x1 = iw->walls[i].x - x;
			d.x2 = iw->walls[i].next->x - x;
			if ((d.x1 ^ d.x2) >= 0)
				d.wallCrossed ^= d.x1;
			else
				d.wallCrossed ^= (d.x1 * d.y2 - d.x2 * d.y1) ^ d.y2;
		}
	}
	return (d.wallCrossed >> 31);
}

int		in_sec(int sector, t_sdl *iw)
{
	t_in_sec	d;
	int			i;

	d.wallCrossed = 0;
	i = iw->sectors[sector].sw - 1;
	while (++i < iw->sectors[sector].sw + iw->sectors[sector].nw)
	{
		d.y1 = iw->walls[i].y - iw->p.y;
		d.y2 = iw->walls[i].next->y - iw->p.y;
		if ((d.y1 ^ d.y2) < 0)
		{
			d.x1 = iw->walls[i].x - iw->p.x;
			d.x2 = iw->walls[i].next->x - iw->p.x;
			if ((d.x1 ^ d.x2) >= 0)
				d.wallCrossed ^= d.x1;
			else
				d.wallCrossed ^= (d.x1 * d.y2 - d.x2 * d.y1) ^ d.y2;
		}
	}
	return (d.wallCrossed >> 31);
}

int		get_sector(t_sdl *iw)
{
	int sec;

	sec = iw->v.ls - 1;
	while (++sec < iw->v.sc)
		if (in_sec(sec, iw) != 0)
			return (sec);
	sec = -1;
	while (++sec < iw->v.ls)
		if (in_sec(sec, iw) != 0)
			return (sec);
	return (-1);
}