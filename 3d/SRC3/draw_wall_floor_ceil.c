/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_floor_ceil.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:03:56 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:03:57 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_wall_floor_ceil_tex3(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (iw->d.wallBot[d->j] < iw->d.top[left->x + d->j])
		d->i = iw->d.top[left->x + d->j] - 1;
	else
		d->i = iw->d.wallBot[d->j] - 1;
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) +
		d->frpl * (float)(d->i + 1 - iw->d.wallBot[d->j]);
	while (++d->i < iw->d.bottom[left->x + d->j])
	{
		d->weight = d->wall_dist / d->k;
		d->k += d->frpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		set_pixel2(iw->sur, left->x + d->j, d->i,
	get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
	((d->floor.x < 0) ? (((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs].
	fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->
	d.cs].fr.t]->w - 1) : ((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.
	cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)), ((d->floor.y < 0)
	? (((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t
	[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h -
	1) : ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) %
	iw->t[iw->sectors[iw->d.cs].fr.t]->h))), iw->sectors[iw->d.cs].light));
	}
	iw->d.bottom[left->x + d->j] = iw->d.wallBot[d->j];
}

void	draw_wall_floor_ceil_tex4(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (iw->d.wallTop[d->j] < iw->d.bottom[left->x + d->j])
		d->i = iw->d.wallTop[d->j] + 1;
	else
		d->i = iw->d.bottom[left->x + d->j] + 1;
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) + d->clpl *
		(float)(iw->d.wallTop[d->j] - d->i + 1);
	while (--d->i >= iw->d.top[left->x + d->j])
	{
		d->weight = d->wall_dist / d->k;
		d->k += d->clpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		set_pixel2(iw->sur, left->x + d->j, d->i,
	get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
	((d->floor.x < 0) ? (((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs].
	cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->
	d.cs].cl.t]->w - 1) : ((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.
	cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)), ((d->floor.y < 0)
	? (((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t
	[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h -
	1) : ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->
	t[iw->sectors[iw->d.cs].cl.t]->h))), iw->sectors[iw->d.cs].light));
	}
	iw->d.top[left->x + d->j] = iw->d.wallTop[d->j];
}

void	draw_wall_floor_ceil_tex5_1(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (iw->d.wallTop[d->j] < iw->d.top[d->j + left->x] &&
		iw->d.top[left->x + d->j] < iw->d.bottom[left->x + d->j])
		iw->d.bottom[left->x + d->j] = iw->d.top[left->x + d->j] - 1;
	else if (iw->d.wallTop[d->j] >= iw->d.top[d->j + left->x] &&
		iw->d.wallTop[d->j] < iw->d.bottom[left->x + d->j])
		iw->d.bottom[left->x + d->j] = iw->d.wallTop[d->j] - 1;
}

void	draw_wall_floor_ceil_tex5(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->tx = (left->olen + d->left_len) * (float)iw->t[left->wall->t]->w *
		iw->tsz[left->wall->t] / 1000.0f;
	if (iw->d.wallTop[d->j] < iw->d.top[d->j + left->x])
		d->ty = d->zu + iw->tsz[left->wall->t] * (d->zu - d->zd) *
		(float)(iw->d.top[d->j + left->x] - iw->d.wallTop[d->j]) /
		(float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]);
	else
		d->ty = d->zu;
	d->ty = d->ty * (float)iw->t[left->wall->t]->h / 1000.0f;
	d->dty = ((d->zu - d->zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (
	float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) * iw->tsz[left->wall->t];
	if (iw->d.wallTop[d->j] < iw->d.top[d->j + left->x])
		d->i = iw->d.top[left->x + d->j] - 1;
	else
		d->i = iw->d.wallTop[d->j] - 1;
	while (++d->i < iw->d.bottom[left->x + d->j])
	{
		set_pixel2(iw->sur, left->x + d->j, d->i,
	get_light_color(get_pixel(iw->t[left->wall->t], (int)d->tx
	% iw->t[left->wall->t]->w, (int)d->ty % iw->t[left->wall->t]->h),
	iw->sectors[iw->d.cs].light));
		d->ty += d->dty;
	}
	draw_wall_floor_ceil_tex5_1(iw, left, d);
}

void	draw_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex	d;

	draw_wall_floor_ceil_tex1(iw, left, right, &d);
	d.py = (float)iw->p.y / 1000.0f;
	d.frcoef = d.zu - d.zd;
	d.j = -1;
	while (++d.j < len)
	{
		if (iw->d.top[left->x + d.j] >= iw->d.bottom[left->x + d.j])
		{
			d.ang += d.dang;
			continue;
		}
		draw_wall_floor_ceil_tex2(iw, left, &d);
		if (iw->d.wallBot[d.j] < iw->d.bottom[left->x + d.j])
			draw_wall_floor_ceil_tex3(iw, left, &d);
		if (iw->d.wallTop[d.j] > iw->d.top[left->x + d.j] &&
			iw->sectors[iw->d.cs].cl.t >= 0)
			draw_wall_floor_ceil_tex4(iw, left, &d);
		if (left->wall->t >= 0)
			draw_wall_floor_ceil_tex5(iw, left, &d);
		d.ang += d.dang;
	}
}
