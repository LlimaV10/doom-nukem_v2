/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_left_right_visible_walls.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:04:44 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:04:45 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		if_not_in_vw(t_sdl *iw, t_wall *wall)
{
	t_save_wall	*tmp;

	tmp = iw->d.vw;
	while (tmp != 0)
	{
		if (tmp->wall == wall)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	add_lr_wall(t_sdl *iw, t_intpoint2d *p, t_wall *wall, int x)
{
	t_save_wall	*tmp;

	tmp = (t_save_wall *)malloc(sizeof(t_save_wall));
	tmp->x = x;
	tmp->wall = wall;
	tmp->len = sqrtf(powf((float)(iw->p.x - p->x), 2.0f)
		+ powf((float)(iw->p.y - p->y), 2.0f));
	tmp->plen = fabsf(iw->d.screen.a * (float)p->x +
		iw->d.screen.b * (float)p->y + iw->d.screen.c) / sqrtf(
			iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	if ((int)tmp->plen == 0)
		tmp->plen = 1.0f;
	tmp->olen = sqrtf(powf(p->x - wall->x, 2.0f) + powf(p->y - wall->y, 2.0f));
	tmp->p = *p;
	tmp->zd = get_floor_z(iw, p->x, p->y);
	tmp->zu = get_ceil_z(iw, p->x, p->y);
	tmp->next = 0;
	add_wall(iw, tmp);
}

int		visible_wall(t_sdl *iw, int wall)
{
	if ((iw->p.x - iw->walls[wall].x) * (iw->walls[wall].next->y
		- iw->walls[wall].y) -
		(iw->p.y - iw->walls[wall].y) * (iw->walls[wall].next->x
			- iw->walls[wall].x) >= 0)
		return (1);
	return (0);
}

int		point_in_front_and_on_wall(t_sdl *iw, t_intpoint2d *p, int wall)
{
	float	side;

	side = iw->d.screen.a * p->x + iw->d.screen.b * p->y + iw->d.screen.c;
	if (side * iw->d.view_dir.y < 0)
		if ((p->x >= ft_min(iw->walls[wall].x, iw->walls[wall].next->x) &&
			p->x <= ft_max(iw->walls[wall].x, iw->walls[wall].next->x))
			&& (p->y >= ft_min(iw->walls[wall].y, iw->walls[wall].next->y) &&
				p->y <= ft_max(iw->walls[wall].y, iw->walls[wall].next->y)))
			return (1);
	return (0);
}

int		cross_two_lines(t_line2d *l1, t_line2d *l2, t_intpoint2d *p)
{
	t_line2d	*tmp;
	float		py;

	if (l1->a == l2->a && l1->b == l2->b)
		return (0);
	if (l1->a == 0 && l2->a != 0)
	{
		tmp = l1;
		l1 = l2;
		l2 = tmp;
	}
	else if (l1->a == 0)
		return (0);
	py = (l2->a * l1->c - l1->a * l2->c) / (l1->a * l2->b - l2->a * l1->b);
	p->y = (int)roundf(py);
	if (l2->b == 0 && l2->a != 0)
		p->x = (int)roundf((l2->b * py + l2->c) / (-l2->a));
	else
		p->x = (int)roundf((l1->b * py + l1->c) / (-l1->a));
	return (1);
}
