# define _CRT_SECURE_NO_WARNINGS

#include "doom_nuken.h"

void	set_sector_wall_light_numbs(t_doom *den, t_sector_list *sec)
{
	t_wals		*tmp;
	t_picture	*pic;
	int			num_wall;
	int			num_pic;

	if (sec->light == 0)
		return;
	tmp = den->tmp;
	num_wall = 0;
	while (tmp)
	{
		pic = tmp->p;
		num_pic = 0;
		while (pic)
		{
			if (pic == sec->light)
			{
				sec->light_wall_numb = num_wall;
				sec->light_numb = num_pic;
				return;
			}
			pic = pic->next;
			num_pic++;
		}
		tmp = tmp->next;
		num_wall++;
	}
}

void	save_reverse_map_sec(t_doom *den, t_sector_list *tmp)
{
	if (tmp->next != 0)
		save_reverse_map_sec(den, tmp->next);
	set_sector_wall_light_numbs(den, tmp);
	write(den->fd, tmp, sizeof(t_sector_list));
	if (tmp->fr.n != 0)
		write(den->fd, tmp->fr.n, sizeof(t_vector));
	if (tmp->cl.n != 0)
		write(den->fd, tmp->cl.n, sizeof(t_vector));
}

void	save_map_sec(t_doom *den)
{
	int		i;

	i = den->sec;
	write(den->fd, &i, sizeof(int));
	if (den->sectors)
		save_reverse_map_sec(den, den->sectors);
}

int		count_pictures(t_picture *pic)
{
	int		i;

	i = 0;
	while (pic)
	{
		i++;
		pic = pic->next;
	}
	return (i);
}

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
	write(den->fd, spr, sizeof(t_sprite) - sizeof(int) * (WINDOW_W * 2 + 2));
}

void	save_map_sprites(t_doom *den)
{
	write(den->fd, &den->sprites, sizeof(int));
	if (den->sprite)
		save_reverse_map_sprites(den, den->sprite);
}

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
				return;
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
				return;
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
			return;
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

void	save_map_textures(t_doom *den, int zero)
{
	int		i;

	i = -1;
	while (++i < TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t[i]->pixels, den->iw.t[i]->pitch * den->iw.t[i]->h);
	}
	write(den->fd, (void *)den->iw.tsz, sizeof(float) * TEXTURES_COUNT);
	write(den->fd, &zero, sizeof(int));
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_decor[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_decor[i]->pixels, den->iw.t_decor[i]->pitch * den->iw.t_decor[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_pickup[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_pickup[i]->pixels, den->iw.t_pickup[i]->pitch * den->iw.t_pickup[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_enemies[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_enemies[i]->pixels, den->iw.t_enemies[i]->pitch * den->iw.t_enemies[i]->h);
	}
	write(den->fd, &zero, sizeof(int));
	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
	{
		write(den->fd, den->iw.t_weap[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.t_weap[i]->pixels, den->iw.t_weap[i]->pitch * den->iw.t_weap[i]->h);
	}
	write(den->fd, &zero, sizeof(int));

	write(den->fd, den->iw.hud.enot, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.enot->pixels, den->iw.hud.enot->pitch * den->iw.hud.enot->h);
	write(den->fd, &zero, sizeof(int));

	write(den->fd, den->iw.hud.miss, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.miss->pixels, den->iw.hud.miss->pitch * den->iw.hud.miss->h);
	write(den->fd, &zero, sizeof(int));

	write(den->fd, den->iw.hud.saved, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.saved->pixels, den->iw.hud.saved->pitch * den->iw.hud.saved->h);
	write(den->fd, &zero, sizeof(int));

	write(den->fd, den->iw.hud.dead, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.dead->pixels, den->iw.hud.dead->pitch * den->iw.hud.dead->h);
	write(den->fd, &zero, sizeof(int));

	write(den->fd, den->iw.hud.win, sizeof(t_packaging_texture));
	write(den->fd, den->iw.hud.win->pixels, den->iw.hud.win->pitch * den->iw.hud.win->h);
	write(den->fd, &zero, sizeof(int));

	i = -1;
	while (++i < 3)
	{
		write(den->fd, den->iw.bag.button[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.bag.button[i]->pixels, den->iw.bag.button[i]->pitch * den->iw.bag.button[i]->h);
	}
	write(den->fd, &zero, sizeof(int));

	write(den->fd, den->iw.map.player, sizeof(t_packaging_texture));
	write(den->fd, den->iw.map.player->pixels, den->iw.map.player->pitch * den->iw.map.player->h);
	write(den->fd, &zero, sizeof(int));
	
	i = -1;
	while (++i < 6)
	{
		write(den->fd, den->iw.menu.icons[i], sizeof(t_packaging_texture));
		write(den->fd, den->iw.menu.icons[i]->pixels, den->iw.menu.icons[i]->pitch * den->iw.menu.icons[i]->h);
	}
	write(den->fd, &zero, sizeof(int));

	// Иконки едитора не нужны
	//i = -1;
	//while (++i < 13) //////////////////////Заменить Сюрфейс на ПАК_ТЕКС
	//{
	//	write(den->fd, den->icon[i], sizeof(t_packaging_texture));
	//	write(den->fd, den->icon[i]->pixels, den->icon[i]->pitch * den->icon[i]->h);
	//}
	//write(den->fd, &zero, sizeof(int));
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

void	save_map_kernel_cl(t_doom *den)
{
	
}

void	save_map_file(t_doom *den)
{
	int		zero;

	den->fd = open(den->fname, WRITE_MAP);
	if (den->fd < 0)
		return;
	zero = 0;
	write(den->fd, "This is map created for doom-nukem by dbolilyi & ddehtyar & kkostrub & elopukh", 78);
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
