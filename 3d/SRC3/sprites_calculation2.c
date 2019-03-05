/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_calculation2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:16:09 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 17:16:10 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		find_point(t_save_wall_pairs *tmp, t_sprite *tmp1)
{
	int		i;
	int		ax;
	int		ay;
	int		bx;
	int		by;

	ax = tmp->right->p.x - tmp->left->p.x;
	ay = tmp->right->p.y - tmp->left->p.y;
	bx = tmp1->x - tmp->left->p.x;
	by = tmp1->y - tmp->left->p.y;
	i = ax * by - ay * bx;
	if (i > 0)
		return (0);
	else
		return (1);
}

void	calculate_sprites_once2(t_sdl *iw, t_sprite *tmp1)
{
	float lang;
	float rang;

	lang = get_vectors_angle(iw->d.left_point.x -
		(float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(tmp1->x - iw->p.x), (float)(tmp1->y - iw->p.y));
	rang = get_vectors_angle(iw->d.right_point.x -
		(float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(tmp1->x - iw->p.x), (float)(tmp1->y - iw->p.y));
	tmp1->x_s = (int)(lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	if (rang > 2.0f * iw->v.angle)
		tmp1->x_s = -tmp1->x_s;
	tmp1->plen = sqrtf(powf((float)(iw->p.x - tmp1->x), 2.0f)
		+ powf((float)(iw->p.y - tmp1->y), 2.0f)) + 1.0f;
	tmp1->pplen = fabsf(iw->d.screen.a * (float)tmp1->x +
		iw->d.screen.b * (float)tmp1->y + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a +
			iw->d.screen.b * iw->d.screen.b) + 1.0f;
	if (tmp1->pplen / tmp1->plen >= 0.5f)
		tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w)
			/ tmp1->pplen) * tmp1->scale);
	else
		tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w)
			/ tmp1->plen) * tmp1->scale);
	tmp1->sx = tmp1->x_s - tmp1->spritewidth;
}

void	calculate_sprites_once(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (!iw->sectors[tmp1->num_sec].visited || tmp1->draweble == 1)
		{
			tmp1 = tmp1->next;
			continue;
		}
		calculate_sprites_once2(iw, tmp1);
		tmp1->ex = tmp1->x_s + tmp1->spritewidth;
		if (!(tmp1->sx > iw->d.screen_right || tmp1->ex < iw->d.screen_left))
			tmp1->draweble = 1;
		tmp1 = tmp1->next;
	}
}
