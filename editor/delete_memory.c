/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 18:51:14 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/19 18:51:15 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	delete_pointer(t_doom *den, void *ptr)
{
	t_wals	*rmp;

	rmp = den->tmp;
	while (rmp->next)
	{
		if (rmp->next_sector == ptr)
			rmp->next_sector = NULL;
		if (rmp->nextsector_wall == ptr)
			rmp->nextsector_wall = NULL;
		rmp = rmp->next;
	}
}

void	delete_sector_memory_one(t_doom *den, t_sector_list *first)
{
	t_sector_list	*sect;

	if (first->next != NULL)
	{
		sect = first;
		delete_pointer(den, sect);
		den->sectors = den->sectors->next;
		free(sect);
		return ;
	}
	else if (first->next == NULL)
	{
		free(first);
		den->sectors = NULL;
	}
}

void	delete_sector_memory_two(t_doom *den, t_sector_list *first)
{
	t_sector_list	*sect;

	if (first->next->next != NULL)
	{
		sect = first->next;
		delete_pointer(den, sect);
		first->next = first->next->next;
		free(sect);
		return ;
	}
	else if (first->next->next == NULL)
	{
		sect = first->next;
		free(sect);
		first->next = NULL;
		return ;
	}
}

void	delete_sector_memory(t_doom *den)
{
	t_sector_list	*first;
	int				i;

	i = -1;
	first = den->sectors;
	while (first)
	{
		if (++i == 0 && den->find_tmp->sec == i)
		{
			delete_sector_memory_one(den, first);
			return ;
		}
		else if (first->next && i + 1 == den->find_tmp->sec)
		{
			delete_sector_memory_two(den, first);
			return ;
		}
		first = first->next;
	}
}

void	delete_wall(t_doom *den, t_col *vec)
{
	if (den->change == 4)
	{
		delete_sprite(den, den->find_sprite);
		den->change = 0;
	}
	else if (den->change == 3)
	{
		check_finish_sprite(den);
		delete_sector_memory(den);
		delete_sector(den);       // удаляю сектор
		den->change = 0;
	}
	else
	{
		den->button = 0;
		write_list(den, den->tmp); // удаляю стену
		clean_vec(vec);
	}
	clean_find_vec(den);
	clear_texture(den, vec);
	map_network(den);
	info_display(den);
}
