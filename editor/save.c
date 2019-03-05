/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopukh <elopukh@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:21:11 by elopukh           #+#    #+#             */
/*   Updated: 2019/03/05 15:21:13 by elopukh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _CRT_SECURE_NO_WARNINGS
#include "doom_nuken.h"

void	set_nextsectors(t_doom *den)
{
	t_wals	*w;

	w = den->tmp;
	while (w)
	{
		if (w->next_sector != 0)
		{
			w->next_sector_numb = w->next_sector->id;
			w->nextsector_wall_numb = w->nextsector_wall->id;
		}
		else
		{
			w->next_sector_numb = -1;
			w->nextsector_wall_numb = -1;
		}
		w = w->next;
	}
}

void	save_map_music(t_doom *den)
{
	int		i;

	i = -1;
	while (++i < MUSIC_COUNT)
	{
		write(den->fd, &(den->iw.sounds.music_pack_size[i]), sizeof(size_t));
		write(den->fd, den->iw.sounds.music_pack[i],
			den->iw.sounds.music_pack_size[i]);
	}
}

void	save_map_env(t_doom *den)
{
	int		i;

	i = -1;
	while (++i < ENV_COUNT)
	{
		write(den->fd, den->iw.sounds.env[i], sizeof(Mix_Chunk));
		write(den->fd, den->iw.sounds.env[i]->abuf,
			den->iw.sounds.env[i]->alen);
	}
}

void	save_map_file1(t_doom *den, int zero)
{
	write(den->fd, "This is map created for doom-nukem "
		"by dbolilyi & ddehtyar & kkostrub & elopukh", 78);
	set_walls_sectors_ids(den);
	set_nextsectors(den);
	save_map_walls_pictures(den);
	write(den->fd, &zero, sizeof(int));
	save_map_sec(den);
	write(den->fd, &zero, sizeof(int));
	save_map_sprites(den);
	write(den->fd, &zero, sizeof(int));
	save_map_sector_animations(den);
	write(den->fd, &zero, sizeof(int));
	save_map_wall_animations(den);
	write(den->fd, &zero, sizeof(int));
	save_map_checkpoints(den);
	write(den->fd, &zero, sizeof(int));
	den->iw.l.start_x = den->player.x * MAP_SCALE;
	den->iw.l.start_y = den->player.y * MAP_SCALE;
	den->iw.l.start_rot = den->player.introt;
	den->iw.l.win_sector = den->finish;
	write(den->fd, &den->iw.l, sizeof(t_level));
	write(den->fd, &zero, sizeof(int));
	save_map_textures(den, zero);
}

void	save_map_file(t_doom *den)
{
	int		zero;

	den->fd = open(den->fname, WRITE_MAP);
	if (den->fd < 0)
		return ;
	zero = 0;
	save_map_file1(den, zero);
	write(den->fd, &zero, sizeof(int));
	save_map_music(den);
	write(den->fd, &zero, sizeof(int));
	save_map_env(den);
	write(den->fd, &zero, sizeof(int));
	write(den->fd, &den->iw.k.source_size, sizeof(size_t));
	write(den->fd, den->iw.k.source_str, den->iw.k.source_size);
	write(den->fd, &zero, sizeof(int));
	write(den->fd, &den->iw.font_pack_size, sizeof(size_t));
	write(den->fd, den->iw.font_pack, den->iw.font_pack_size);
	write(den->fd, &zero, sizeof(int));
	close(den->fd);
}
