/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_visible_walls.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:05:15 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:05:16 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

float	get_vectors_angle(float x1, float y1, float x2, float y2)
{
	float	k;

	k = (x1 * x2 + y1 * y2) / (sqrtf(x1 * x1 + y1 * y1)
		* sqrtf(x2 * x2 + y2 * y2));
	if (k > 1.0f)
		return (acos(1.0f));
	else if (k < -1.0f)
		return (acos(-1.0f));
	return (acosf(k));
}

void	get_visible_walls2(t_sdl *iw, int wall, float lang)
{
	t_save_wall	*w;

	w = (t_save_wall *)malloc(sizeof(t_save_wall));
	w->x = (int)(lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	w->len = sqrtf(powf((float)(iw->p.x - iw->walls[wall].x), 2.0f)
		+ powf((float)(iw->p.y - iw->walls[wall].y), 2.0f));
	w->plen = fabsf(iw->d.screen.a * (float)iw->walls[wall].x +
		iw->d.screen.b * (float)iw->walls[wall].y + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	if ((int)w->plen == 0)
		w->plen = 1.0f;
	w->olen = 0.0f;
	w->p.x = iw->walls[wall].x;
	w->p.y = iw->walls[wall].y;
	w->wall = &iw->walls[wall];
	w->zu = get_ceil_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->zd = get_floor_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->next = 0;
	add_wall(iw, w);
}

void	get_visible_walls(t_sdl *iw)
{
	int		wall;
	float	lang;
	float	rang;

	wall = iw->sectors[iw->d.cs].sw;
	while (wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		lang = get_vectors_angle(iw->d.left_point.x -
			(float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
			(float)(iw->walls[wall].x - iw->p.x), (float)(iw->walls[wall].y - iw->p.y));
		rang = get_vectors_angle(iw->d.right_point.x -
			(float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
			(float)(iw->walls[wall].x - iw->p.x), (float)(iw->walls[wall].y - iw->p.y));
		if ((lang <= 2 * iw->v.angle && rang <= 2 * iw->v.angle)
			|| lang == 0.0f || rang == 0.0f)
			get_visible_walls2(iw, wall, lang);
		wall++;
	}
}

void	add_wall(t_sdl *iw, t_save_wall *tmp)
{
	t_save_wall	*tmp2;

	if (iw->d.vw == 0 || iw->d.vw->x > tmp->x)
	{
		tmp->next = iw->d.vw;
		iw->d.vw = tmp;
	}
	else
	{
		tmp2 = iw->d.vw;
		while (tmp2->next != 0 && tmp2->next->x < tmp->x)
			tmp2 = tmp2->next;
		if (tmp2->next != 0)
			tmp->next = tmp2->next;
		tmp2->next = tmp;
	}
}
