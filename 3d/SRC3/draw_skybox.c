/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:03:36 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:03:37 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_skybox2(t_sdl *iw, t_draw_skybox *d)
{
	d->rot = iw->p.rot - iw->v.angle;
	if (d->rot < 0.0f)
		d->rot += G360;
	d->dx = (float)iw->t[iw->l.skybox]->w /
		(G360 / (iw->v.angle * 2) * WINDOW_W);
	d->sky_x = d->rot * ((float)iw->t[iw->l.skybox]->w) / G360
		+ d->dx * (float)iw->d.screen_left;
	d->dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
}

void	draw_skybox(t_sdl *iw)
{
	int				j;
	int				i;
	t_draw_skybox	d;

	draw_skybox2(iw, &d);
	j = iw->d.screen_left - 1;
	while (++j < iw->d.screen_right)
	{
		d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
		d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
		d.sky_y += d.dy * iw->d.top[j];
		i = iw->d.top[j] - 1;
		while (++i <= iw->d.bottom[j] && i < WINDOW_H)
		{
			set_pixel2(iw->sur, j, i,
				get_pixel(iw->t[iw->l.skybox],
				(int)d.sky_x, (int)d.sky_y));
			d.sky_y += d.dy;
		}
		d.sky_x += d.dx;
		if (d.sky_x >= iw->t[iw->l.skybox]->w)
			d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
		iw->d.bottom[j] = iw->d.top[j];
	}
}
