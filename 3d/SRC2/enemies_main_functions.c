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
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
		{
			esp->k1 = esp->a * (float)iw->walls[wall].x +
				esp->b * (float)iw->walls[wall].y + esp->c;
			esp->k2 = esp->a * (float)iw->walls[wall].next->x
				+ esp->b * (float)iw->walls[wall].next->y + esp->c;
			if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
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
	if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
	{
		esp->k1 = esp->a * (float)iw->walls[portal].x +
			esp->b * (float)iw->walls[portal].y + esp->c;
		esp->k2 = esp->a * (float)iw->walls[portal].next->x +
			esp->b * (float)iw->walls[portal].next->y + esp->c;
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
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
		if (!esp_check_walls(iw, esp) || !esp_check_portal(iw, esp, way->portal))
			break;
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
	s->x += vect->x + iw->walls[iw->walls[wall].nextsector_wall].x
		- iw->walls[wall].next->x;
	s->y += vect->y + iw->walls[iw->walls[wall].nextsector_wall].y
		- iw->walls[wall].next->y;
	s->num_sec = iw->walls[wall].nextsector;
	s->e.vis_esp.ex += iw->walls[iw->walls[wall].nextsector_wall].x
		- iw->walls[wall].next->x;
	s->e.vis_esp.ey += iw->walls[iw->walls[wall].nextsector_wall].y
		- iw->walls[wall].next->y;
	s->e.vis_esp.px += iw->walls[iw->walls[wall].nextsector_wall].x
		- iw->walls[wall].next->x;
	s->e.vis_esp.py += iw->walls[iw->walls[wall].nextsector_wall].y
		- iw->walls[wall].next->y;
}

int		move_enemy_in_portal2(t_sdl *iw, t_sprite *s, t_intpoint2d *vect, int wall)
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
	if (!((k1 < 0.0f && k2 > 0.0f) || (k1 > 0.0f && k2 < 0.0f)))
		return (0);
	move_enemy_in_portal2_1(iw, s, vect, wall);
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

int		move_enemy(t_sdl *iw, t_sprite *s)
{
	t_point2d		vect_norm;
	t_intpoint2d	vect;
	float			len;
	clock_t			t;

	vect_norm.x = s->e.vis_esp.px - s->e.vis_esp.ex;
	vect_norm.y = s->e.vis_esp.py - s->e.vis_esp.ey;
	len = sqrtf(powf(vect_norm.x, 2.0f) + powf(vect_norm.y, 2.0f));
	vect_norm.x /= len;
	vect_norm.y /= len;
	t = clock();
	vect.x = vect_norm.x * (float)(t - s->e.prev_update_time)
		/ (float)CLKS_P_S * 1000.0f;
	vect.y = vect_norm.y * (float)(t - s->e.prev_update_time)
		/ (float)CLKS_P_S * 1000.0f;
	if (in_sec_xy(iw, s->num_sec, s->x + vect.x, s->y + vect.y))
	{
		s->x += vect.x;
		s->y += vect.y;
		return (1);
	}
	return (move_enemy_in_portal(iw, s, &vect));
}

void	sprite_physics(t_sdl *iw, t_sprite *s)
{
	int		tmp;

	if (s->fall_time != 1 && (s->e.enemy_numb != 0 || s->e.status >= 4))
		s->z -= (int)(iw->l.accel * ((float)(clock() - s->fall_time) /
			(float)CLKS_P_S) * 50.0f);
	tmp = get_ceil_z_sec(iw, s->x, s->y, s->num_sec);
	if (s->z + SPRITE_HEIGHT > tmp)
		s->z = tmp - SPRITE_HEIGHT;
	tmp = get_floor_z_sec(iw, s->x, s->y, s->num_sec);
	if (s->z < tmp)
	{
		s->z = tmp;
		s->fall_time = 1;
	}
	else
		s->fall_time = clock() - CLKS_P_S / 10;
}

void	check_enemies(t_sdl *iw)
{
	t_sprite	*tmp;

	tmp = *iw->sprite;
	while (tmp)
	{
		if (tmp->type == 2)
		{
			if (tmp->e.enemy_numb == 0)
				enemy_intelligence0(iw, tmp);
			else if (tmp->e.enemy_numb == 1)
				enemy_intelligence1(iw, tmp);
			else if (tmp->e.enemy_numb == 2)
				enemy_intelligence2(iw, tmp, 0);
			tmp->e.prev_update_time = clock();
		}
		tmp = tmp->next;
	}
}