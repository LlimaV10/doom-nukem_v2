/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 15:12:57 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/20 15:12:58 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void				add_new_sector(t_doom *den)
{
	t_sector_list	*nsec;
	t_sector_list	*tmp;

	nsec = (t_sector_list *)malloc(sizeof(t_sector_list));
	nsec->next = 0;
	nsec->fr.z = 0;
	nsec->fr.n = 0;
	nsec->fr.t = 1;
	nsec->cl.z = 2000;
	nsec->cl.n = 0;
	nsec->cl.t = 3;
	nsec->light = 0;
	if (den->sectors == 0)
		den->sectors = nsec;
	else
	{
		tmp = den->sectors;
		while (tmp->next != 0)
			tmp = tmp->next;
		tmp->next = nsec;
	}
}

void				change_way2(t_doom *den, t_wals *first_element,
												t_wals **first_element2)
{
	int				tmp;

	if (first_element != 0)
		change_way2(den, first_element->next, first_element2);
	else
		return ;
	tmp = first_element->x;
	first_element->x = first_element->x1;
	first_element->x1 = tmp;
	tmp = first_element->y;
	first_element->y = first_element->y1;
	first_element->y1 = tmp;
	(*first_element2)->next = first_element;
	(*first_element2) = (*first_element2)->next;
}

void				sort_help(int i, t_wals *rmp, t_wals *tmp, int *walls)
{
	while (rmp)
	{
		walls[i] = rmp->x;
		walls[i + 1] = rmp->y;
		walls[i + 2] = rmp->x1;
		walls[i + 3] = rmp->y1;
		i += 4;
		rmp = rmp->next;
	}
	--i;
	while (tmp)
	{
		tmp->y = walls[i];
		tmp->x = walls[i - 1];
		tmp->y1 = walls[i - 2];
		tmp->x1 = walls[i - 3];
		i -= 4;
		tmp = tmp->next;
	}
}

void				sort_walls_sec(t_wals *tmp)
{
	t_wals			*rmp;
	int				i;
	int				*walls;

	i = 0;
	rmp = tmp;
	while (rmp)
	{
		i++;
		rmp = rmp->next;
	}
	rmp = tmp;
	walls = (int *)malloc(sizeof(int) * (i * 4));
	i = 0;
	sort_help(i, rmp, tmp, walls);
	free(walls);
}

t_sector_list		*get_sector_id(t_doom *den, int id)
{
	t_sector_list	*tmp;
	int				i;

	tmp = den->sectors;
	i = -1;
	while (++i < id)
		tmp = tmp->next;
	return (tmp);
}
