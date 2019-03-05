/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map_sec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkostrub <kkostrub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:47:57 by kkostrub          #+#    #+#             */
/*   Updated: 2019/03/05 17:33:54 by kkostrub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		load_map_checkpoints_1(t_doom *den, t_keyb_inp *tmp)
{
	t_sprite			*spr;
	int					j;

	if (!read_check(den->fd, tmp, sizeof(t_keyb_inp)))
	{
		free(tmp);
		return (0);
	}
	j = -1;
	spr = den->sprite;
	while (spr && ++j < tmp->sprite_numb)
		spr = spr->next;
	if (!spr)
	{
		free(tmp);
		return (0);
	}
	tmp->sprite = spr;
	return (1);
}

int		load_map_checkpoints(t_doom *den)
{
	int					i;
	int					count;
	t_keyb_inp			*tmp;

	den->iw.checkpoints = 0;
	if (!read_check(den->fd, &count, sizeof(int)))
		return (0);
	i = -1;
	while (++i < count)
	{
		tmp = (t_keyb_inp *)malloc(sizeof(t_keyb_inp));
		if (!load_map_checkpoints_1(den, tmp))
			return (0);
		tmp->next = den->iw.checkpoints;
		den->iw.checkpoints = tmp;
	}
	return (1);
}

int		load_map_sec_1(t_doom *den, t_sector_list *tmp)
{
	if (!read_check(den->fd, tmp, sizeof(t_sector_list)))
	{
		free(tmp);
		return (0);
	}
	if (tmp->fr.n != 0)
	{
		tmp->fr.n = (t_vector *)malloc(sizeof(t_vector));
		if (!read_check(den->fd, tmp->fr.n, sizeof(t_vector)))
		{
			free(tmp);
			return (0);
		}
	}
	if (tmp->cl.n != 0)
	{
		tmp->cl.n = (t_vector *)malloc(sizeof(t_vector));
		if (!read_check(den->fd, tmp->cl.n, sizeof(t_vector)))
		{
			free(tmp);
			return (0);
		}
	}
	return (1);
}

int		load_map_sec_2(t_doom *den, t_sector_list *tmp)
{
	int					j;
	t_wals				*rmp;
	t_picture			*pic;

	rmp = den->tmp;
	j = -1;
	while (rmp && ++j < tmp->light_wall_numb)
		rmp = rmp->next;
	if (!rmp)
	{
		free(tmp);
		return (0);
	}
	pic = rmp->p;
	j = -1;
	while (pic && ++j < tmp->light_numb)
		pic = pic->next;
	if (!pic)
	{
		free(tmp);
		return (0);
	}
	tmp->light = pic;
	return (1);
}

int		load_map_sec(t_doom *den)
{
	t_sector_list		*tmp;
	int					i;

	den->sectors = 0;
	if (!read_check(den->fd, &den->sec, sizeof(int)))
		return (0);
	i = -1;
	while (++i < den->sec)
	{
		tmp = (t_sector_list *)malloc(sizeof(t_sector_list));
		if (!load_map_sec_1(den, tmp))
			return (0);
		if (tmp->light != 0)
			if (!load_map_sec_2(den, tmp))
				return (0);
		tmp->next = den->sectors;
		den->sectors = tmp;
	}
	den->secbak = den->sec;
	den->secbaktmp = den->sec;
	return (1);
}
