/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkostrub <kkostrub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:48:03 by kkostrub          #+#    #+#             */
/*   Updated: 2019/03/05 17:34:01 by kkostrub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		load_map_sector_animations_1(t_doom *den,
			t_sector_animation *tmp, t_picture *pic)
{
	t_wals				*w;
	int					j;

	j = -1;
	w = den->tmp;
	while (w && ++j < tmp->trig_wall_numb)
		w = w->next;
	if (!w)
	{
		free(tmp);
		return (0);
	}
	pic = w->p;
	j = -1;
	while (pic && ++j < tmp->trig_pic_numb)
		pic = pic->next;
	if (!pic)
	{
		free(tmp);
		return (0);
	}
	tmp->trigger = pic;
	tmp->prev_clock = clock();
	tmp->next = den->iw.sector_animations;
	return (1);
}

int		load_map_sector_animations(t_doom *den)
{
	int					count_anim;
	int					i;
	t_sector_animation	*tmp;
	t_picture			*pic;

	pic = 0;
	den->iw.sector_animations = 0;
	if (!read_check(den->fd, &count_anim, sizeof(int)))
		return (0);
	i = -1;
	while (++i < count_anim)
	{
		tmp = (t_sector_animation *)malloc(sizeof(t_sector_animation));
		if (!read_check(den->fd, tmp, sizeof(t_sector_animation)))
		{
			free(tmp);
			return (0);
		}
		if (!load_map_sector_animations_1(den, tmp, pic))
			return (0);
		den->iw.sector_animations = tmp;
	}
	return (1);
}

int		load_map_wall_animations_1(t_doom *den, t_wall_animation *tmp)
{
	t_wals				*w;
	t_picture			*pic;
	int					j;

	j = -1;
	w = den->tmp;
	while (w && ++j < tmp->trig_wall_numb)
		w = w->next;
	if (!w)
	{
		free(tmp);
		return (0);
	}
	pic = w->p;
	j = -1;
	while (pic && ++j < tmp->trig_pic_numb)
		pic = pic->next;
	if (!pic)
	{
		free(tmp);
		return (0);
	}
	tmp->trigger = pic;
	tmp->prev_clock = 0;
	return (1);
}

int		load_map_wall_animations(t_doom *den)
{
	int					count_anim;
	int					i;
	t_wall_animation	*tmp;

	den->iw.wall_animations = 0;
	if (!read_check(den->fd, &count_anim, sizeof(int)))
		return (0);
	i = -1;
	while (++i < count_anim)
	{
		tmp = (t_wall_animation *)malloc(sizeof(t_wall_animation));
		if (!read_check(den->fd, tmp, sizeof(t_wall_animation)))
		{
			free(tmp);
			return (0);
		}
		if (!load_map_wall_animations_1(den, tmp))
			return (0);
		tmp->next = den->iw.wall_animations;
		den->iw.wall_animations = tmp;
	}
	return (1);
}
