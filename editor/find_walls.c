/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 16:47:58 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/05 16:47:59 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	find_wals_help(t_doom *den)
{
	if (den->rec.x1 == 0 && den->rec.x2 == 0)
	{
		den->rec.x1 = den->tmp->x * XSKAPE;
		den->rec.y1 = den->tmp->y * YSKAPE;
		den->rec.x2 = den->tmp->x1 * XSKAPE;
		den->rec.y2 = den->tmp->y1 * YSKAPE;
		den->rec.sect = den->tmp->sec;
		den->find_tmp_one = den->tmp;
		den->change = 1;
	}
	else if (den->rec_two.x1 == 0 && den->rec_two.x2 == 0)
	{
		den->rec_two.x1 = den->tmp->x * XSKAPE;
		den->rec_two.y1 = den->tmp->y * YSKAPE;
		den->rec_two.x2 = den->tmp->x1 * XSKAPE;
		den->rec_two.y2 = den->tmp->y1 * YSKAPE;
		den->rec_two.sect = den->tmp->sec;
		den->find_tmp_two = den->tmp;
	}
	drow_find(den);
}

void	find_wals(int x, int y, t_doom *den)
{
	t_wals	*rmp;
	int		i;

	i = 0;
	rmp = den->tmp;
	if (den->change == 0)
	{
		while (den->tmp)
		{
			i = point_into_segments(x, y, den);
			if (i >= -3000 && i <= 3000)
				find_wals_help(den);
			den->tmp = den->tmp->next;
		}
		den->tmp = rmp;
	}
	else
	{
		ft_line(den, &den->rec);
		ft_line(den, &den->rec_two);
		SDL_UpdateWindowSurface(den->window);
		clear_find(den);
	}
}

void	look_find_sector_two(t_doom *den)
{
	t_wals	*rmp;

	rmp = den->tmp;
	while (rmp)
	{
		if (rmp != den->find_tmp && ((rmp->x == den->find_tmp->x1 && rmp->y ==
			den->find_tmp->y1 && rmp->x1 == den->find_tmp->x &&
			rmp->y1 == den->find_tmp->y) || (rmp->x == den->find_tmp->x &&
			rmp->y == den->find_tmp->y && rmp->x1 == den->find_tmp->x1 &&
			rmp->y1 == den->find_tmp->y1)))
		{
			den->find_tmp_two = rmp;
			change_rec(den, &den->rec_two, den->find_tmp_two);
			break ;
		}
		else
		{
			den->find_tmp_two = NULL;
		}
		rmp = rmp->next;
	}
}

void	clear_find(t_doom *den)
{
	den->rec.x1 = 0;
	den->rec.y1 = 0;
	den->rec.x2 = 0;
	den->rec.y2 = 0;
	den->rec.sect = 0;
	den->find_tmp_one = 0;
	den->change = 0;
	den->rec_two.x1 = 0;
	den->rec_two.y1 = 0;
	den->rec_two.x2 = 0;
	den->rec_two.y2 = 0;
	den->rec_two.sect = 0;
	den->find_tmp_two = 0;
}

int		point_into_segments(int x, int y, t_doom *den)
{
	int	x1;
	int	y1;
	int	x2;
	int	y2;
	int	skap;

	skap = 10;
	if (den->skape < 0)
		skap = 3;
	x1 = den->tmp->x * XSKAPE;
	y1 = den->tmp->y * YSKAPE;
	x2 = den->tmp->x1 * XSKAPE;
	y2 = den->tmp->y1 * YSKAPE;
	if ((x1 - skap > x && x < x2 + skap) || (x2 - skap < x && x > x1 + skap))
		return (3001);
	if ((y1 - skap > y && y < y2 + skap) || (y2 - skap < y && y > y1 + skap))
		return (3001);
	return (((x - x1) * (y2 - y1)) - (y - y1) * (x2 - x1));
}
