/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_delete_picture.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 14:56:20 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 14:57:48 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	add_picture(t_sdl *iw, t_wall *wall)
{
	t_picture	*tmp;

	if (iw->v.submenu_mode != 0 || iw->v.f_button_mode != 0)
		return ;
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 500;
	tmp->zu = get_ceil_z(iw, wall->x, wall->y) - 100;
	tmp->tw = 500;
	tmp->t = iw->v.tex_to_fill;
	tmp->next = wall->p;
	wall->p = tmp;
	if (iw->v.tex_to_fill == 17 || iw->v.tex_to_fill == 18)
	{
		iw->v.f_button_mode = 1;
		iw->v.f_button_pointer = (void *)tmp;
	}
	else if (iw->v.tex_to_fill == 19)
	{
		iw->v.submenu_mode = 4;
		draw_submenu(iw);
		iw->v.f_button_pointer = (void *)tmp;
	}
	calculate_picture(iw, wall, tmp);
}

void	delete_wall_animation1(t_sdl *iw, t_wall_animation *tmp2)
{
	do_wall_animation_step_dx(iw, tmp2, -tmp2->curr_dx);
	do_wall_animation_step_dy(iw, tmp2, -tmp2->curr_dy);
	iw->wall_animations = iw->wall_animations->next;
	free(tmp2);
}

void	delete_wall_animation(t_sdl *iw, t_picture *pic)
{
	t_wall_animation	*tmp;
	t_wall_animation	*tmp2;

	if (iw->wall_animations == 0)
		return ;
	if (iw->wall_animations->trigger == pic)
	{
		delete_wall_animation1(iw, iw->wall_animations);
		return ;
	}
	tmp = iw->wall_animations;
	while (tmp->next)
	{
		if (tmp->next->trigger == pic)
		{
			tmp2 = tmp->next;
			do_wall_animation_step_dx(iw, tmp2, -tmp2->curr_dx);
			do_wall_animation_step_dy(iw, tmp2, -tmp2->curr_dy);
			tmp->next = tmp->next->next;
			free(tmp2);
			return ;
		}
		tmp = tmp->next;
	}
}

void	delete_light_and_animations(t_sdl *iw, t_picture *pic)
{
	int					sec;
	t_sector_animation	a;
	t_sector_animation	*tmp;
	t_sector_animation	*tmp2;

	sec = -1;
	while (++sec < iw->v.sc)
		if (iw->sectors[sec].light == pic)
			iw->sectors[sec].light = 0;
	a.next = iw->sector_animations;
	tmp = &a;
	while (tmp->next)
	{
		if (tmp->next->trigger == pic)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			do_sector_animation_step(iw, tmp2, -tmp2->curr_dy);
			free(tmp2);
		}
		else
			tmp = tmp->next;
	}
	iw->sector_animations = a.next;
	delete_wall_animation(iw, pic);
}

void	delete_picture(t_wall *wall, t_picture *pic, t_sdl *iw)
{
	t_picture	*tmp;

	if (iw->v.submenu_mode != 0 || iw->v.f_button_mode != 0)
		return ;
	delete_light_and_animations(iw, pic);
	if (pic == wall->p)
	{
		wall->p = wall->p->next;
		free(pic);
	}
	else
	{
		tmp = wall->p;
		while (tmp->next != 0)
		{
			if (tmp->next == pic)
				break ;
			tmp = tmp->next;
		}
		if (tmp->next != 0)
		{
			tmp->next = tmp->next->next;
			free(pic);
		}
	}
}

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
