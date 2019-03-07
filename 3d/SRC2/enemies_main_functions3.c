/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_main_functions3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:46:42 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:46:43 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		enemy_sees_player3(t_sdl *iw, t_sprite *s, t_enemy_sees_player *esp)
{
	esp->a = (float)(esp->py - esp->ey);
	esp->b = (float)(esp->ex - esp->px);
	esp->c = (float)(esp->px * esp->ey - esp->ex * esp->py);
	if (!esp_check_walls(iw, esp))
		return (-1);
	s->e.vis_esp = *esp;
	return ((int)sqrtf(powf(esp->px - esp->ex, 2.0f)
		+ powf(esp->py - esp->ey, 2.0f)));
}

int		enemy_sees_player(t_sdl *iw, t_sprite *s)
{
	t_enemy_sees_player	esp;
	t_sector_ways		*ways;
	t_sector_way		*way;
	int					ret;

	esp.px = iw->p.x;
	esp.py = iw->p.y;
	esp.ex = s->x;
	esp.ey = s->y;
	esp.curr_sector = s->num_sec;
	if (iw->d.cs == s->num_sec)
		return (enemy_sees_player3(iw, s, &esp));
	ways = iw->ways[s->num_sec][iw->d.cs];
	while (ways)
	{
		way = ways->way_start;
		esp_get_new_player_coordinates(iw, way, &esp, s);
		if ((ret = enemy_sees_player2(iw, s, way, &esp)) != -1)
			return (ret);
		ways = ways->next;
	}
	return (-1);
}

int		move_enemy_in_portal2_1(t_sdl *iw, t_sprite *s,
			t_intpoint2d *vect, int wall)
{
	int		nx;
	int		ny;

	nx = s->x + vect->x + iw->walls[iw->walls[wall].nextsector_wall].x
		- iw->walls[wall].next->x;
	ny = s->y + vect->y + iw->walls[iw->walls[wall].nextsector_wall].y
		- iw->walls[wall].next->y;
	if (get_ceil_z_sec(iw, nx, ny, iw->walls[wall].nextsector) -
		get_floor_z_sec(iw, nx, ny, iw->walls[wall].nextsector) < SPRITE_HEIGHT)
		return (0);
	s->x = nx;
	s->y = ny;
	s->num_sec = iw->walls[wall].nextsector;
	s->e.vis_esp.ex += iw->walls[iw->walls[wall].nextsector_wall].x
		- iw->walls[wall].next->x;
	s->e.vis_esp.ey += iw->walls[iw->walls[wall].nextsector_wall].y
		- iw->walls[wall].next->y;
	s->e.vis_esp.px += iw->walls[iw->walls[wall].nextsector_wall].x
		- iw->walls[wall].next->x;
	s->e.vis_esp.py += iw->walls[iw->walls[wall].nextsector_wall].y
		- iw->walls[wall].next->y;
	return (1);
}

int		move_enemy_in_portal2(t_sdl *iw, t_sprite *s,
	t_intpoint2d *vect, int wall)
{
	float	k1;
	float	k2;

	s->e.vis_esp.a = (float)vect->y;
	s->e.vis_esp.b = (float)(-vect->x);
	s->e.vis_esp.c = (float)((s->x + vect->x) * s->y - s->x * (s->y + vect->y));
	k1 = s->e.vis_esp.a * iw->walls[wall].x + s->e.vis_esp.b
		* iw->walls[wall].y + s->e.vis_esp.c;
	k2 = s->e.vis_esp.a * iw->walls[wall].next->x +
		s->e.vis_esp.b * iw->walls[wall].next->y + s->e.vis_esp.c;
	if ((!((k1 < 0.0f && k2 > 0.0f) || (k1 > 0.0f && k2 < 0.0f))) ||
		(!move_enemy_in_portal2_1(iw, s, vect, wall)))
		return (0);
	return (1);
}

int		move_enemy_in_portal(t_sdl *iw, t_sprite *s, t_intpoint2d *vect)
{
	int		wall;
	float	k1;
	float	k2;

	wall = iw->sectors[s->num_sec].sw - 1;
	while (++wall < iw->sectors[s->num_sec].sw + iw->sectors[s->num_sec].nw)
	{
		if (iw->walls[wall].nextsector == -1 || iw->walls[wall].glass != -1)
			continue;
		k1 = iw->walls[wall].l.a * s->x + iw->walls[wall].l.b
			* s->y + iw->walls[wall].l.c;
		k2 = iw->walls[wall].l.a * (s->x + vect->x)
			+ iw->walls[wall].l.b * (s->y + vect->y) + iw->walls[wall].l.c;
		if (((k1 < 0.0f && k2 > 0.0f) || (k1 > 0.0f && k2 < 0.0f))
			&& move_enemy_in_portal2(iw, s, vect, wall))
			return (1);
	}
	return (0);
}
