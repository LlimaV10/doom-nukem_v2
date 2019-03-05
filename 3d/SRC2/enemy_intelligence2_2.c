/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_intelligence2_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:45:36 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:45:37 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	enemy_intelligence2_s0(t_sdl *iw, t_sprite *s, int i)
{
	if (clock() - s->e.previous_picture_change > CLKS_P_S / 3)
	{
		s->e.previous_picture_change = clock();
		if (s->t_numb == 20)
			s->t_numb = 20;
		else
			s->t_numb = 20;
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}
	if ((i = enemy_sees_player(iw, s)) != -1)
	{
		if ((i < 10000 || s->e.health < ENEMY_HEALTH2) && i > 4000)
			s->e.status = 2;
		else if (i <= 4000)
			s->e.status = 3;
	}
}
