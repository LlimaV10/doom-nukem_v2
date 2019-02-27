/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 12:11:58 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/20 12:11:59 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	make_portal_sector2(t_doom *den, t_wals *first_element,
										t_wals *first_element_new)
{
	t_sector_list	*lmp;
	t_sector_list	*sector_in = 0;
	t_wals			*next_sector_start;

	add_new_sector(den);
	lmp = den->sectors;
	den->i = -1;
	while (lmp->next)
	{
		if (++den->i == den->find_tmp->sec)
			sector_in = lmp;
		lmp = lmp->next;
	}
	next_sector_start = first_element;
	change_way2(den, first_element->next, &first_element);
	first_element->next = 0;
	while (next_sector_start->next)
	{
		first_element_new->next_sector = lmp;
		next_sector_start->next->next_sector = sector_in;
		first_element_new = first_element_new->next;
		next_sector_start = next_sector_start->next;
	}
	next_sector_start->blouk = 1;
}

void	portal_sector_new(t_doom *den, t_wals *rmp, t_wals *lmp)
{
	while (lmp && lmp->inside == den->find_tmp->inside)
	{
		rmp->next = (t_wals *)malloc(sizeof(t_wals));
		rmp->next->x = lmp->x;
		rmp->next->y = lmp->y;
		rmp->next->x1 = lmp->x1;
		rmp->next->y1 = lmp->y1;
		rmp->next->sec = den->sec;
		rmp->next->p = 0;
		rmp->next->glass = -1;
		rmp->next->blouk = 0;
		rmp->next->inside = 0;
		rmp->next->tex = 1;
		rmp->next->nextsector_wall = lmp;
		lmp->nextsector_wall = rmp->next;
		rmp->next->next = NULL;
		den->walls += 1;
		lmp = lmp->next;
		rmp = rmp->next;
	}
}

void	make_portal_sector(t_doom *den, t_col *vec)
{
	t_wals	*rmp;
	t_wals	*lmp = 0;
	t_wals	*first_element;
	t_wals	*first_element_new;

	rmp = den->tmp;
	while (rmp->next)
	{
		if ((rmp->sec != den->find_tmp->sec && rmp->next->sec ==
		den->find_tmp->sec && rmp->next->inside == den->find_tmp->inside) ||
		(rmp->sec == den->find_tmp->sec && rmp->inside != den->find_tmp->inside
		&& rmp->next->inside == den->find_tmp->inside))
			lmp = rmp->next;
		rmp = rmp->next;
	}
	first_element = rmp;
	first_element_new = lmp;
	portal_sector_new(den, rmp, lmp);
	den->sec += 1;
	den->secbak += 1;
	make_portal_sector2(den, first_element, first_element_new);
	retry_write(den, vec);
}

int		check_make_portal(t_doom *den)
{
	if (((den->find_tmp_one->x1 == den->find_tmp_two->x) &&
		(den->find_tmp_one->x == den->find_tmp_two->x1) &&
		(den->find_tmp_one->y == den->find_tmp_two->y1) &&
		(den->find_tmp_one->y1 == den->find_tmp_two->y)) ||
		((den->find_tmp_one->x1 == den->find_tmp_two->x1) &&
		(den->find_tmp_one->x == den->find_tmp_two->x) &&
		(den->find_tmp_one->y == den->find_tmp_two->y) &&
		(den->find_tmp_one->y1 == den->find_tmp_two->y1)))
		return (1);
	return (0);
}

void	portal_sector(t_doom *den)
{
	t_wals	*rmp;

	if (check_make_portal(den) == 1)
	{
		rmp = den->tmp;
		while (rmp)
		{
			if (rmp == den->find_tmp_one)
			{
				rmp->next_sector = get_sector_id(den, den->find_tmp_two->sec);
				rmp->nextsector_wall = den->find_tmp_two;
			}
			if (rmp == den->find_tmp_two)
			{
				rmp->next_sector = get_sector_id(den, den->find_tmp_one->sec);
				rmp->nextsector_wall = den->find_tmp_one;
				den->color = 0xFF0000;
				ft_line(den, &den->rec);
				break ;
			}
			rmp = rmp->next;
		}
		den->color = 0xFFFFFF;
		SDL_UpdateWindowSurface(den->window);
	}
}
