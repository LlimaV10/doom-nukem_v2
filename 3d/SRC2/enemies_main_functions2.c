/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_main_functions2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:46:39 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:46:40 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		esp_check_walls(t_sdl *iw, t_enemy_sees_player *esp)
{
	int		wall;

	wall = iw->sectors[esp->curr_sector].sw - 1;
	while (++wall < iw->sectors[esp->curr_sector].sw
		+ iw->sectors[esp->curr_sector].nw)
	{
		if (iw->walls[wall].nextsector != -1)
			continue;
		esp->k1 = iw->walls[wall].l.a * (float)esp->px +
			iw->walls[wall].l.b * (float)esp->py + iw->walls[wall].l.c;
		esp->k2 = iw->walls[wall].l.a * (float)esp->ex +
			iw->walls[wall].l.b * (float)esp->ey + iw->walls[wall].l.c;
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f)
			|| (esp->k1 < 0.0f && esp->k2 > 0.0f))
		{
			esp->k1 = esp->a * (float)iw->walls[wall].x +
				esp->b * (float)iw->walls[wall].y + esp->c;
			esp->k2 = esp->a * (float)iw->walls[wall].next->x
				+ esp->b * (float)iw->walls[wall].next->y + esp->c;
			if ((esp->k1 > 0.0f && esp->k2 < 0.0f)
			|| (esp->k1 < 0.0f && esp->k2 > 0.0f))
				return (0);
		}
	}
	return (1);
}

int		esp_check_portal(t_sdl *iw, t_enemy_sees_player *esp, int portal)
{
	if (iw->walls[portal].glass >= 0 ||
		iw->walls[iw->walls[portal].nextsector_wall].glass >= 0)
		return (0);
	esp->k1 = iw->walls[portal].l.a * (float)esp->px +
		iw->walls[portal].l.b * (float)esp->py + iw->walls[portal].l.c;
	esp->k2 = iw->walls[portal].l.a * (float)esp->ex +
		iw->walls[portal].l.b * (float)esp->ey + iw->walls[portal].l.c;
	if ((esp->k1 > 0.0f && esp->k2 < 0.0f)
		|| (esp->k1 < 0.0f && esp->k2 > 0.0f))
	{
		esp->k1 = esp->a * (float)iw->walls[portal].x +
			esp->b * (float)iw->walls[portal].y + esp->c;
		esp->k2 = esp->a * (float)iw->walls[portal].next->x +
			esp->b * (float)iw->walls[portal].next->y + esp->c;
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f) ||
			(esp->k1 < 0.0f && esp->k2 > 0.0f))
			return (1);
	}
	return (0);
}

void	esp_get_new_player_coordinates(t_sdl *iw, t_sector_way *way,
			t_enemy_sees_player *esp, t_sprite *s)
{
	esp->px = iw->p.x;
	esp->py = iw->p.y;
	esp->ex = s->x;
	esp->ey = s->y;
	esp->curr_sector = s->num_sec;
	while (way)
	{
		esp->px -= iw->walls[iw->walls[way->portal].nextsector_wall].x
			- iw->walls[way->portal].next->x;
		esp->py -= iw->walls[iw->walls[way->portal].nextsector_wall].y
			- iw->walls[way->portal].next->y;
		way = way->next;
	}
}

int		enemy_sees_player2_1(t_sdl *iw, t_sprite *s,
	t_sector_way *way, t_enemy_sees_player *esp)
{
	if (!way)
	{
		esp->a = (float)(esp->py - esp->ey);
		esp->b = (float)(esp->ex - esp->px);
		esp->c = (float)(esp->px * esp->ey - esp->ex * esp->py);
		if (esp_check_walls(iw, esp))
		{
			s->e.vis_esp = *esp;
			return ((int)sqrtf(powf(esp->px - esp->ex, 2.0f)
				+ powf(esp->py - esp->ey, 2.0f)));
		}
	}
	return (-1);
}

int		enemy_sees_player2(t_sdl *iw, t_sprite *s,
			t_sector_way *way, t_enemy_sees_player *esp)
{
	while (way)
	{
		esp->a = (float)(esp->py - esp->ey);
		esp->b = (float)(esp->ex - esp->px);
		esp->c = (float)(esp->px * esp->ey - esp->ex * esp->py);
		if (!esp_check_walls(iw, esp) ||
			!esp_check_portal(iw, esp, way->portal))
			break ;
		esp->px += iw->walls[iw->walls[way->portal].nextsector_wall].x
			- iw->walls[way->portal].next->x;
		esp->py += iw->walls[iw->walls[way->portal].nextsector_wall].y
			- iw->walls[way->portal].next->y;
		esp->ex += iw->walls[iw->walls[way->portal].nextsector_wall].x
			- iw->walls[way->portal].next->x;
		esp->ey += iw->walls[iw->walls[way->portal].nextsector_wall].y
			- iw->walls[way->portal].next->y;
		esp->curr_sector = iw->walls[way->portal].nextsector;
		way = way->next;
	}
	return (enemy_sees_player2_1(iw, s, way, esp));
}
