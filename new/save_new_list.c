/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_new_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 15:43:27 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/04 15:43:28 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	ft_lstaddmap(t_wals **start, t_wals *new)
{
	t_wals	*tmp;

	if (*start == 0)
		*start = new;
	else
	{
		tmp = *start;
		while (tmp->next != 0)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	ft_lstnewmap(t_wals **new, int x, int y, t_doom *den)
{
	t_wals	*tmp;

	if (*new == NULL)
	{
		tmp = (t_wals *)malloc(sizeof(t_wals));
		tmp->x = (x - den->startx) / (START + den->skape);
		tmp->y = (y - den->starty) / (START + den->skape);
		tmp->next = NULL;
		*new = tmp;
	}
	else
	{
		tmp = *new;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (t_wals *)malloc(sizeof(t_wals));
		tmp->next->x = (x - den->startx) / (START + den->skape);
		tmp->next->y = (y - den->starty) / (START + den->skape);
		tmp->next->next = NULL;
	}
}

void	ft_save_parm(t_wals **tmp, int x, int y, t_doom *den)
{
	(*tmp)->x1 = (x - den->startx) / (START + den->skape);
	(*tmp)->y1 = (y - den->starty) / (START + den->skape);
	(*tmp)->tex = 0;
	(*tmp)->next_sector = NULL;
	(*tmp)->nextsector_wall = NULL;
	(*tmp)->blouk = den->blouk;
	(*tmp)->inside = den->incede;

	(*tmp)->glass = -1;
	(*tmp)->p = 0;
	
}

void	ft_lstnewmap_sec_help(t_doom *den)
{
	if (den->blouk == 1)
	{
		if (den->incede == 0)
		{
			add_new_sector(den);
			sort_sector(den);
		}
		else if (den->incede == 1)
		{
			sort_list_incide(den);
			sector_in_sector(den);
			check_data_sector(den);
			den->incede = 0;
		}
	}
}

void	ft_lstnewmap_sec(t_wals **new, int x, int y, t_doom *den)
{
	t_wals	*tmp;

	tmp = *new;
	if (tmp->next == NULL)
	{
		ft_save_parm(&tmp, x, y, den);
		tmp->sec = den->sec;
		*new = tmp;
	}
	else
	{
		tmp = *new;
		while (tmp->next)
			tmp = tmp->next;
		ft_save_parm(&tmp, x, y, den);
		tmp->sec = den->secbaktmp;
		ft_lstnewmap_sec_help(den);
	}
}
