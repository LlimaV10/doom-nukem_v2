/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor_ceil.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:02:39 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:02:40 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_floor_ceil_tex3(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->left_len = sinf(d->ang) * d->lenpl / sinf(d->sing - d->ang);
	d->r.x = (float)left->p.x + d->rv.x * d->left_len;
	d->r.y = (float)left->p.y + d->rv.y * d->left_len;
	d->frcoef = get_ceil_z(iw, d->r.x, d->r.y) -
		get_floor_z(iw, d->r.x, d->r.y);
	d->wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a *
		d->r.x + iw->d.screen.b * d->r.y + iw->d.screen.c) /
		iw->d.screen_len) * d->frcoef;
	d->r.x /= 1000.0f;
	d->r.y /= 1000.0f;
}

void	draw_floor_ceil_tex4(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (iw->d.wallBot[d->j] < iw->d.top[left->x + d->j])
		d->i = iw->d.top[left->x + d->j] - 1;
	else
		d->i = iw->d.wallBot[d->j] - 1;
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j])
		+ d->frpl * (float)(d->i + 1 - iw->d.wallBot[d->j]);
	while (++d->i < iw->d.bottom[left->x + d->j])
	{
		d->weight = d->wall_dist / d->k;
		d->k += d->frpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		set_pixel2(iw->sur, left->x + d->j, d->i,
		get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
		((d->floor.x < 0) ? (((int)(d->floor.x * (float)iw->t[iw->sectors
		[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)
		+ iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) : ((int)(d->floor.x
		* (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->
		sectors[iw->d.cs].fr.t]->w)), ((d->floor.y < 0) ? (((int)(d->floor.y
		* (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->
		sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1)
		: ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h)
			% iw->t[iw->sectors[iw->d.cs].fr.t]->h))),
			iw->sectors[iw->d.cs].light));
	}
}

void	draw_floor_ceil_tex5_1(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (iw->d.wallTop[d->j] < iw->d.bottom[left->x + d->j])
		d->i = iw->d.wallTop[d->j] + 1;
	else
		d->i = iw->d.bottom[left->x + d->j] + 1;
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j])
		+ d->clpl * (float)(iw->d.wallTop[d->j] - d->i + 1);
	while (--d->i >= iw->d.top[left->x + d->j])
	{
		d->weight = d->wall_dist / d->k;
		d->k += d->clpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		set_pixel2(iw->sur, left->x + d->j, d->i,
		get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
		((d->floor.x < 0) ? (((int)(d->floor.x * (float)
		iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors
		[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
		((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) %
		iw->t[iw->sectors[iw->d.cs].cl.t]->w)), ((d->floor.y < 0) ? (((int)
		(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->
		sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1)
		: ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h)
			% iw->t[iw->sectors[iw->d.cs].cl.t]->h))),
			iw->sectors[iw->d.cs].light));
	}
}

void	draw_floor_ceil_tex5(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (!(iw->d.wallTop[d->j] > iw->d.top[left->x + d->j]))
		return ;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		draw_floor_ceil_tex5_1(iw, left, d);
		iw->d.top[left->x + d->j] = iw->d.wallTop[d->j];
	}
	else
	{
		d->sky_y = -iw->p.rotup + 2 * WINDOW_H;
		d->sky_y = (d->sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
		d->i = iw->d.top[left->x + d->j] - 1;
		d->sky_y += d->dy * iw->d.top[left->x + d->j];
		if (d->sky_x > iw->t[iw->l.skybox]->w)
			d->sky_x = d->sky_x - iw->t[iw->l.skybox]->w;
		while (++d->i < iw->d.wallTop[d->j] &&
			d->i < iw->d.bottom[left->x + d->j])
		{
			set_pixel2(iw->sur, d->j + left->x, d->i,
				get_pixel(iw->t[iw->l.skybox],
				(int)d->sky_x, (int)d->sky_y));
			d->sky_y += d->dy;
		}
	}
}

void	draw_floor_ceil_tex(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex	d;

	draw_floor_ceil_tex1(iw, left, right, &d);
	draw_floor_ceil_tex2(iw, left, &d);
	d.j = -1;
	while (++d.j < len)
	{
		if (iw->d.top[left->x + d.j] >= iw->d.bottom[left->x + d.j])
		{
			d.ang += d.dang;
			if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;
			continue;
		}
		draw_floor_ceil_tex3(iw, left, &d);
		if (iw->d.wallBot[d.j] < iw->d.bottom[left->x + d.j])
		{
			draw_floor_ceil_tex4(iw, left, &d);
			iw->d.bottom[left->x + d.j] = iw->d.wallBot[d.j];
		}
		draw_floor_ceil_tex5(iw, left, &d);
		if (iw->sectors[iw->d.cs].cl.t < 0)
			d.sky_x += d.dx;
		d.ang += d.dang;
	}
}
