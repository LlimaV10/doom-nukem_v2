/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_mode.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:29:13 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/05 15:29:14 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		get_3d_def_game(t_doom *den)
{
	den->iw.v.game_mode = 1;
	den->iw.v.kernel = 1;
	den->iw.v.look_wall = (t_wall **)malloc(sizeof(t_wall *));
	den->iw.v.look_sector = (t_sector **)malloc(sizeof(t_sector *));
	den->iw.v.look_picture = (t_picture **)malloc(sizeof(t_picture *));
	den->iw.sprite = (t_sprite **)malloc(sizeof(t_sprite *));
	den->iw.vw_save = (t_save_wall **)malloc(sizeof(t_save_wall *));
	den->iw.arial_font = TTF_OpenFontRW(SDL_RWFromConstMem(den->iw.font_pack,
		den->iw.font_pack_size), 0, 24);
	if (den->iw.arial_font == 0)
	{
		write(1, "Error loading file\n", 20);
		return (0);
	}
	den->font = den->iw.arial_font;
	load_kernel(&den->iw.k, &den->iw);
	get_kernel_mem(&den->iw);
	get_kernels(&den->iw);
	get_guns(&den->iw);
	get_sounds_game(&den->iw);
	return (1);
}

void	main3d_game_help(t_doom *den)
{
	circle(&den->iw.hud, FOOTX, FOOTY);
	get_map(&den->iw, den);
	set_sprites_z(&den->iw);
	get_sectors_ways(&den->iw);
	create_map(&den->iw);
	Mix_PlayMusic(den->iw.sounds.music[4], -1);
	image_loop(&den->iw, den->iw.t[den->iw.l.story]);
	menu_loop(&den->iw);
	Mix_HaltMusic();
	draw(&den->iw);
}

void	main3d_game(t_doom *den)
{
	give_date(den, &den->iw);
	get_def_new(&den->iw);
	den->iw.win = SDL_CreateWindow("Guardians of the Galaxy",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	SDL_SetRelativeMouseMode(den->iw.v.mouse_mode);
	den->iw.sur = SDL_GetWindowSurface(den->iw.win);
	main3d_game_help(den);
	SDL_UpdateWindowSurface(den->iw.win);
	main_loop(&den->iw);
	SDL_SetRelativeMouseMode(0);
	SDL_FreeSurface(den->iw.sur);
	SDL_DestroyWindow(den->iw.win);
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

int		main_game(t_doom *den)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	if (!get_3d_def_game(den))
		return (0);
	main3d_game(den);
	return (0);
}
