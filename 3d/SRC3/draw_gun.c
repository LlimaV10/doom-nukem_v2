/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_gun.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:03:01 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:03:02 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_gun2(t_sdl *iw, t_draw_gun *d)
{
	d->changed_rect = iw->guns.t_rect[iw->guns.t];
	d->changed_rect.x += iw->v.weapon_change_x;
	d->changed_rect.y += iw->v.weapon_change_y + iw->v.weapon_change_y_hide;
	d->to_i = d->changed_rect.y + d->changed_rect.h;
	if (d->to_i > WINDOW_H)
		d->to_i = WINDOW_H;
	if (d->changed_rect.y < 0)
		d->i = -1;
	else
		d->i = d->changed_rect.y - 1;
	if (d->changed_rect.x < 0)
		d->start_j = -1;
	else
		d->start_j = d->changed_rect.x - 1;
	d->to_j = d->changed_rect.w + d->changed_rect.x;
	if (d->to_j > WINDOW_W)
		d->to_j = WINDOW_W;
}

void	draw_gun(t_sdl *iw)
{
	t_draw_gun	d;

	draw_gun2(iw, &d);
	while (++d.i < d.to_i)
	{
		d.j = d.start_j;
		while (++d.j < d.to_j)
		{
			d.pixel = get_pixel(iw->t_weap[iw->guns.t],
				(d.j - d.changed_rect.x) * iw->t_weap[iw->guns.t]->w
				/ d.changed_rect.w,
				(d.i - d.changed_rect.y) * iw->t_weap[iw->guns.t]->h
				/ d.changed_rect.h);
			if (d.pixel != 0x010000)
				set_pixel2(iw->sur, d.j, d.i,
					get_light_color(d.pixel, iw->sectors[iw->d.cs].light));
		}
	}
}
