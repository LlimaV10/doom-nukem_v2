/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_from_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:07:38 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 12:07:39 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	read_weapons_textures(t_sdl *iw)
{
	iw->t_weap_sur[0] = SDL_LoadBMP("weapons/0.bmp");
	iw->t_weap_sur[1] = SDL_LoadBMP("weapons/1.bmp");
	iw->t_weap_sur[2] = SDL_LoadBMP("weapons/2.bmp");
	iw->t_weap_sur[3] = SDL_LoadBMP("weapons/3.bmp");
	iw->t_weap_sur[4] = SDL_LoadBMP("weapons/4.bmp");
	iw->t_weap_sur[5] = SDL_LoadBMP("weapons/5.bmp");
	iw->t_weap_sur[6] = SDL_LoadBMP("weapons/6.bmp");
	iw->t_weap_sur[7] = SDL_LoadBMP("weapons/7.bmp");
	iw->t_weap_sur[8] = SDL_LoadBMP("weapons/8.bmp");
	iw->t_weap_sur[9] = SDL_LoadBMP("weapons/9.bmp");
	iw->t_weap_sur[10] = SDL_LoadBMP("weapons/10.bmp");
	iw->t_weap_sur[11] = SDL_LoadBMP("weapons/11.bmp");
	iw->t_weap_sur[12] = SDL_LoadBMP("weapons/12.bmp");
	iw->t_weap_sur[13] = SDL_LoadBMP("weapons/13.bmp");
	iw->t_weap_sur[14] = SDL_LoadBMP("weapons/14.bmp");
	iw->t_weap_sur[15] = SDL_LoadBMP("weapons/15.bmp");
	iw->t_weap_sur[16] = SDL_LoadBMP("weapons/16.bmp");
	iw->t_weap_sur[17] = SDL_LoadBMP("weapons/17.bmp");
	iw->t_weap_sur[18] = SDL_LoadBMP("weapons/18.bmp");
}

void	get_sounds_game(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < MUSIC_COUNT)
	{
		iw->sounds.music_rw[i] = SDL_RWFromConstMem(iw->sounds.music_pack[i],
			iw->sounds.music_pack_size[i]);
		iw->sounds.music[i] = Mix_LoadMUS_RW(iw->sounds.music_rw[i], 0);
	}
}

void	get_music(t_sdl *iw, int i, const char *file)
{
	void	*tmp;
	int		fd;

	fd = open(file, READ_MAP);
	if (fd < 0)
	{
		iw->sounds.music[i] = 0;
		return ;
	}
	tmp = malloc(MAX_MUSIC_SIZE);
	iw->sounds.music_pack_size[i] = read(fd, tmp, MAX_MUSIC_SIZE);
	close(fd);
	iw->sounds.music_pack[i] = malloc(iw->sounds.music_pack_size[i]);
	ft_memcpy(iw->sounds.music_pack[i], tmp, iw->sounds.music_pack_size[i]);
	free(tmp);
	iw->sounds.music_rw[i] = SDL_RWFromConstMem(iw->sounds.music_pack[i],
		iw->sounds.music_pack_size[i]);
	iw->sounds.music[i] = Mix_LoadMUS_RW(iw->sounds.music_rw[i], 0);
}

void	get_sounds2(t_sdl *iw)
{
	iw->sounds.env[0] = Mix_LoadWAV("sound/environment/0.wav");
	iw->sounds.env[1] = Mix_LoadWAV("sound/environment/1.wav");
	iw->sounds.env[2] = Mix_LoadWAV("sound/environment/2.wav");
	iw->sounds.env[3] = Mix_LoadWAV("sound/environment/3.wav");
	iw->sounds.env[4] = Mix_LoadWAV("sound/environment/4.wav");
	iw->sounds.env[5] = Mix_LoadWAV("sound/environment/5.wav");
	iw->sounds.env[6] = Mix_LoadWAV("sound/environment/6.wav");
	iw->sounds.env[7] = Mix_LoadWAV("sound/environment/7.wav");
	iw->sounds.env[8] = Mix_LoadWAV("sound/environment/8.wav");
	iw->sounds.env[9] = Mix_LoadWAV("sound/environment/9.wav");
	iw->sounds.env[10] = Mix_LoadWAV("sound/environment/10.wav");
	iw->sounds.env[11] = Mix_LoadWAV("sound/environment/11.wav");
	iw->sounds.env[12] = Mix_LoadWAV("sound/environment/12.wav");
	iw->sounds.env[13] = Mix_LoadWAV("sound/environment/13.wav");
	iw->sounds.env[14] = Mix_LoadWAV("sound/environment/14.wav");
	iw->sounds.env[15] = Mix_LoadWAV("sound/environment/15.wav");
	iw->sounds.env[16] = Mix_LoadWAV("sound/environment/16.wav");
	iw->sounds.env[17] = Mix_LoadWAV("sound/environment/17.wav");
	iw->sounds.env[18] = Mix_LoadWAV("sound/environment/18.wav");
	iw->sounds.env[19] = Mix_LoadWAV("sound/environment/19.wav");
	iw->sounds.env[20] = Mix_LoadWAV("sound/environment/20.wav");
	iw->sounds.env[21] = Mix_LoadWAV("sound/environment/21.wav");
}

void	get_sounds(t_sdl *iw)
{
	get_music(iw, 0, "sound/background/0.mp3");
	get_music(iw, 1, "sound/background/1.mp3");
	get_music(iw, 2, "sound/background/2.mp3");
	get_music(iw, 3, "sound/background/3.mp3");
	get_music(iw, 4, "sound/background/4.mp3");
	get_sounds2(iw);
}
