/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_floor_ceil2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:13:21 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 17:13:22 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_wall_floor_ceil_tex1(t_sdl *iw, t_save_wall *left,
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
	d->lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) +
		powf(iw->p.y - left->p.y, 2.0f));
	d->len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) +
		powf(left->p.y - right->p.y, 2.0f));
	d->rv.x = (float)(right->p.x - left->p.x) / d->len_lr;
	d->rv.y = (float)(right->p.y - left->p.y) / d->len_lr;
	d->zu = get_ceil_z(iw, 0, 0);
	d->zd = get_floor_z(iw, 0, 0);
	d->frpl = (float)(d->zu - d->zd) / (float)(iw->p.z - d->zd);
	d->clpl = (float)(d->zu - d->zd) / (float)(d->zu - iw->p.z);
	d->px = (float)iw->p.x / 1000.0f;
}

void	draw_wall_floor_ceil_tex2(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->left_len = sinf(d->ang) * d->lenpl / sinf(d->sing - d->ang);
	d->r.x = (float)left->p.x + d->rv.x * d->left_len;
	d->r.y = (float)left->p.y + d->rv.y * d->left_len;
	d->wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d->r.x
		+ iw->d.screen.b * d->r.y + iw->d.screen.c) /
		iw->d.screen_len) * d->frcoef;
	d->r.x /= 1000.0f;
	d->r.y /= 1000.0f;
}
