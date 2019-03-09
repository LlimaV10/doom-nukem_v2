/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_loops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:06:16 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:06:17 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	sound_loop(t_sdl *iw)
{
	// if (!Mix_PlayingMusic())
	// {
	// 	Mix_PlayMusic(iw->sounds.music[iw->v.next_music], 0);
	// 	iw->v.next_music++;
	// 	if (iw->v.next_music >= MUSIC_COUNT)
	// 		iw->v.next_music = 0;
	// }
}

void	environment_loop(t_sdl *iw)
{
	if (iw->v.fly_mode == 0 && iw->v.fall == 1 &&
		(iw->v.front != 1 || iw->v.back != 1 ||
		iw->v.left != 1 || iw->v.right != 1))
	{
		if (!Mix_Playing(0))
			Mix_PlayChannel(0, iw->sounds.env[0], -1);
	}
	else if (iw->v.fly_mode == 0 && Mix_Playing(0))
		Mix_HaltChannel(0);
	if (iw->v.fly_mode && !Mix_Playing(0))
		Mix_PlayChannel(0, iw->sounds.env[18], -1);
}
