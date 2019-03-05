/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sector_ways2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:12:14 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 17:12:15 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void			free_way(t_sector_ways **way)
{
	t_sector_way	*tmp;
	t_sector_way	*tmp2;

	if (*way == 0)
		return ;
	tmp = (*way)->way_start;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	free(*way);
	*way = 0;
}

int				sector_in_way(t_sdl *iw, t_sector_ways *way, int sector)
{
	t_sector_way	*tmp;

	if (way == 0)
		return (0);
	tmp = way->way_start;
	while (tmp)
	{
		if (iw->walls[tmp->portal].nextsector == sector)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
