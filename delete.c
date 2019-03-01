/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/06 12:16:30 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/06 12:16:31 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		go_delete_help(int sec, t_wals *rmp, t_doom *den, int k)
{
	if (rmp->next && rmp->next->sec != sec)
	{
		if (k == 1)
			den->tmp->next = rmp->next;
		if (k == 0)
			den->tmp = rmp->next;
		return (den->i);
	}
	if (k == 0 && rmp->next == NULL)
	{
		den->tmp = NULL;
		return (den->i);
	}
	if (rmp->next == NULL)
	{
		den->tmp->next = NULL;
		return (den->i);
	}
	return (0);
}

int		go_delete(int sec, t_wals *rmp, t_doom *den, int k)
{
	t_wals	*cmp;
	t_wals	*nmp;
	int		m;

	den->i = 0;
	while (rmp)
	{
		if (rmp->next_sector != 0)
		{
			delete_pointer(den, rmp);
			nmp = rmp->nextsector_wall;
			nmp->next_sector = 0;
			nmp->nextsector_wall = 0;
			rmp->next_sector = 0;
			rmp->nextsector_wall = 0;
		}
		cmp = rmp;
		den->i += 1;
		if ((m = go_delete_help(sec, rmp, den, k)) != 0)
		{
			free(cmp);
			return (den->i);
		}
		rmp = rmp->next;
		free(cmp);
	}
	return (0);
}

void	delete_sector_one(t_doom *den, int sec)
{
	t_wals	*first;

	den->i = go_delete(sec, den->tmp, den, 0);
	first = den->tmp;
	while (den->tmp)
	{
		den->tmp->sec -= 1;
		den->tmp = den->tmp->next;
	}
	den->tmp = first;
}

void	delete_sector_two(t_doom *den, int sec)
{
	den->i = go_delete(sec, den->tmp->next, den, 1);
	while (den->tmp->next)
	{
		den->tmp->next->sec -= 1;
		den->tmp = den->tmp->next;
	}
}

void	delete_sector(t_doom *den)
{
	t_wals	*first;
	int		sec;

	den->i = 0;
	sec = den->find_tmp->sec;
	first = den->tmp;
	while (den->tmp)
	{
		if (den->tmp->sec == sec)
		{
			delete_sector_one(den, sec);
			break ;
		}
		else if (den->tmp->next && den->tmp->next->sec == sec)
		{
			delete_sector_two(den, sec);
			den->tmp = first;
			break ;
		}
		den->tmp = den->tmp->next;
	}
	den->walls = den->walls - den->i;
	den->sec -= 1;
	den->secbak -= 1;
	den->incede = 0;
}
