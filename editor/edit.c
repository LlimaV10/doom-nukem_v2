/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 14:27:18 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/05 14:27:19 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		main_edit(t_doom *den)
{
	int		ret;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	ret = get_3d_def_edit(den);
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

int		get_3d_def_edit(t_doom *den)
{
	int		ret;

	den->iw.v.game_mode = 0;
	den->iw.v.kernel = 1;
	den->iw.v.look_wall = (t_wall **)malloc(sizeof(t_wall *));
	den->iw.v.look_sector = (t_sector **)malloc(sizeof(t_sector *));
	den->iw.v.look_picture = (t_picture **)malloc(sizeof(t_picture *));
	den->iw.sprite = (t_sprite **)malloc(sizeof(t_sprite *));
	den->iw.vw_save = (t_save_wall **)malloc(sizeof(t_save_wall *));
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

void	main3d_edit_free(t_doom *den)
{
	if (den->iw.v.sector_anim != 0)
		free(den->iw.v.sector_anim);
	if (den->iw.v.wall_anim != 0)
		free(den->iw.v.wall_anim);
	undo_animations(&den->iw);
	unget_map(&den->iw, den, den->tmp);
	free(den->iw.walls);
	free(den->iw.sectors);
	free_sector_ways(&den->iw);
}

void	main3d_edit(t_doom *den)
{
	give_date(den, &den->iw);
	get_def_new(&den->iw);
	den->iw.win = SDL_CreateWindow("Guardians of the Galaxy",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H + 300, SDL_WINDOW_SHOWN);
	SDL_SetRelativeMouseMode(den->iw.v.mouse_mode);
	den->iw.sur = SDL_GetWindowSurface(den->iw.win);
	circle(&den->iw.hud, FOOTX, FOOTY);
	draw_tex_to_select(&den->iw);
	draw_decor_tex_to_select(&den->iw);
	draw_menu(&den->iw);
	get_map(&den->iw, den);
	set_sprites_z(&den->iw);
	get_sectors_ways(&den->iw);
	create_map(&den->iw);
	draw(&den->iw);
	SDL_UpdateWindowSurface(den->iw.win);
	main_loop(&den->iw);
	SDL_SetRelativeMouseMode(0);
	SDL_FreeSurface(den->iw.sur);
	SDL_DestroyWindow(den->iw.win);
	main3d_edit_free(den);
	Mix_HaltMusic();
}
