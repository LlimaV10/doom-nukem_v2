/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_inclined_wall_floor_ceil2.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:08:28 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 17:08:29 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_inclined_wall_floor_ceil_tex1(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_wall_floor_ceil_tex *d)
{
	d->lv.x = (float)(left->p.x - iw->p.x);
	d->lv.y = (float)(left->p.y - iw->p.y);
	d->rv.x = (float)(right->p.x - iw->p.x);
	d->rv.y = (float)(right->p.y - iw->p.y);
	d->ang = acosf((d->lv.x * d->rv.x + d->lv.y * d->rv.y) /
		(sqrtf(d->lv.x * d->lv.x + d->lv.y * d->lv.y) *
			sqrtf(d->rv.x * d->rv.x + d->rv.y * d->rv.y)));
	d->dang = d->ang / (float)(right->x - left->x + 1);
	d->ang = 0.0f;
	d->rv.x = (float)(-right->p.x + left->p.x);
	d->rv.y = (float)(-right->p.y + left->p.y);
	d->sing = G180 - acosf((d->lv.x * d->rv.x + d->lv.y * d->rv.y) /
		(sqrtf(d->lv.x * d->lv.x + d->lv.y * d->lv.y) *
			sqrtf(d->rv.x * d->rv.x + d->rv.y * d->rv.y)));
	d->lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f)
		+ powf(iw->p.y - left->p.y, 2.0f));
	d->len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f)
		+ powf(left->p.y - right->p.y, 2.0f));
	d->zudiff = (right->zu - left->zu) / d->len_lr;
	d->zddiff = (right->zd - left->zd) / d->len_lr;
	d->rv.x = (float)(right->p.x - left->p.x) / d->len_lr;
	d->rv.y = (float)(right->p.y - left->p.y) / d->len_lr;
	d->zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d->zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d->frpl = (float)(d->zu - d->zd) / (float)(iw->p.z - d->zd);
}

void	draw_inclined_wall_floor_ceil_tex3(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) +
		d->frpl * (float)(d->i + 1 - iw->d.wallBot[d->j]);
	while (++d->i < iw->d.bottom[left->x + d->j])
	{
		d->weight = d->wall_dist * d->frcoef / d->k;
		d->k += d->frpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		d->frcoef = (get_ceil_z(iw, d->floor.x * 1000.0f, d->floor.y *
		1000.0f) - get_floor_z(iw, d->floor.x * 1000.0f, d->floor.y * 1000.0f)
			+ d->frcoef) / 2.0f;
		set_pixel2(iw->sur, left->x + d->j, d->i,
	get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
	((d->floor.x < 0) ? (((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs].
	fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->
	d.cs].fr.t]->w - 1) : ((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.
	cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)), ((d->floor.y < 0)
	? (((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->
	t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h
	- 1) : ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) %
	iw->t[iw->sectors[iw->d.cs].fr.t]->h))),
	iw->sectors[iw->d.cs].light));
	}
	iw->d.bottom[left->x + d->j] = iw->d.wallBot[d->j];
}
