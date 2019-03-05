/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_left_right_visible_walls2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:11:03 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 17:11:04 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_all_intersection_line(t_sdl *iw, t_line2d *nl, int right)
{
	int				wall;
	t_intpoint2d	p;

	wall = iw->sectors[iw->d.cs].sw - 1;
	while (++wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
		if (if_not_in_vw(iw, ((right == 0) ?
			&iw->walls[wall] : iw->walls[wall].next))
			&& visible_wall(iw, wall) &&
			cross_two_lines(nl, &iw->walls[wall].l, &p)
			&& point_in_front_and_on_wall(iw, &p, wall))
			add_lr_wall(iw, &p, ((right == 0) ? &iw->walls[wall]
				: iw->walls[wall].next), right * WINDOW_W);
}

void	get_left_right_visible_walls(t_sdl *iw)
{
	get_all_intersection_line(iw, &iw->d.left_line, 0);
	get_all_intersection_line(iw, &iw->d.right_line, 1);
}
