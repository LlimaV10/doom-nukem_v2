/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_defaults_sprites.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:06:05 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:06:06 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_sprite_texture(t_sdl *iw, t_sprite *s)
{
	if (s->type == 0)
	{
		s->t = iw->t_decor[s->t_numb];
		s->t_kernel = &iw->k.m_t_decor[s->t_numb];
	}
	else if (s->type == 1)
	{
		s->t = iw->t_pickup[s->t_numb];
		s->t_kernel = &iw->k.m_t_pickup[s->t_numb];
	}
	else if (s->type == 2)
	{
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}
}

void	set_sprites_z(t_sdl *iw)
{
	t_sprite	*s;

	s = *(iw->sprite);
	while (s)
	{
		get_sprite_texture(iw, s);
		if (s->type != 2 || s->e.enemy_numb != 0)
			s->z = get_floor_z_sec(iw, s->x, s->y, s->num_sec);
		s = s->next;
	}
}
