/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 17:38:17 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/20 17:38:18 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	change_rec(t_doom *den, t_col *rec, t_wals *tmp)
{
	rec->x1 = tmp->x * XSKAPE;
	rec->y1 = tmp->y * YSKAPE;
	rec->x2 = tmp->x1 * XSKAPE;
	rec->y2 = tmp->y1 * YSKAPE;
}

void	next_wall_change(t_doom *den, t_wals *tmp, t_wals *rmp)
{
	den->color = 0xFFFFFF;
	if (rmp->nextsector_wall)
		den->color = 0xFF0000;
	ft_line(den, &den->rec);
	den->rec.x1 = tmp->x * XSKAPE;
	den->rec.y1 = tmp->y * YSKAPE;
	den->rec.x2 = tmp->x1 * XSKAPE;
	den->rec.y2 = tmp->y1 * YSKAPE;
	den->color = 0x00FF00;
	ft_line(den, &den->rec);
}

void	next_wall(t_doom *den)
{
	t_wals	*tmp;

	tmp = den->tmp;
	if (den->find_tmp->next && den->find_tmp->next->sec == den->rec.sect)
	{
		next_wall_change(den, den->find_tmp->next, den->find_tmp);
		den->find_tmp = den->find_tmp->next;
		den->find_tmp_one = den->find_tmp;
	}
	else
	{
		while (tmp)
		{
			if (tmp->sec == den->rec.sect)
			{
				den->find_tmp = tmp;
				den->find_tmp_one = den->find_tmp;
				next_wall_change(den, den->find_tmp, den->find_tmp);
				break ;
			}
			tmp = tmp->next;
		}
	}
	look_find_sector_two(den);
	den->color = 0xFFFFFF;
}

void	back_wall_check(t_doom *den, t_wals *rmp, t_wals *lmp)
{
	while (den->tmp->next->next != 0)
		den->tmp = den->tmp->next;
	if (den->tmp->blouk != 1)
	{
		rmp = den->tmp->next;
		den->tmp->next = NULL;
		free(rmp);
		den->walls -= 1;
	}
	else if (den->tmp->sec != den->tmp->next->sec)
	{
		rmp = den->tmp->next;
		den->tmp->next = NULL;
		free(rmp);
		den->blouk = 1;
		den->walls -= 1;
	}
	den->tmp = lmp;
}

void	back_wall(t_doom *den, t_col *vec)
{
	t_wals	*rmp;
	t_wals	*lmp;

	lmp = den->tmp;
	rmp = den->tmp;
	if (den->tmp)
	{
		if (den->tmp->next != 0)
			back_wall_check(den, rmp, lmp);
		else if (den->tmp->next == 0)
		{
			rmp = den->tmp;
			free(rmp);
			den->tmp = NULL;
			den->blouk = 1;
			den->walls -= 1;
		}
	}
	retry_write(den, vec);
}
