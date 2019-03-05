/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopukh <elopukh@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:44:11 by elopukh           #+#    #+#             */
/*   Updated: 2019/03/05 15:44:12 by elopukh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	save_map_wall_animations(t_doom *den)
{
	t_wall_animation	*tmp;
	int					count;

	count = 0;
	tmp = den->iw.wall_animations;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	write(den->fd, &count, sizeof(int));
	if (den->iw.wall_animations)
		save_reverse_map_wall_animations(den, den->iw.wall_animations);
}

void	set_checkpoints_sprite_numb(t_doom *den, t_keyb_inp *tmp)
{
	t_sprite	*spr;
	int			numb;

	numb = 0;
	spr = den->sprite;
	while (spr)
	{
		if (spr == tmp->sprite)
		{
			tmp->sprite_numb = numb;
			return ;
		}
		spr = spr->next;
		numb++;
	}
}

void	save_reverse_map_checkpoints(t_doom *den, t_keyb_inp *tmp)
{
	if (tmp->next != 0)
		save_reverse_map_checkpoints(den, tmp->next);
	set_checkpoints_sprite_numb(den, tmp);
	write(den->fd, tmp, sizeof(t_keyb_inp));
}

void	save_map_checkpoints(t_doom *den)
{
	t_keyb_inp	*tmp;
	int			count;

	count = 0;
	tmp = den->iw.checkpoints;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	write(den->fd, &count, sizeof(int));
	if (den->iw.checkpoints)
		save_reverse_map_checkpoints(den, den->iw.checkpoints);
}

void	write_textures(t_doom *den, int zero)
{
	int i;

	i = -1;
	while (++i < TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t[i]->pixels,
		den->iw.t[i]->pitch * den->iw.t[i]->h);
	}
	write(den->fd, (void *)den->iw.tsz,
	sizeof(float) * TEXTURES_COUNT);
	write(den->fd, &zero, sizeof(int));
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_decor[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_decor[i]->pixels,
		den->iw.t_decor[i]->pitch * den->iw.t_decor[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
}
