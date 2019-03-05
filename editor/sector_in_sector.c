/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_in_sector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:37:10 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/05 17:37:11 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	sector_in_sector_two(t_doom *den, t_wals *cmp, t_wals *lmp)
{
	while (den->tmp->next)
	{
		if (den->tmp->sec == den->xy.bool_cor && den->tmp->next->sec
			!= den->xy.bool_cor)
		{
			den->tmp->next = lmp->next;
			while (den->tmp->next)
				den->tmp = den->tmp->next;
			den->tmp->next = cmp;
			while (den->tmp)
			{
				if (den->tmp->next == lmp->next)
					den->tmp->next = NULL;
				den->tmp = den->tmp->next;
			}
			return ;
		}
		den->tmp = den->tmp->next;
	}
}

void	sector_in_sector(t_doom *den)
{
	t_wals	*rmp;
	t_wals	*cmp;
	t_wals	*lmp;

	cmp = NULL;
	rmp = den->tmp;
	if (den->sec > 2)
	{
		while (den->tmp->next)
		{
			if (den->tmp->next->sec == den->xy.bool_cor &&
				den->tmp->next->next->sec != den->xy.bool_cor)
				cmp = den->tmp->next->next;
			if (den->tmp->next->sec == den->sec - 1)
			{
				lmp = den->tmp;
				den->tmp = rmp;
				sector_in_sector_two(den, cmp, lmp);
				break ;
			}
			den->tmp = den->tmp->next;
		}
	}
	den->tmp = rmp;
}

void	sort_walls_inside_help(t_doom *den, int *walls, int i, t_wals *rmp)
{
	while (rmp && rmp->inside == 1)
	{
		walls[i] = rmp->x;
		walls[i + 1] = rmp->y;
		walls[i + 2] = rmp->x1;
		walls[i + 3] = rmp->y1;
		i += 4;
		rmp = rmp->next;
	}
	--i;
	while (den->tmp && den->tmp->inside == 1)
	{
		den->tmp->y = walls[i];
		den->tmp->x = walls[i - 1];
		den->tmp->y1 = walls[i - 2];
		den->tmp->x1 = walls[i - 3];
		i -= 4;
		den->tmp = den->tmp->next;
	}
	free(walls);
}

void	sort_walls_inside(t_doom *den)
{
	t_wals	*rmp;
	int		i;
	int		*walls;

	i = 0;
	rmp = den->tmp;
	while (rmp && rmp->inside == 1)
	{
		i++;
		rmp = rmp->next;
	}
	rmp = den->tmp;
	walls = (int *)malloc(sizeof(int) * (i * 4));
	i = 0;
	sort_walls_inside_help(den, walls, i, rmp);
}

int		inside_sect(t_doom *den, t_wals **tmp, int sec)
{
	t_insect	in;

	in.wallcrossed = 0;
	while (*tmp)
	{
		den->xy.inside = (*tmp)->inside;
		if ((*tmp)->sec != sec)
			break ;
		in.y1 = (*tmp)->y - den->xy.y;
		in.y2 = (*tmp)->y1 - den->xy.y;
		if ((in.y1 ^ in.y2) < 0)
		{
			in.x1 = (*tmp)->x - den->xy.x;
			in.x2 = (*tmp)->x1 - den->xy.x;
			if ((in.x1 ^ in.x2) >= 0)
				in.wallcrossed ^= in.x1;
			else
				in.wallcrossed ^= (in.x1 * in.y2 - in.x2 * in.y1) ^ in.y2;
		}
		if ((*tmp)->next == NULL || ((*tmp)->next->sec != sec))
			break ;
		if ((*tmp)->next != NULL)
			(*tmp) = (*tmp)->next;
	}
	return (in.wallcrossed >> 31);
}
