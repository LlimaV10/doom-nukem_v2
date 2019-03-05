/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:03:43 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:03:44 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_sprite2(t_sdl *iw, t_sprite *sprite, t_draw_sprite *d)
{
	sprite->spriteheight = 2 * sprite->spritewidth * sprite->t->h / sprite->t->w;
	if (abs(sprite->z - iw->p.z) > 1500)
		sprite->spriteheight = (float)sprite->spriteheight * 1.0f /
		(float)(abs(sprite->z - iw->p.z) / 1500.0f);
	sprite->ey = WINDOW_H * (iw->p.z + (int)sprite->pplen / 2 - sprite->z)
		/ (int)sprite->pplen + iw->p.rotup;
	sprite->sy = sprite->ey - sprite->spriteheight;
	d->i = 0;
	d->stripe = sprite->sx - 1;
	if (d->stripe < -1)
	{
		d->i -= d->stripe + 1;
		d->stripe = -1;
	}
}

void	draw_sprite3(t_sdl *iw, t_sprite *sprite, t_draw_sprite *d)
{
	d->y = sprite->sy - 1;
	if (d->y < -1)
	{
		d->j -= d->y + 1;
		d->y = -1;
	}
	while (++d->y < sprite->ey && d->y < WINDOW_H)
	{
		if (sprite->sy < WINDOW_H && sprite->bottom[d->stripe] > d->y
			&& sprite->top[d->stripe] < d->y)
		{
			d->koef = (float)sprite->spriteheight / sprite->t->h;
			d->texY = (int)(d->j / d->koef);
			d->colour = get_pixel(sprite->t, d->texX, d->texY);
			if (d->colour != 0x010000)
				set_pixel2(iw->sur, d->stripe, d->y,
					get_light_color(d->colour, iw->sectors[sprite->num_sec].light));
		}
		d->j++;
	}
}

void	draw_sprite(t_sdl *iw, t_sprite *sprite)
{
	t_draw_sprite	d;

	draw_sprite2(iw, sprite, &d);
	while (++d.stripe < sprite->ex && d.stripe < WINDOW_W)
	{
		d.j = 0;
		d.koef = (float)sprite->spritewidth * 2.0f / (float)sprite->t->w;
		d.texX = (int)fabsf((float)d.i / d.koef);
		if ((sprite->top[d.stripe] < sprite->bottom[d.stripe])
			&& sprite->top[d.stripe] != -1)
			draw_sprite3(iw, sprite, &d);
		d.i++;
	}
}

void	draw_sprites(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (iw->sectors[tmp1->num_sec].visited && tmp1->draweble && tmp1->taken == 0)
		{
			if (tmp1->top[WINDOW_W / 2] != -1 && tmp1->sx
				< WINDOW_W / 2 && tmp1->ex > WINDOW_W / 2 &&
				tmp1->top[WINDOW_W / 2] < WINDOW_H / 2 &&
				tmp1->bottom[WINDOW_W / 2] > WINDOW_H / 2
				&& tmp1->sy < WINDOW_H / 2 && tmp1->ey > WINDOW_H / 2)
				iw->v.look_sprite = tmp1;
			draw_sprite(iw, tmp1);
		}
		tmp1 = tmp1->next;
	}
}
