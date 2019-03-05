/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_between_sectors_bot_tex.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:02:09 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:02:10 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_between_sectors_bot_tex1(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_wall_tex *d)
{
	d->lv.x = (float)(left->p.x - iw->p.x);
	d->lv.y = (float)(left->p.y - iw->p.y);
	d->rv.x = (float)(right->p.x - iw->p.x);
	d->rv.y = (float)(right->p.y - iw->p.y);
	d->ang = acosf((d->lv.x * d->rv.x + d->lv.y * d->rv.y) /
		(sqrtf(d->lv.x * d->lv.x + d->lv.y * d->lv.y) *
			sqrtf(d->rv.x * d->rv.x + d->rv.y * d->rv.y)));
	d->dang = d->ang / (float)(right->x - left->x);
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
	d->left_len = 0.0f;
	d->tx = left->olen * (float)iw->t[left->wall->t]->w
		* iw->tsz[left->wall->t] / 1000.0f;
	while (d->tx >= (float)iw->t[left->wall->t]->w)
		d->tx -= (float)iw->t[left->wall->t]->w;
}

void	draw_between_sectors_bot_tex2_1(t_sdl *iw, t_save_wall *left,
	int *tmp, t_draw_wall_tex *d)
{
	iw->d.bottom[left->x + d->j] = tmp[d->j];
	d->ang += d->dang;
	d->left_len = sinf(d->ang) * d->lenpl / sinf(d->sing - d->ang);
	d->tx = (left->olen + d->left_len) * (float)iw->t[left->wall->t]->w
		* iw->tsz[left->wall->t] / 1000.0f;
	while (d->tx > (float)iw->t[left->wall->t]->w)
		d->tx -= (float)iw->t[left->wall->t]->w;
}

void	draw_between_sectors_bot_tex2(t_sdl *iw, t_save_wall *left,
	int *tmp, t_draw_wall_tex *d)
{
	int		i;

	d->ty = d->ty * (float)iw->t[left->wall->t]->h / 1000.0f;
	d->dty = ((d->zu - d->zd) * (float)iw->t[left->wall->t]->h / 1000.0f) /
		(float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j])
		* iw->tsz[left->wall->t];
	while (d->ty > (float)iw->t[left->wall->t]->h)
		d->ty -= (float)iw->t[left->wall->t]->h;
	if (tmp[d->j] > iw->d.top[d->j + left->x])
		i = tmp[d->j] - 1;
	else
	{
		i = iw->d.top[d->j + left->x];
		d->ty += d->dty * (float)(iw->d.top[d->j + left->x] - tmp[d->j]);
	}
	while (++i < iw->d.bottom[left->x + d->j])
	{
		set_pixel2(iw->sur, left->x + d->j, i,
			get_light_color(get_pixel(iw->t[left->wall->t], (int)d->tx,
			(int)d->ty), iw->sectors[iw->visited_sectors->sec].light));
		d->ty += d->dty;
		while (d->ty >= (float)iw->t[left->wall->t]->h)
			d->ty -= (float)iw->t[left->wall->t]->h;
	}
	draw_between_sectors_bot_tex2_1(iw, left, tmp, d);
}

void	draw_between_sectors_bot_tex(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int *tmp)
{
	t_draw_wall_tex	d;

	if (left->wall->t < 0)
		return ;
	draw_between_sectors_bot_tex1(iw, left, right, &d);
	d.j = -1;
	while (++d.j < right->x - left->x)
	{
		if (iw->d.top[left->x + d.j] >= iw->d.bottom[left->x + d.j] ||
			iw->d.bottom[left->x + d.j] <= tmp[d.j])
		{
			d.ang += d.dang;
			continue;
		}
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[d.j] < tmp[d.j])
			d.ty = d.zu + iw->tsz[left->wall->t] *
			(d.zu - d.zd) * (float)(tmp[d.j] - iw->d.wallTop[d.j]) /
			(float)(iw->d.wallBot[d.j] - iw->d.wallTop[d.j]);
		else
			d.ty = d.zu;
		draw_between_sectors_bot_tex2(iw, left, tmp, &d);
	}
}
