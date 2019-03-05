/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_glass.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:02:52 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:02:53 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_glass_tex1(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_glass *d)
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
}

void	draw_glass_tex2(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_glass *d)
{
	d->nleft_zd = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].
		next->x, iw->walls[left->wall->nextsector_wall].next->y);
	if (left->zu < d->nleft_zu)
	{
		d->zudiff = (right->zu - left->zu) / d->len_lr;
		d->nleft_zu = left->zu;
	}
	else
	{
		d->nright_zu = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x,
			iw->walls[left->wall->nextsector_wall].y);
		d->zudiff = (d->nright_zu - d->nleft_zu) / d->len_lr;
	}
	if (left->zd > d->nleft_zd)
	{
		d->zddiff = (right->zd - left->zd) / d->len_lr;
		d->nleft_zd = left->zd;
	}
	else
	{
		d->nright_zd = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x,
			iw->walls[left->wall->nextsector_wall].y);
		d->zddiff = (d->nright_zd - d->nleft_zd) / d->len_lr;
	}
}

void	draw_glass_tex3(t_sdl *iw, t_save_wall *left,
	t_draw_glass *d)
{
	d->zd = (float)d->nleft_zd + d->left_len * d->zddiff;
	d->dty = ((d->zu - d->zd) * (float)iw->t[left->wall->glass]->h / 1000.0f) /
		(float)(iw->d.save_bot_betw[d->j] - iw->d.save_top_betw[d->j])
		* iw->tsz[left->wall->glass];
	d->ty = 0.0f;
	if (iw->d.save_top_betw[d->j] < iw->d.top[left->x + d->j])
	{
		d->ty += (float)(iw->d.top[left->x + d->j] -
			iw->d.save_top_betw[d->j]) * d->dty;
		d->i = iw->d.top[left->x + d->j] - 1;
	}
	else
		d->i = iw->d.save_top_betw[d->j] - 1;
	while (++d->i < iw->d.save_bot_betw[d->j]
		&& d->i < iw->d.bottom[d->j + left->x])
	{
		d->pixel = get_pixel(iw->t[left->wall->glass],
			(int)d->tx % iw->t[left->wall->glass]->w,
			(int)d->ty % iw->t[left->wall->glass]->h);
		if (d->pixel != 0x010000)
			set_pixel2(iw->sur, left->x + d->j, d->i,
				get_light_color(d->pixel, iw->sectors[iw->d.cs].light));
		d->ty += d->dty;
	}
	d->ang += d->dang;
}

void	draw_glass_tex(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int len)
{
	t_draw_glass	d;

	draw_glass_tex1(iw, left, right, &d);
	d.i = iw->d.cs;
	iw->d.cs = left->wall->nextsector;
	d.nleft_zu = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x,
		iw->walls[left->wall->nextsector_wall].next->y);
	draw_glass_tex2(iw, left, right, &d);
	iw->d.cs = d.i;
	d.j = -1;
	while (++d.j < len)
	{
		if (iw->d.top[left->x + d.j] >= iw->d.bottom[left->x + d.j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->glass]->w
			* iw->tsz[left->wall->glass] / 1000.0f;
		d.zu = (float)d.nleft_zu + d.left_len * d.zudiff;
		draw_glass_tex3(iw, left, &d);
	}
}

void	draw_glass_sprites(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (tmp1->num_sec == iw->d.cs && tmp1->draweble)
		{
			draw_sprite(iw, tmp1);
			tmp1->draweble = 0;
		}
		tmp1 = tmp1->next;
	}
}
