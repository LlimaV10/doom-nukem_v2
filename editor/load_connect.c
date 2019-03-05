/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkostrub <kkostrub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:11:12 by kkostrub          #+#    #+#             */
/*   Updated: 2019/03/05 17:11:21 by kkostrub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		connect_sector(t_doom *den, t_wals *wall)
{
	t_sector_list		*tmp;

	tmp = den->sectors;
	while (tmp)
	{
		if (tmp->id == wall->next_sector_numb)
		{
			wall->next_sector = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int		connect_wall(t_doom *den, t_wals *wall)
{
	t_wals				*tmp;

	tmp = den->tmp;
	while (tmp)
	{
		if (tmp->id == wall->nextsector_wall_numb)
		{
			wall->nextsector_wall = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int		connect_map_sectors(t_doom *den)
{
	t_wals				*tmp;

	tmp = den->tmp;
	while (tmp)
	{
		if (tmp->next_sector_numb != -1)
		{
			if (!connect_sector(den, tmp) ||
				!connect_wall(den, tmp))
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}
