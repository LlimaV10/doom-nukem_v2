/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:57:25 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/11 12:57:40 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	check_deleting_sector_animations3(t_wall_animation *wa, int nw,
	int *f, int cw)
{
	t_wall_animation	*tmp;
	int					i;

	i = -1;
	while (++i < wa->next->count_walls)
	{
		if (wa->next->walls[i] >= nw && wa->next->walls[i] < nw + cw)
		{
			tmp = wa->next;
			wa->next = wa->next->next;
			free(tmp);
			*f = 0;
			break ;
		}
		else if (wa->next->walls[i] >= nw + cw)
			wa->next->walls[i] -= cw;
	}
}

void	check_deleting_sector_animations2(t_doom *den, int cw, int nw)
{
	t_wall_animation	wa_start;
	t_wall_animation	*wa;
	int					f;

	wa_start.next = den->iw.wall_animations;
	wa = &wa_start;
	while (wa->next)
	{
		f = 1;
		check_deleting_sector_animations3(wa, nw, &f, cw);
		if (f)
			wa = wa->next;
	}
	den->iw.wall_animations = wa_start.next;
}

void	check_deleting_sector_animations(t_doom *den, int sec, int cw, int nw)
{
	t_sector_animation	sa_start;
	t_sector_animation	*sa;
	t_sector_animation	*tmp;

	sa_start.next = den->iw.sector_animations;
	sa = &sa_start;
	while (sa->next)
		if (sa->next->sector == sec)
		{
			tmp = sa->next;
			sa->next = sa->next->next;
			free(tmp);
		}
		else
		{
			if (sa->next->sector > sec)
				sa->next->sector -= 1;
			sa = sa->next;
		}
	den->iw.sector_animations = sa_start.next;
	check_deleting_sector_animations2(den, cw, nw);
}

void	delete_sector2(t_doom *den, int sec, int nw)
{
	check_deleting_sector_animations(den, sec, den->i, nw);
	den->walls = den->walls - den->i;
	den->sec -= 1;
	den->secbak -= 1;
	den->incede = 0;
}
