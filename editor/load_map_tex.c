/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map_tex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkostrub <kkostrub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:48:08 by kkostrub          #+#    #+#             */
/*   Updated: 2019/03/05 17:03:23 by kkostrub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		read_check(int fd, void *dst, size_t max_inp)
{
	size_t				i;

	if ((i = read(fd, dst, max_inp)) != max_inp)
		return (0);
	return (1);
}

int		load_map_texture(int fd, t_packaging_texture **t, int count)
{
	int					i;

	i = -1;
	while (++i < count)
	{
		t[i] = (t_packaging_texture *)malloc(sizeof(t_packaging_texture));
		if (!read_check(fd, t[i], sizeof(t_packaging_texture)))
			return (0);
		t[i]->pixels = malloc(t[i]->pitch * t[i]->h);
		if (!read_check(fd, t[i]->pixels, t[i]->pitch * t[i]->h))
			return (0);
	}
	return (1);
}

int		load_map_textures2(t_doom *den, int i)
{
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, &den->iw.hud.dead, 1))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, &den->iw.hud.win, 1))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, den->iw.bag.button, 3))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, &den->iw.map.player, 1))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, den->iw.menu.icons, 6))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	return (1);
}

int		load_map_textures1(t_doom *den, int i)
{
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_enemies,
		ENEMIES_TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_weap,
		WEAPONS_TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, &den->iw.hud.enot, 1))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, &den->iw.hud.miss, 1))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, &den->iw.hud.saved, 1))
		return (0);
	if (!load_map_textures2(den, i))
		return (0);
	return (1);
}

int		load_map_textures(t_doom *den)
{
	int					i;

	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t,
		TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, (void *)den->iw.tsz,
		sizeof(float) * TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_decor,
		DECOR_TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_pickup,
		PICK_UP_TEXTURES_COUNT))
		return (0);
	if (!load_map_textures1(den, i))
		return (0);
	return (1);
}
