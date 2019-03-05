/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopukh <elopukh@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:46:05 by elopukh           #+#    #+#             */
/*   Updated: 2019/03/05 15:46:06 by elopukh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	write_textures2(t_doom *den, int zero)
{
	int i;

	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_weap[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_weap[i]->pixels,
		den->iw.t_weap[i]->pitch * den->iw.t_weap[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
}

void	write_textures1(t_doom *den, int zero)
{
	int i;

	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_pickup[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_pickup[i]->pixels,
		den->iw.t_pickup[i]->pitch * den->iw.t_pickup[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_enemies[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_enemies[i]->pixels,
		den->iw.t_enemies[i]->pitch * den->iw.t_enemies[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
	write_textures2(den, zero);
}

void	many_write(t_doom *den, int zero)
{
	write(den->fd, den->iw.hud.enot, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.enot->pixels,
	den->iw.hud.enot->pitch * den->iw.hud.enot->h);
	write(den->fd, &zero, sizeof(int));
	write(den->fd, den->iw.hud.miss, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.miss->pixels,
	den->iw.hud.miss->pitch * den->iw.hud.miss->h);
	write(den->fd, &zero, sizeof(int));
	write(den->fd, den->iw.hud.saved, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.saved->pixels,
	den->iw.hud.saved->pitch * den->iw.hud.saved->h);
	write(den->fd, &zero, sizeof(int));
	write(den->fd, den->iw.hud.dead, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.dead->pixels,
	den->iw.hud.dead->pitch * den->iw.hud.dead->h);
	write(den->fd, &zero, sizeof(int));
	write(den->fd, den->iw.hud.win, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.win->pixels,
	den->iw.hud.win->pitch * den->iw.hud.win->h);
	write(den->fd, &zero, sizeof(int));
}

void	save_map_textures(t_doom *den, int zero)
{
	int		i;

	write_textures(den, zero);
	write_textures1(den, zero);
	many_write(den, zero);
	i = -1;
	while (++i < 3)
	{
		write(den->fd, den->iw.bag.button[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.bag.button[i]->pixels,
		den->iw.bag.button[i]->pitch * den->iw.bag.button[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
	write(den->fd, den->iw.map.player, sizeof(t_packaging_texture));
	write(den->fd, den->iw.map.player->pixels,
	den->iw.map.player->pitch * den->iw.map.player->h);
	write(den->fd, &zero, sizeof(int));
	i = -1;
	while (++i < 6)
	{
		write(den->fd, den->iw.menu.icons[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.menu.icons[i]->pixels,
		den->iw.menu.icons[i]->pitch * den->iw.menu.icons[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
}

void	set_walls_sectors_ids(t_doom *den)
{
	t_sector_list	*sec;
	int				id;
	t_wals			*w;

	sec = den->sectors;
	id = 0;
	while (sec)
	{
		sec->id = id++;
		sec = sec->next;
	}
	w = den->tmp;
	id = 0;
	while (w)
	{
		w->id = id++;
		w = w->next;
	}
}
