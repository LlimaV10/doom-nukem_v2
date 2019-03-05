/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopukh <elopukh@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:42:18 by elopukh           #+#    #+#             */
/*   Updated: 2019/03/05 15:42:18 by elopukh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	set_sector_anim_wall_trig_numbs(t_doom *den, t_sector_animation *sec)
{
	t_wals		*tmp;
	t_picture	*pic;
	int			num_wall;
	int			num_pic;

	tmp = den->tmp;
	num_wall = 0;
	while (tmp)
	{
		pic = tmp->p;
		num_pic = 0;
		while (pic)
		{
			if (pic == sec->trigger)
			{
				sec->trig_wall_numb = num_wall;
				sec->trig_pic_numb = num_pic;
				return ;
			}
			pic = pic->next;
			num_pic++;
		}
		tmp = tmp->next;
		num_wall++;
	}
}

void	save_reverse_map_sector_animations(t_doom *den, t_sector_animation *tmp)
{
	if (tmp->next != 0)
		save_reverse_map_sector_animations(den, tmp->next);
	set_sector_anim_wall_trig_numbs(den, tmp);
	write(den->fd, tmp, sizeof(t_sector_animation));
}

void	save_map_sector_animations(t_doom *den)
{
	t_sector_animation	*tmp;
	int					count;

	count = 0;
	tmp = den->iw.sector_animations;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	write(den->fd, &count, sizeof(int));
	if (den->iw.sector_animations)
		save_reverse_map_sector_animations(den, den->iw.sector_animations);
}

void	set_wall_anim_wall_trig_numbs(t_doom *den, t_wall_animation *sec)
{
	t_wals		*tmp;
	t_picture	*pic;
	int			num_wall;
	int			num_pic;

	tmp = den->tmp;
	num_wall = 0;
	while (tmp)
	{
		pic = tmp->p;
		num_pic = 0;
		while (pic)
		{
			if (pic == sec->trigger)
			{
				sec->trig_wall_numb = num_wall;
				sec->trig_pic_numb = num_pic;
				return ;
			}
			pic = pic->next;
			num_pic++;
		}
		tmp = tmp->next;
		num_wall++;
	}
}

void	save_reverse_map_wall_animations(t_doom *den, t_wall_animation *tmp)
{
	if (tmp->next != 0)
		save_reverse_map_wall_animations(den, tmp->next);
	set_wall_anim_wall_trig_numbs(den, tmp);
	write(den->fd, tmp, sizeof(t_wall_animation));
}
