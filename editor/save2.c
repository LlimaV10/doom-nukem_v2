/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopukh <elopukh@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:39:33 by elopukh           #+#    #+#             */
/*   Updated: 2019/03/05 15:39:34 by elopukh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define SUM WINDOW_W * 2 + 2
#include "doom_nuken.h"

void	save_reverse_map_pictures(t_doom *den, t_picture *pic)
{
	if (pic->next != 0)
		save_reverse_map_pictures(den, pic->next);
	write(den->fd, pic, sizeof(t_picture));
}

void	save_reverse_map_walls_pictures(t_doom *den, t_wals *tmp)
{
	int		i;

	if (tmp->next != 0)
		save_reverse_map_walls_pictures(den, tmp->next);
	write(den->fd, tmp, sizeof(t_wals));
	i = count_pictures(tmp->p);
	write(den->fd, &i, sizeof(int));
	if (tmp->p)
		save_reverse_map_pictures(den, tmp->p);
}

void	save_map_walls_pictures(t_doom *den)
{
	int		i;

	i = den->walls + 1;
	write(den->fd, &i, sizeof(int));
	if (den->tmp)
		save_reverse_map_walls_pictures(den, den->tmp);
}

void	save_reverse_map_sprites(t_doom *den, t_sprite *spr)
{
	if (spr->next != 0)
		save_reverse_map_sprites(den, spr->next);
	write(den->fd, spr, sizeof(t_sprite) - sizeof(int) * (SUM));
}

void	save_map_sprites(t_doom *den)
{
	write(den->fd, &den->sprites, sizeof(int));
	if (den->sprite)
		save_reverse_map_sprites(den, den->sprite);
}
