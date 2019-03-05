/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_delete_picture2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:13:02 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 15:13:03 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	calculate_picture2(t_wall *wall, t_picture *pic)
{
	if (wall->y == wall->next->y)
	{
		pic->y1 = wall->y;
		pic->y0 = wall->y;
		if (wall->x > wall->next->x)
		{
			pic->x1 = wall->x - pic->left_plus;
			pic->x0 = pic->x1 - pic->tw;
		}
		else
		{
			pic->x1 = wall->x + pic->left_plus;
			pic->x0 = pic->x1 + pic->tw;
		}
	}
	else
		calculate_not_squared_wall_picture(wall, pic);
}

void	calculate_picture(t_sdl *iw, t_wall *wall, t_picture *pic)
{
	if (wall->x == wall->next->x)
	{
		pic->x1 = wall->x;
		pic->x0 = wall->x;
		if (wall->y > wall->next->y)
		{
			pic->y1 = wall->y - pic->left_plus;
			pic->y0 = pic->y1 - pic->tw;
		}
		else
		{
			pic->y1 = wall->y + pic->left_plus;
			pic->y0 = pic->y1 + pic->tw;
		}
	}
	else
		calculate_picture2(wall, pic);
	pic->zd = pic->zu - pic->tw * iw->t[pic->t]->h
		* 120 / iw->t[pic->t]->w / 100;
}

void	calculate_not_squared_wall_picture(t_wall *wall, t_picture *pic)
{
	float	alpha;

	alpha = get_vectors_angle(wall->next->x - wall->x, wall->next->y - wall->y,
		((wall->next->x > wall->x) ? 10 : -10), 0);
	pic->x1 = (float)wall->x + (float)pic->left_plus * cosf(alpha) *
		((wall->next->x > wall->x) ? 1.0f : -1.0f);
	pic->y1 = (float)wall->y + (float)pic->left_plus * sinf(alpha) *
		((wall->next->y > wall->y) ? 1.0f : -1.0f);
	pic->x0 = (float)wall->x + (float)(pic->left_plus + pic->tw) * cosf(alpha) *
		((wall->next->x > wall->x) ? 1.0f : -1.0f);
	pic->y0 = (float)wall->y + (float)(pic->left_plus + pic->tw) * sinf(alpha) *
		((wall->next->y > wall->y) ? 1.0f : -1.0f);
}
