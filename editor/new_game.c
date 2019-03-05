/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:04:48 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/05 15:04:49 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		main_new(t_doom *den)
{
	int		ret;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	ret = get_3d_def_new(den);
	if (ret == 0)
	{
		write(1, "Error, check your files which need to be exist\n", 47);
		return (0);
	}
	else if (ret == -1)
	{
		write(1, "OpenCL initialization error\n", 28);
		den->iw.v.kernel = 0;
	}
	den->color = 0xFFFFFF;
	den->b_color = 0x505050;
	ft_sdl_init(den);
	return (0);
}

void	get_3d_def_new_init(t_doom *den)
{
	den->iw.v.game_mode = 0;
	den->iw.v.kernel = 1;
	den->iw.l.accel = 9.81f;
	den->iw.l.skybox = 13;
	den->iw.l.story = 9;
}

int		get_3d_def_new(t_doom *den)
{
	int		ret;

	get_3d_def_new_init(den);
	den->iw.v.look_wall = (t_wall **)malloc(sizeof(t_wall *));
	den->iw.v.look_sector = (t_sector **)malloc(sizeof(t_sector *));
	den->iw.v.look_picture = (t_picture **)malloc(sizeof(t_picture *));
	den->iw.sprite = (t_sprite **)malloc(sizeof(t_sprite *));
	den->iw.vw_save = (t_save_wall **)malloc(sizeof(t_save_wall *));
	den->iw.checkpoints = 0;
	den->iw.sector_animations = 0;
	den->iw.wall_animations = 0;
	get_font_file(den);
	load_kernel(&den->iw.k, &den->iw);
	read_textures(&den->iw);
	read_sprites_textures(&den->iw);
	read_weapons_textures(&den->iw);
	get_sounds(&den->iw);
	if ((ret = check_all_validation(&den->iw)) == 0)
		return (0);
	get_packaging_textures(&den->iw);
	get_kernel_mem(&den->iw);
	get_kernels(&den->iw);
	get_guns(&den->iw);
	return (ret);
}
