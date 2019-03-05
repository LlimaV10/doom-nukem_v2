/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_sector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 18:34:08 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/04 18:34:09 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	sort_sector(t_doom *den)
{
	t_wals	*tmp;

	tmp = den->tmp;
	if (tmp->sec == den->sec - 1)
	{
		if (isleft_or_right(tmp) < 0)
			sort_walls_sec(tmp);
	}
	else
	{
		while (tmp)
		{
			if (tmp->sec == den->sec - 1)
			{
				if (isleft_or_right(tmp) < 0)
				{
					sort_walls_sec(tmp);
					break ;
				}
				else
					break ;
			}
			tmp = tmp->next;
		}
	}
}

void	add_sector_dir(t_wals *tmp, t_wals *last, t_sort *sort)
{
	float		ang;

	sort->l.a = (float)sort->v1.y;
	sort->l.b = (float)(-sort->v1.x);
	sort->l.c = -sort->l.a * (float)tmp->x - sort->l.b * (float)tmp->y;
	ang = G180 - acosf((sort->v1.x * sort->v2.x + sort->v1.y * sort->v2.y) /
		sqrtf(sort->v1.x * sort->v1.x + sort->v1.y * sort->v1.y) /
		sqrtf(sort->v2.x * sort->v2.x + sort->v2.y * sort->v2.y));
	if (sort->l.a * (float)last->x +
		sort->l.b * (float)last->y + sort->l.c > 0)
		sort->sector_dir += ang;
	else
		sort->sector_dir -= ang;
}

float	isleft_or_right(t_wals *list)
{
	t_wals			*tmp;
	t_sort			sort;

	sort.sector_dir = 0.0f;
	tmp = list;
	while (tmp->next->next)
	{
		sort.v1.x = tmp->x - tmp->next->x;
		sort.v1.y = tmp->y - tmp->next->y;
		sort.v2.x = tmp->next->next->x - tmp->next->x;
		sort.v2.y = tmp->next->next->y - tmp->next->y;
		add_sector_dir(tmp, tmp->next->next, &sort);
		tmp = tmp->next;
	}
	sort.v1.x = tmp->x - tmp->next->x;
	sort.v1.y = tmp->y - tmp->next->y;
	sort.v2.x = list->x - tmp->next->x;
	sort.v2.y = list->y - tmp->next->y;
	add_sector_dir(tmp, list, &sort);
	sort.v1.x = tmp->next->x - list->x;
	sort.v1.y = tmp->next->y - list->y;
	sort.v2.x = list->next->x - list->x;
	sort.v2.y = list->next->y - list->y;
	add_sector_dir(tmp->next, list->next, &sort);
	return (sort.sector_dir);
}

void	sort_list_incide(t_doom *den)
{
	t_wals	*rmp;

	rmp = den->tmp;
	while (den->tmp->next)
	{
		if (den->tmp->inside == 1 && den->tmp->sec == den->sec - 1)
		{
			if (isleft_or_right(den->tmp) > 0)
			{
				sort_walls_inside(den);
				break ;
			}
		}
		den->tmp = den->tmp->next;
	}
	den->tmp = rmp;
}

void	check_data_sector(t_doom *den)
{
	t_wals	*rmp;

	rmp = den->tmp;
	while (den->tmp)
	{
		if (den->tmp->sec == den->sec - 1)
		{
			den->tmp->inside += den->xy.inside;
			den->tmp->sec = den->xy.bool_cor;
		}
		den->tmp = den->tmp->next;
	}
	den->tmp = rmp;
	den->sec -= 1;
	den->secbak -= 1;
}
