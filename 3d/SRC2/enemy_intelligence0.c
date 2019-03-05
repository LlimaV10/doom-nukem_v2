/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_intelligence0.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:05:15 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 12:08:06 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	enemy_intelligence0_s0(t_sdl *iw, t_sprite *s)
{
	int		i;

	if (clock() - s->e.previous_picture_change > CLKS_P_S / 3)
	{
		s->e.previous_picture_change = clock();
		if (s->t_numb == 0)
			s->t_numb = 2;
		else
			s->t_numb = 0;
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}
	if ((i = enemy_sees_player(iw, s)) != -1)
	{
		if ((i < 10000 || s->e.health < ENEMY_HEALTH0) && i > 800)
			move_enemy(iw, s);
		else if (i <= 800)
			s->e.status = 3;
	}
	else if (s->e.vis_esp.curr_sector != -1)
	{
		if ((i = (int)sqrtf(powf(s->x - s->e.vis_esp.px, 2.0f)
			+ powf(s->y - s->e.vis_esp.py, 2.0f))) > 10)
			move_enemy(iw, s);
	}
}

void	enemy_intelligence0_s3(t_sdl *iw, t_sprite *s)
{
	s->e.previous_picture_change = clock();
	if (s->t_numb != 3 && s->t_numb != 4)
		s->t_numb = 3;
	else if (s->t_numb == 3)
	{
		s->t_numb = 4;
		iw->p.health -= ENEMY_DAMAGE0 * iw->menu.count;
		if (!Mix_Playing(5))
			Mix_PlayChannel(5, iw->sounds.env[13], 0);
		if (!Mix_Playing(4))
			Mix_PlayChannel(4, iw->sounds.env[9], 0);
	}
	else
	{
		s->t_numb = 0;
		s->e.status = 0;
	}
	s->t = iw->t_enemies[s->t_numb];
	s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
}

void	enemy_intelligence0_2(t_sdl *iw, t_sprite *s)
{
	int		i;

	if (s->e.status == 4 && clock() -
		s->e.previous_picture_change > CLKS_P_S / 5)
	{
		s->e.previous_picture_change = clock();
		if (s->t_numb < 5)
			s->t_numb = 5;
		else if (s->t_numb < 7)
			s->t_numb++;
		else
			s->e.status = 5;
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}
	else if (s->e.status == 1)
		if (s->e.health < 10 ||
			((i = enemy_sees_player(iw, s)) != -1 && i < 1000))
		{
			s->e.status = 0;
			s->t_numb = 0;
			s->t = iw->t_enemies[s->t_numb];
			s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
		}
}

void	enemy_intelligence0(t_sdl *iw, t_sprite *s)
{
	if (s->e.status == 0)
		enemy_intelligence0_s0(iw, s);
	else if (s->e.status == 3 && clock() -
		s->e.previous_picture_change > CLKS_P_S / 5)
		enemy_intelligence0_s3(iw, s);
	else
		enemy_intelligence0_2(iw, s);
	sprite_physics(iw, s);
	if (s->e.health < 0 && s->e.status < 4)
	{
		Mix_PlayChannel(5, iw->sounds.env[20], 0);
		s->e.status = 4;
	}
}
