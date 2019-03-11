/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_list2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:01:56 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/11 13:02:08 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	check_deleting_wall_animations2(t_wall_animation *wa,
	int nw, int *f)
{
	t_wall_animation	*tmp;
	int					i;

	i = -1;
	while (++i < wa->next->count_walls)
	{
		if (wa->next->walls[i] == nw)
		{
			tmp = wa->next;
			wa->next = wa->next->next;
			free(tmp);
			*f = 0;
			break ;
		}
		else if (wa->next->walls[i] > nw)
			wa->next->walls[i] -= 1;
	}
}

void	check_deleting_wall_animations(t_doom *den, int nw)
{
	t_wall_animation	wa_start;
	t_wall_animation	*wa;
	int					f;

	wa_start.next = den->iw.wall_animations;
	wa = &wa_start;
	while (wa->next)
	{
		f = 1;
		check_deleting_wall_animations2(wa, nw, &f);
		if (f)
			wa = wa->next;
	}
	den->iw.wall_animations = wa_start.next;
}
