/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopukh <elopukh@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:35:34 by elopukh           #+#    #+#             */
/*   Updated: 2019/03/05 15:35:35 by elopukh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		func1(t_sector_list *sec, t_picture *pic, int num_wall, int num_pic)
{
	if (pic == sec->light)
	{
		sec->light_wall_numb = num_wall;
		sec->light_numb = num_pic;
		return (1);
	}
	return (0);
}

void	set_sector_wall_light_numbs(t_doom *den, t_sector_list *sec)
{
	t_wals		*tmp;
	t_picture	*pic;
	int			num_wall;
	int			num_pic;

	if (sec->light == 0)
		return ;
	tmp = den->tmp;
	num_wall = 0;
	while (tmp)
	{
		pic = tmp->p;
		num_pic = 0;
		while (pic)
		{
			if (func1(sec, pic, num_wall, num_pic) == 1)
				return ;
			pic = pic->next;
			num_pic++;
		}
		tmp = tmp->next;
		num_wall++;
	}
}

void	save_reverse_map_sec(t_doom *den, t_sector_list *tmp)
{
	if (tmp->next != 0)
		save_reverse_map_sec(den, tmp->next);
	set_sector_wall_light_numbs(den, tmp);
	write(den->fd, tmp, sizeof(t_sector_list));
	if (tmp->fr.n != 0)
		write(den->fd, tmp->fr.n, sizeof(t_vector));
	if (tmp->cl.n != 0)
		write(den->fd, tmp->cl.n, sizeof(t_vector));
}

void	save_map_sec(t_doom *den)
{
	int		i;

	i = den->sec;
	write(den->fd, &i, sizeof(int));
	if (den->sectors)
		save_reverse_map_sec(den, den->sectors);
}

int		count_pictures(t_picture *pic)
{
	int		i;

	i = 0;
	while (pic)
	{
		i++;
		pic = pic->next;
	}
	return (i);
}
