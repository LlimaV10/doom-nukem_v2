/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:10:25 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/05 12:10:26 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		count_walls(t_wals *tmp, t_getmap *gm, t_sdl *iw, t_doom *den)
{
	int		len;

	iw->v.sc = den->sec;
	iw->v.ls = 0;
	len = 0;
	gm->sl = den->sectors;
	gm->i = -1;
	while (tmp != 0)
	{
		tmp->id = len;
		len++;
		tmp = tmp->next;
	}
	return (len);
}

void	get_map_one(t_sdl *iw, t_getmap *gm)
{
	gm->sl->id = gm->i;
	iw->sectors[gm->i].nw = 0;
	iw->sectors[gm->i].fr.n = gm->sl->fr.n;
	iw->sectors[gm->i].fr.z = gm->sl->fr.z;
	iw->sectors[gm->i].fr.t = gm->sl->fr.t;
	iw->sectors[gm->i].cl.n = gm->sl->cl.n;
	iw->sectors[gm->i].cl.z = gm->sl->cl.z;
	iw->sectors[gm->i].cl.t = gm->sl->cl.t;
	iw->sectors[gm->i].light = gm->sl->light;
}

void	get_map_two(t_sdl *iw, t_getmap *gm)
{
	if (gm->tmp->next_sector)
	{
		iw->walls[gm->w].nextsector = gm->tmp->next_sector->id;
		iw->walls[gm->w].nextsector_wall = gm->tmp->nextsector_wall->id;
	}
	else
	{
		iw->walls[gm->w].nextsector = -1;
		iw->walls[gm->w].nextsector_wall = -1;
	}
	if (gm->inside != gm->tmp->inside)
	{
		iw->walls[gm->w - 1].next = &iw->walls[gm->inside_start];
		gm->inside_start = gm->w;
		gm->inside++;
	}
	else if (gm->w != iw->sectors[gm->s].sw)
		iw->walls[gm->w - 1].next = &iw->walls[gm->w];
	else
	{
		iw->sectors[gm->s].fr.x = iw->walls[gm->w].x;
		iw->sectors[gm->s].fr.y = iw->walls[gm->w].y;
		iw->sectors[gm->s].cl.x = iw->walls[gm->w].x;
		iw->sectors[gm->s].cl.y = iw->walls[gm->w].y;
	}
}

void	get_map_three(t_sdl *iw, t_getmap *gm)
{
	if (gm->tmp->sec != gm->s)
	{
		iw->walls[gm->w - 1].next = &iw->walls[gm->inside_start];
		gm->s++;
		iw->sectors[gm->s].sw = gm->w;
		gm->inside = 0;
		gm->inside_start = gm->w;
	}
	iw->sectors[gm->s].nw++;
	iw->walls[gm->w].x = gm->tmp->x * MAP_SCALE;
	iw->walls[gm->w].y = -gm->tmp->y * MAP_SCALE;
	iw->walls[gm->w].t = gm->tmp->tex;
	iw->walls[gm->w].glass = gm->tmp->glass;
	iw->walls[gm->w].p = gm->tmp->p;
}

void	get_map(t_sdl *iw, t_doom *den)
{
	t_getmap		gm;

	gm.cw = count_walls(den->tmp, &gm, iw, den);
	iw->sectors = (t_sector *)malloc(iw->v.sc * sizeof(t_sector));
	while (++gm.i < iw->v.sc)
	{
		get_map_one(iw, &gm);
		gm.sl = gm.sl->next;
	}
	iw->walls = (t_wall *)malloc(gm.cw * sizeof(t_wall));
	gm.s = 0;
	iw->sectors[0].sw = 0;
	gm.w = 0;
	gm.inside = 0;
	gm.inside_start = 0;
	gm.tmp = den->tmp;
	while (gm.tmp != 0)
	{
		get_map_three(iw, &gm);
		get_map_two(iw, &gm);
		gm.w++;
		gm.tmp = gm.tmp->next;
	}
	iw->walls[gm.w - 1].next = &iw->walls[gm.inside_start];
	*den->iw.sprite = den->sprite;
}
