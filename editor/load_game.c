/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkostrub <kkostrub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:48:11 by kkostrub          #+#    #+#             */
/*   Updated: 2019/03/05 17:19:56 by kkostrub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		load_map_level(t_doom *den)
{
	if (!read_check(den->fd, &den->iw.l, sizeof(t_level)))
		return (0);
	den->player.x = den->iw.l.start_x / MAP_SCALE;
	den->player.y = den->iw.l.start_y / MAP_SCALE;
	den->player.introt = den->iw.l.start_rot;
	den->finish = den->iw.l.win_sector;
	return (1);
}

int		load_map_music(t_doom *den)
{
	int					i;

	i = -1;
	while (++i < MUSIC_COUNT)
	{
		if (!read_check(den->fd, &(den->iw.sounds.music_pack_size[i]),
			sizeof(size_t)))
			return (0);
		den->iw.sounds.music_pack[i] =
					malloc(den->iw.sounds.music_pack_size[i]);
		if (!read_check(den->fd, den->iw.sounds.music_pack[i],
			den->iw.sounds.music_pack_size[i]))
			return (0);
	}
	return (1);
}

int		load_map_env(t_doom *den)
{
	int					i;

	i = -1;
	while (++i < ENV_COUNT)
	{
		den->iw.sounds.env[i] = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
		if (!read_check(den->fd, den->iw.sounds.env[i], sizeof(Mix_Chunk)))
			return (0);
		den->iw.sounds.env[i]->abuf = malloc(den->iw.sounds.env[i]->alen);
		if (!read_check(den->fd, den->iw.sounds.env[i]->abuf,
			den->iw.sounds.env[i]->alen))
			return (0);
	}
	return (1);
}

int		load4_game_1(t_doom *den, int i)
{
	den->iw.k.source_str = malloc(den->iw.k.source_size);
	if (!read_check(den->fd, den->iw.k.source_str, den->iw.k.source_size))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!read_check(den->fd, &den->iw.font_pack_size, sizeof(size_t)))
		return (0);
	den->iw.font_pack = malloc(den->iw.font_pack_size);
	if (!read_check(den->fd, den->iw.font_pack, den->iw.font_pack_size))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	return (1);
}

int		load4_game(t_doom *den)
{
	int					i;

	if (!load_map_textures(den))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_music(den))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!load_map_env(den))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);
	if (!read_check(den->fd, &den->iw.k.source_size, sizeof(size_t)))
		return (0);
	if (!load4_game_1(den, i))
		return (0);
	return (1);
}
