/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_start_draw_vectors_lines.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:05:09 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:05:10 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_direction(t_sdl *iw)
{
	if (iw->p.rot > G90 && iw->p.rot < G90 * 3)
		iw->d.view_dir.x = -1;
	else
		iw->d.view_dir.x = 1;
	if (iw->p.rot < G180)
		iw->d.view_dir.y = -1;
	else
		iw->d.view_dir.y = 1;
}

void	get_screen_line(t_sdl *iw)
{
	float	na;

	na = iw->p.rot + G90;
	if (na > G360)
		na -= G360;
	na = get_k_angle(na);
	iw->d.screen.a = tanf(na);
	iw->d.screen.b = -1.0f;
	iw->d.screen.c = (float)iw->p.y - iw->d.screen.a * (float)iw->p.x;
	iw->d.screen_len = sqrtf(iw->d.screen.a * iw->d.screen.a
		+ iw->d.screen.b * iw->d.screen.b);
}

void	get_left_right_lines_points2(t_sdl *iw)
{
	float	na;
	float	nk;

	na = iw->p.rot + iw->v.angle;
	if (na > G360)
		na -= G360;
	nk = get_k_angle(na);
	iw->d.right_line.a = tanf(nk);
	iw->d.right_line.b = -1.0f;
	iw->d.right_line.c = (float)iw->p.y - iw->d.right_line.a * (float)iw->p.x;
	if (na < 180.0f * G1)
		iw->d.right_point.y = (float)iw->p.y - 1.0f;
	else
		iw->d.right_point.y = (float)iw->p.y + 1.0f;
	iw->d.right_point.x = (iw->d.right_line.b * iw->d.right_point.y
		+ iw->d.right_line.c) / (-iw->d.right_line.a);
}

void	get_left_right_lines_points(t_sdl *iw)
{
	float	na;
	float	nk;

	na = iw->p.rot - iw->v.angle;
	if (na < 0.0f)
		na += G360;
	nk = get_k_angle(na);
	iw->d.left_line.a = tanf(nk);
	iw->d.left_line.b = -1.0f;
	iw->d.left_line.c = (float)iw->p.y - iw->d.left_line.a * (float)iw->p.x;
	if (na < 180.0f * G1)
		iw->d.left_point.y = (float)iw->p.y - 1.0f;
	else
		iw->d.left_point.y = (float)iw->p.y + 1.0f;
	iw->d.left_point.x = (iw->d.left_line.b * iw->d.left_point.y +
		iw->d.left_line.c) / (-iw->d.left_line.a);
	get_left_right_lines_points2(iw);
}

float	get_k_angle(float rot)
{
	if (rot < G180)
		return (G180 - rot);
	else
		return (G360 - rot);
}
