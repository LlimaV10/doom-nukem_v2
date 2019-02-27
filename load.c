/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 17:16:02 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/22 17:16:03 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		read_check(int fd, void *dst, size_t max_inp)
{
	size_t	i;
	if ((i = read(fd, dst, max_inp)) != max_inp)
		return (0);
	return (1);
}

int		load_map_sprites(t_doom *den)
{
	t_sprite	*tmp;
	int			i;

	den->sprite = 0;
	if (!read_check(den->fd, &den->sprites, sizeof(int)))
		return (0);
	i = -1;
	while (++i < den->sprites)
	{
		tmp = (t_sprite *)malloc(sizeof(t_sprite));
		if (!read_check(den->fd, tmp, sizeof(t_sprite) - sizeof(int) * (WINDOW_W * 2 + 2)))
		{
			free(tmp);
			return (0);
		}
		tmp->fall_time = 1;
		tmp->e.previous_picture_change = 1;
		tmp->e.prev_update_time = 1;
		tmp->next = den->sprite;
		den->sprite = tmp;
	}
	return (1);
}

int		load_map_sec(t_doom *den)
{
	t_sector_list	*tmp;
	t_wals  		*rmp;
	t_picture		*pic;
	int				i;
	int				j;
	int				k;

	den->sectors = 0;
	if (!read_check(den->fd, &den->sec, sizeof(int)))
		return (0);
	i = -1;
	while (++i < den->sec)
	{
		tmp = (t_sector_list *)malloc(sizeof(t_sector_list));
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
		if (tmp->light != 0)
		{
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
		}
		tmp->next = den->sectors;
		den->sectors = tmp;
	}
	//den->sec += 1;
	den->secbak = den->sec;
	den->secbaktmp = den->sec;
	return (1);
}


int		load_map_walls_pictures(t_doom *den)
{
	t_wals		*tmp;
	t_picture   *pic;
	int			j;
	int			i;
	int			count_pic;

	den->tmp = 0;
	if (!read_check(den->fd, &den->walls, sizeof(int)))
		return (0);
	i = -1;
	while (++i < den->walls)
	{
		tmp = (t_wals *)malloc(sizeof(t_wals));
		if (!read_check(den->fd, tmp, sizeof(t_wals)))
		{
			free(tmp);
			return (0);
		}
		tmp->p = 0;
		if (!read_check(den->fd, &count_pic, sizeof(int)))
			return (0);
		j = -1;
		while (++j < count_pic)
		{
			pic = (t_picture *)malloc(sizeof(t_picture));
			if (!read_check(den->fd, pic, sizeof(t_picture)))
			{
				free(pic);
				return (0);
			}
			pic->next = tmp->p;
			tmp->p = pic;
		}
		tmp->next = den->tmp;
		den->tmp = tmp;
	}
	den->walls -= 1;
	return (1);
}

int		connect_sector(t_doom *den, t_wals *wall)
{
	t_sector_list	*tmp;

	tmp = den->sectors;
	while (tmp)
	{
		if (tmp->id == wall->next_sector_numb)
		{
			wall->next_sector = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int		connect_wall(t_doom *den, t_wals *wall)
{
	t_wals	*tmp;

	tmp = den->tmp;
	while (tmp)
	{
		if (tmp->id == wall->nextsector_wall_numb)
		{
			wall->nextsector_wall = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int		connect_map_sectors(t_doom *den)
{
	t_wals	*tmp;

	tmp = den->tmp;
	while (tmp)
	{
		if (tmp->next_sector_numb != -1)
		{
			if (!connect_sector(den, tmp) ||
				!connect_wall(den, tmp))
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int		load_map_sector_animations(t_doom *den)
{
	int		count_anim;
	int		i;
	t_sector_animation	*tmp;
	t_wals		*w;
	t_picture	*pic;
	int		j;

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
		den->iw.sector_animations = tmp;
	}
	return (1);
}

int		load_map_wall_animations(t_doom *den)
{
	int		count_anim;
	int		i;
	t_wall_animation	*tmp;
	t_wals		*w;
	t_picture	*pic;
	int		j;

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
		tmp->next = den->iw.wall_animations;
		den->iw.wall_animations = tmp;
	}
	return (1);
}

int		load_map_checkpoints(t_doom *den)
{
	int		i;
	int		count;
	t_keyb_inp	*tmp;
	t_sprite	*spr;
	int		j;

	den->iw.checkpoints = 0;
	if (!read_check(den->fd, &count, sizeof(int)))
		return (0);
	i = -1;
	while (++i < count)
	{
		tmp = (t_keyb_inp *)malloc(sizeof(t_keyb_inp));
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
		tmp->next = den->iw.checkpoints;
		den->iw.checkpoints = tmp;
	}
	return (1);
}

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

int	load_map(t_doom *den)
{
	int		zero;
	char	author[79];

	/*char	a[200];
	read_check(den->fd, a, 200);*/
	zero = 0;
	author[78] = '\0';
	if (!read_check(den->fd, author, 78))
		return (0);
	if (ft_strcmp(author, "This is map created for doom-nukem by dbolilyi & ddehtyar & kkostrub & elopukh") != 0)
		return (0);
	if (!load_map_walls_pictures(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_sec(den))
		return (0);
	if (!connect_map_sectors(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_sprites(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_sector_animations(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_wall_animations(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_checkpoints(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_level(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	return (1);
}

int		load_map_texture(int fd, t_packaging_texture **t, int count)
{
	int		i;

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

int		load_map_textures(t_doom *den)
{
	int		i;

	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t, TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, (void *)den->iw.tsz, sizeof(float) * TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);

	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_decor, DECOR_TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);

	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_pickup, PICK_UP_TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);

	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_enemies, ENEMIES_TEXTURES_COUNT))
		return (0);
	if (!read_check(den->fd, &i, sizeof(int)) || i != 0)
		return (0);

	if (!load_map_texture(den->fd, (t_packaging_texture **)den->iw.t_weap, WEAPONS_TEXTURES_COUNT))
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

int		load_map_music(t_doom *den)
{
	int		i;

	i = -1;
	while (++i < MUSIC_COUNT)
	{
		if (!read_check(den->fd, &(den->iw.sounds.music_pack_size[i]), sizeof(size_t)))
			return (0);
		den->iw.sounds.music_pack[i] = malloc(den->iw.sounds.music_pack_size[i]);
		if (!read_check(den->fd, den->iw.sounds.music_pack[i],
			den->iw.sounds.music_pack_size[i]))
			return (0);
	}
	return (1);
}

int		load_map_env(t_doom *den)
{
	int		i;

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

int		load4_game(t_doom *den)
{
	int			i;

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