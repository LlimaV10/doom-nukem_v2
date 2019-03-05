/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_inclined_wall_floor_ceil.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:03:14 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:03:15 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_inclined_wall_floor_ceil_tex2(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->left_len = sinf(d->ang) * d->lenpl / sinf(d->sing - d->ang);
	d->r.x = (float)left->p.x + d->rv.x * d->left_len;
	d->r.y = (float)left->p.y + d->rv.y * d->left_len;
	d->frcoef = get_ceil_z(iw, d->r.x, d->r.y) -
		get_floor_z(iw, d->r.x, d->r.y);
	d->clcoef = d->frcoef;
	d->wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a
		* d->r.x + iw->d.screen.b * d->r.y + iw->d.screen.c) /
		iw->d.screen_len);
	d->r.x /= 1000.0f;
	d->r.y /= 1000.0f;
	if (iw->d.wallBot[d->j] < iw->d.bottom[left->x + d->j])
	{
		if (iw->d.wallBot[d->j] < iw->d.top[left->x + d->j])
			d->i = iw->d.top[left->x + d->j] - 1;
		else
			d->i = iw->d.wallBot[d->j] - 1;
		draw_inclined_wall_floor_ceil_tex3(iw, left, d);
	}
}

void	draw_inclined_wall_floor_ceil_tex4(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) +
		d->clpl * (float)(iw->d.wallTop[d->j] - d->i + 1);
	while (--d->i >= iw->d.top[left->x + d->j])
	{
		d->weight = d->wall_dist * d->clcoef / d->k;
		d->k += d->clpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		d->clcoef = (get_ceil_z(iw, d->floor.x * 1000.0f, d->floor.y *
		1000.0f) - get_floor_z(iw, d->floor.x * 1000.0f, d->floor.y * 1000.0f)
			+ d->clcoef) / 2.0f;
		set_pixel2(iw->sur, left->x + d->j, d->i,
	get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
	((d->floor.x < 0) ? (((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs].
	cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->
	d.cs].cl.t]->w - 1) : ((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs
	].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)), ((d->floor.y < 0) ?
	(((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t
	[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h
	- 1) : ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) %
	iw->t[iw->sectors[iw->d.cs].cl.t]->h))),
	iw->sectors[iw->d.cs].light));
	}
	iw->d.top[left->x + d->j] = iw->d.wallTop[d->j];
}

void	draw_inclined_wall_floor_ceil_tex5(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->zu = (float)left->zu + d->left_len * d->zudiff;
	d->zd = (float)left->zd + d->left_len * d->zddiff;
	if (iw->d.wallTop[d->j] < iw->d.top[d->j + left->x])
		d->ty = d->zu + iw->tsz[left->wall->t] * (d->zu - d->zd) *
		(float)(iw->d.top[d->j + left->x] - iw->d.wallTop[d->j]) /
		(float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]);
	else
		d->ty = d->zu;
	d->ty = d->ty * (float)iw->t[left->wall->t]->h / 1000.0f;
	d->dty = ((d->zu - d->zd) * (float)iw->t[left->wall->t]->h / 1000.0f) /
		(float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) *
		iw->tsz[left->wall->t];
	if (iw->d.wallTop[d->j] < iw->d.top[d->j + left->x])
		d->i = iw->d.top[left->x + d->j] - 1;
	else
		d->i = iw->d.wallTop[d->j] - 1;
	while (++d->i < iw->d.bottom[left->x + d->j])
	{
		set_pixel2(iw->sur, left->x + d->j, d->i,
			get_light_color(get_pixel(iw->t[left->wall->t], (int)d->tx %
				iw->t[left->wall->t]->w, (int)d->ty % iw->t[left->wall->t]->h),
				iw->sectors[iw->d.cs].light));
		d->ty += d->dty;
	}
}

void	draw_inclined_wall_floor_ceil_tex6(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (iw->d.wallTop[d->j] > iw->d.top[left->x + d->j]
		&& iw->sectors[iw->d.cs].cl.t >= 0)
	{
		if (iw->d.wallTop[d->j] < iw->d.bottom[left->x + d->j])
			d->i = iw->d.wallTop[d->j] + 1;
		else
			d->i = iw->d.bottom[left->x + d->j] + 1;
		draw_inclined_wall_floor_ceil_tex4(iw, left, d);
	}
	if (left->wall->t >= 0)
	{
		d->tx = (left->olen + d->left_len) * (float)iw->t[left->wall->t]->w
			* iw->tsz[left->wall->t] / 1000.0f;
		draw_inclined_wall_floor_ceil_tex5(iw, left, d);
		if (iw->d.wallTop[d->j] < iw->d.top[d->j + left->x] &&
			iw->d.top[left->x + d->j] < iw->d.bottom[left->x + d->j])
			iw->d.bottom[left->x + d->j] = iw->d.top[left->x + d->j] - 1;
		else if (iw->d.wallTop[d->j] >= iw->d.top[d->j + left->x] &&
			iw->d.wallTop[d->j] < iw->d.bottom[left->x + d->j])
			iw->d.bottom[left->x + d->j] = iw->d.wallTop[d->j] - 1;
	}
}

void	draw_inclined_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex	d;

	draw_inclined_wall_floor_ceil_tex1(iw, left, right, &d);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;
	d.j = -1;
	while (++d.j < len)
	{
		if (iw->d.top[left->x + d.j] >= iw->d.bottom[left->x + d.j])
		{
			d.ang += d.dang;
			continue;
		}
		draw_inclined_wall_floor_ceil_tex2(iw, left, &d);
		draw_inclined_wall_floor_ceil_tex6(iw, left, &d);
		d.ang += d.dang;
	}
}
