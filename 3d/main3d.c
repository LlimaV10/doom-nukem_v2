#include "guardians.h"
#include <stdio.h>



void	exit_x(t_sdl *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	TTF_Quit();
	SDL_Quit();
	system("leaks doom-nukem");
	exit(0);
}

void	ft_scaled_blit(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;
	int		p;

	i = -1;
	while (++i < rect->h)
	{
		j = -1;
		while (++j < rect->w)
		{
			p = get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h);
			if (p != 0x010000)
				set_pixel2(winsur, rect->x + j, rect->y + i, p);
		}
	}
}

void	ft_scaled_blit2(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect)
{
	int		i;
	int		j;

	i = -1;
	while (++i < rect->h)
	{
		j = -1;
		while (++j < rect->w)
			set_pixel2(winsur, rect->x + j, rect->y + i, 
				get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h));
	}
}

// HUD ////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////

void	draw_gun(t_sdl *iw)
{
	int		i;
	int		j;
	int		to_i;
	int		start_j;
	int		to_j;
	int		pixel;
	SDL_Rect	changed_rect;

	changed_rect = iw->guns.t_rect[iw->guns.t];
	changed_rect.x += iw->v.weapon_change_x;
	changed_rect.y += iw->v.weapon_change_y + iw->v.weapon_change_y_hide;
	to_i = changed_rect.y + changed_rect.h;
	if (to_i > WINDOW_H)
		to_i = WINDOW_H;
	if (changed_rect.y < 0)
		i = -1;
	else
		i = changed_rect.y - 1;
	if (changed_rect.x < 0)
		start_j = -1;
	else
		start_j = changed_rect.x - 1;
	to_j = changed_rect.w + changed_rect.x;
	if (to_j > WINDOW_W)
		to_j = WINDOW_W;
	while (++i < to_i)
	{
		j = start_j;
		while (++j < to_j)
		{
			pixel = get_pixel(iw->t_weap[iw->guns.t],
				(j - changed_rect.x) * iw->t_weap[iw->guns.t]->w / changed_rect.w,
				(i - changed_rect.y) * iw->t_weap[iw->guns.t]->h / changed_rect.h);
			if (pixel != 0x010000)
				set_pixel2(iw->sur, j, i, get_light_color(pixel, iw->sectors[iw->d.cs].light));
		}
	}
}

//BACKPACK/////////////////////////////////////////////////////////


// void	draw_item(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect)
// {
// 	int		i;
// 	int		j;
//     int     color;

// 	i = -1;
// 	while (++i < rect->w)
// 	{
// 		j = -1;
// 		while (++j < rect->h)
//         {
//             color = get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h);
//             if (color != 0x010000)
//                 set_pixel2(winsur, rect->x + j, rect->y + i, color);
//         }
// 	}
	
// }

//////////////////// NEED TO DELETE SPRITE AND DO SOME...


// just deletes item from inventory



// void draw_button(t_sdl	*iw, SDL_Rect	*rect, t_packaging_texture *texture)
// {
// 	int x;
// 	int y;
// 	int color;

// 	x = -1;
// 	while(++x < rect->w)
// 	{
// 		y = -1;
// 		while(++y < rect->h)
// 		{
// 			color = get_pixel(texture, texture->w * x / rect->w, texture->h * y / rect->h);
//             if (color != 0xff00e3 && color != 0x000000)
//                 set_pixel2(iw->sur, rect->x + x, rect->y + y, color);
// 		}
// 	}
// }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	update(t_sdl *iw)
{
	SDL_FillRect(iw->sur, &iw->winrect, 0x000000);
	draw(iw);
	draw_some_info(iw);
	if (iw->v.game_mode)
	{
		make_health(&iw->hud, iw);
		draw_checkpoint_text(iw);
	}
	if (iw->bag.bag != 1 && iw->map.back != 1)
		draw_crosshair(iw);
	if (iw->hud.miss_time != 1)
	{
		if (clock() - iw->hud.miss_time < HUD_MISS_TIME)
			draw_miss(iw);
		else
			iw->hud.miss_time = 1;
	}
	if (iw->hud.saved_time != 1)
	{
		if (clock() - iw->hud.saved_time < HUD_SAVED_TIME)
			draw_save(iw);
		else
			iw->hud.saved_time = 1;
	}
	if (iw->bag.bag == 1 || iw->map.back == 1)
		transparency(iw);
	if (iw->bag.bag == 1)
		draw_items(iw);
	if (iw->map.back == 1)
		draw_minimap(iw);
	if (iw->v.game_mode)
		draw_icon_bag(iw);
	draw_selected_tex(iw);
	draw_selected_sprite(iw);
	SDL_UpdateWindowSurface(iw->win);
	//printf("Update\n");
	//printf("update ret %d\n", ret);
	/*if (iw->v.look_sprite == 0)
		printf("SPRITE NOT SELECTED\n");
	else
		printf("SPRITE SELECTED\n");*/
}

void	rotate_fc(t_sector_fc *fc, int xy, int pl)
{
	if (fc->n == 0)
	{
		fc->n = (t_vector *)malloc(sizeof(t_vector));
		fc->n->c = INCLINED_FC_Z;
		fc->n->a = 0;
		fc->n->b = 0;
	}
	if (xy && ((pl > 0 && fc->n->b < MAX_INCLINED_FC_XY) || (pl < 0 && fc->n->b > -MAX_INCLINED_FC_XY)))
		fc->n->b += pl;
	else if (!xy && ((pl > 0 && fc->n->a < MAX_INCLINED_FC_XY) || (pl < 0 && fc->n->a > -MAX_INCLINED_FC_XY)))
		fc->n->a += pl;
	if (fc->n->a == 0 && fc->n->b == 0)
	{
		free(fc->n);
		fc->n = 0;
	}
	else
		fc->n->d = -fc->n->a * fc->x - fc->n->b * fc->y - fc->n->c * fc->z;
}

void	get_wall_line2(t_wall *wall)
{
	wall->l.a = (float)(wall->next->y - wall->y);
	wall->l.b = (float)(wall->x - wall->next->x);
	wall->l.c = (float)(wall->next->x * wall->y -
		wall->x * wall->next->y);
}

void	do_sector_animations(t_sdl *iw)
{
	t_sector_animation	*tmp;

	tmp = iw->sector_animations;
	while (tmp)
	{
		if (clock() - tmp->prev_clock > CLKS_P_S / 50)
		{
			if (tmp->status == 1 && abs(tmp->curr_dy) < abs(tmp->dy))//tmp->curr_dy != tmp->dy)
			{
				do_sector_animation_step(iw, tmp, ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10);
				tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
			}
			else if (tmp->status == 0 && tmp->curr_dy != 0)
			{
				do_sector_animation_step(iw, tmp, ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10);
				tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
			}
			tmp->prev_clock = clock();
		}
		tmp = tmp->next;
	}
}

void	do_wall_animations(t_sdl *iw)
{
	t_wall_animation	*tmp;

	tmp = iw->wall_animations;
	while (tmp)
	{
		if (clock() - tmp->prev_clock > CLKS_P_S / 50)
		{
			if (tmp->status == 1)
			{
				if (tmp->priority == 0)
				{
					if (abs(tmp->curr_dx) < abs(tmp->dx))
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (abs(tmp->curr_dy) < abs(tmp->dy))
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
				else
				{
					if (abs(tmp->curr_dy) < abs(tmp->dy))
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy > 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (abs(tmp->curr_dx) < abs(tmp->dx))
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx > 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
			}
			else
			{
				if (tmp->priority == 0)
				{
					if (tmp->curr_dy != 0)
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (tmp->curr_dx != 0)
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
				else
				{
					if (tmp->curr_dx != 0)
					{
						do_wall_animation_step_dx(iw, tmp, ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dx += ((tmp->dx < 0) ? tmp->speed : -tmp->speed) * 10;
					}
					else if (tmp->curr_dy != 0)
					{
						do_wall_animation_step_dy(iw, tmp, ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10);
						tmp->curr_dy += ((tmp->dy < 0) ? tmp->speed : -tmp->speed) * 10;
					}
				}
			}

			tmp->prev_clock = clock();
		}
		tmp = tmp->next;
	}
}

int		check_walls_collisions_on_line_segment(t_sdl *iw, int wall, int len)
{
	//float	len_point;
	float	len_pifagor;
	float	len_p_p;

	len_p_p = powf(iw->walls[wall].x - iw->walls[wall].next->x, 2.0f) +
		powf(iw->walls[wall].y - iw->walls[wall].next->y, 2.0f);
	//len_point = sqrtf(powf(iw->p.x - iw->walls[wall].x, 2.0f) + powf(iw->p.y - iw->walls[wall].y, 2.0f));
	len_pifagor = powf(iw->p.x - iw->walls[wall].x, 2.0f) + powf(iw->p.y - iw->walls[wall].y, 2.0f) -
		powf(len, 2.0f);
	if (len_pifagor > len_p_p)
		return (0);
	len_pifagor = powf(iw->p.x - iw->walls[wall].next->x, 2.0f) + powf(iw->p.y - iw->walls[wall].next->y, 2.0f) -
		powf(len, 2.0f);
	if (len_pifagor > len_p_p)
		return (0);
	return (1);
}

void	check_walls_collisions(t_sdl *iw)
{
	int		wall;
	int		len;
	float	nx;
	float	ny;
	float	tmp;

	if (iw->d.cs < 0)
		return;
	wall = iw->sectors[iw->d.cs].sw - 1;
	while (++wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		if ((iw->walls[wall].nextsector == -1 || iw->walls[wall].glass != -1) && (iw->walls[wall].x != iw->walls[wall].next->x || 
			iw->walls[wall].y != iw->walls[wall].next->y))
		{
			len = (int)(fabsf(iw->walls[wall].l.a * iw->p.x + iw->walls[wall].l.b * iw->p.y + iw->walls[wall].l.c) /
				sqrtf(powf(iw->walls[wall].l.a, 2.0f) + powf(iw->walls[wall].l.b, 2.0f)));
			if (len < COLLISION_SIZE2)
			{
				if (!check_walls_collisions_on_line_segment(iw, wall, len))
					continue;
				nx = iw->walls[wall].l.a;
				ny = iw->walls[wall].l.b;
				if ((iw->walls[wall].next->x > iw->walls[wall].x && ny > 0.0f) ||
					(iw->walls[wall].next->x < iw->walls[wall].x && ny < 0.0f))
				{
					nx = -nx;
					ny = -ny;
				}
				tmp = sqrtf(powf(nx, 2.0f) + powf(ny, 2.0f));
				nx *= (float)(COLLISION_SIZE2 - len) / tmp;
				ny *= (float)(COLLISION_SIZE2 - len) / tmp;
				if (in_sec_xy(iw, iw->d.cs, iw->p.x + (int)nx, iw->p.y + (int)ny))
				{
					iw->p.x += (int)nx;
					iw->p.y += (int)ny;
				}
			}
		}
	}
}

// ENEMIES FUNCTIONS /////////////////////////////////////////////////////////////

int		esp_check_walls(t_sdl *iw, t_enemy_sees_player *esp)
{
	int		wall;

	wall = iw->sectors[esp->curr_sector].sw - 1;
	while (++wall < iw->sectors[esp->curr_sector].sw + iw->sectors[esp->curr_sector].nw)
	{
		if (iw->walls[wall].nextsector != -1)
			continue;
		esp->k1 = iw->walls[wall].l.a * (float)esp->px + iw->walls[wall].l.b * (float)esp->py + iw->walls[wall].l.c;
		esp->k2 = iw->walls[wall].l.a * (float)esp->ex + iw->walls[wall].l.b * (float)esp->ey + iw->walls[wall].l.c;
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
		{
			esp->k1 = esp->a * (float)iw->walls[wall].x + esp->b * (float)iw->walls[wall].y + esp->c;
			esp->k2 = esp->a * (float)iw->walls[wall].next->x + esp->b * (float)iw->walls[wall].next->y + esp->c;
			if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
				return (0);
		}
	}
	return (1);
}

int		esp_check_portal(t_sdl *iw, t_enemy_sees_player *esp, int portal)
{
	if (iw->walls[portal].glass >= 0 || iw->walls[iw->walls[portal].nextsector_wall].glass >= 0)
		return (0);
	esp->k1 = iw->walls[portal].l.a * (float)esp->px + iw->walls[portal].l.b * (float)esp->py + iw->walls[portal].l.c;
	esp->k2 = iw->walls[portal].l.a * (float)esp->ex + iw->walls[portal].l.b * (float)esp->ey + iw->walls[portal].l.c;
	if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
	{
		esp->k1 = esp->a * (float)iw->walls[portal].x + esp->b * (float)iw->walls[portal].y + esp->c;
		esp->k2 = esp->a * (float)iw->walls[portal].next->x + esp->b * (float)iw->walls[portal].next->y + esp->c;
		if ((esp->k1 > 0.0f && esp->k2 < 0.0f) || (esp->k1 < 0.0f && esp->k2 > 0.0f))
			return (1);
	}
	return (0);
}

void	esp_get_new_player_coordinates(t_sdl *iw, t_sector_way *way, t_enemy_sees_player *esp, t_sprite *s)
{
	esp->px = iw->p.x;
	esp->py = iw->p.y;
	esp->ex = s->x;
	esp->ey = s->y;
	esp->curr_sector = s->num_sec;
	while (way)
	{
		esp->px -= iw->walls[iw->walls[way->portal].nextsector_wall].x - iw->walls[way->portal].next->x;
		esp->py -= iw->walls[iw->walls[way->portal].nextsector_wall].y - iw->walls[way->portal].next->y;
		way = way->next;
	}
}

int		enemy_sees_player(t_sdl *iw, t_sprite *s)
{
	t_enemy_sees_player	esp;
	t_sector_ways		*ways;
	t_sector_way		*way;

	esp.px = iw->p.x;
	esp.py = iw->p.y;
	esp.ex = s->x;
	esp.ey = s->y;
	esp.curr_sector = s->num_sec;
	if (iw->d.cs == s->num_sec)
	{
		esp.a = (float)(esp.py - esp.ey);
		esp.b = (float)(esp.ex - esp.px);
		esp.c = (float)(esp.px * esp.ey - esp.ex * esp.py);
		if (!esp_check_walls(iw, &esp))
			return (-1);
		s->e.vis_esp = esp;
		return ((int)sqrtf(powf(esp.px - esp.ex, 2.0f) + powf(esp.py - esp.ey, 2.0f)));
	}
	// if (!iw->ways[s->num_sec][iw->d.cs])
	// 	return (-1);
	ways = iw->ways[s->num_sec][iw->d.cs];
	while (ways)
	{
		way = ways->way_start;
		esp_get_new_player_coordinates(iw, way, &esp, s);
		while (way)
		{
			esp.a = (float)(esp.py - esp.ey);
			esp.b = (float)(esp.ex - esp.px);
			esp.c = (float)(esp.px * esp.ey - esp.ex * esp.py);
			if (!esp_check_walls(iw, &esp) || !esp_check_portal(iw, &esp, way->portal))
				break;
			esp.px += iw->walls[iw->walls[way->portal].nextsector_wall].x - iw->walls[way->portal].next->x;
			esp.py += iw->walls[iw->walls[way->portal].nextsector_wall].y - iw->walls[way->portal].next->y;
			esp.ex += iw->walls[iw->walls[way->portal].nextsector_wall].x - iw->walls[way->portal].next->x;
			esp.ey += iw->walls[iw->walls[way->portal].nextsector_wall].y - iw->walls[way->portal].next->y;
			esp.curr_sector = iw->walls[way->portal].nextsector;
			way = way->next;
		}
		if (!way)
		{
			esp.a = (float)(esp.py - esp.ey);
			esp.b = (float)(esp.ex - esp.px);
			esp.c = (float)(esp.px * esp.ey - esp.ex * esp.py);
			if (esp_check_walls(iw, &esp))
			{
				s->e.vis_esp = esp;
				return ((int)sqrtf(powf(esp.px - esp.ex, 2.0f) + powf(esp.py - esp.ey, 2.0f)));
			}
		}
		ways = ways->next;
	}
	return (-1);
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
		k1 = iw->walls[wall].l.a * s->x + iw->walls[wall].l.b * s->y + iw->walls[wall].l.c;
		k2 = iw->walls[wall].l.a * (s->x + vect->x) + iw->walls[wall].l.b * (s->y + vect->y) + iw->walls[wall].l.c;
		if ((k1 < 0.0f && k2 > 0.0f) || (k1 > 0.0f && k2 < 0.0f))
		{
			s->e.vis_esp.a = (float)vect->y;
			s->e.vis_esp.b = (float)(-vect->x);
			s->e.vis_esp.c = (float)((s->x + vect->x) * s->y - s->x * (s->y + vect->y));
			k1 = s->e.vis_esp.a * iw->walls[wall].x + s->e.vis_esp.b * iw->walls[wall].y + s->e.vis_esp.c;
			k2 = s->e.vis_esp.a * iw->walls[wall].next->x + s->e.vis_esp.b * iw->walls[wall].next->y + s->e.vis_esp.c;
			if ((k1 < 0.0f && k2 > 0.0f) || (k1 > 0.0f && k2 < 0.0f))
			{
				s->x += vect->x + iw->walls[iw->walls[wall].nextsector_wall].x - iw->walls[wall].next->x;
				s->y += vect->y + iw->walls[iw->walls[wall].nextsector_wall].y - iw->walls[wall].next->y;
				s->num_sec = iw->walls[wall].nextsector;
				s->e.vis_esp.ex += iw->walls[iw->walls[wall].nextsector_wall].x - iw->walls[wall].next->x;
				s->e.vis_esp.ey += iw->walls[iw->walls[wall].nextsector_wall].y - iw->walls[wall].next->y;
				s->e.vis_esp.px += iw->walls[iw->walls[wall].nextsector_wall].x - iw->walls[wall].next->x;
				s->e.vis_esp.py += iw->walls[iw->walls[wall].nextsector_wall].y - iw->walls[wall].next->y;
				return (1);
			}
		}
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
	vect.x = vect_norm.x * (float)(t - s->e.prev_update_time) / (float)CLKS_P_S * 1000.0f;
	vect.y = vect_norm.y * (float)(t - s->e.prev_update_time) / (float)CLKS_P_S * 1000.0f;
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

void	enemy_intelligence0(t_sdl *iw, t_sprite *s)
{
	int		i;

	if (s->e.status == 0)
	{
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
			i = (int)sqrtf(powf(s->x - s->e.vis_esp.px, 2.0f) + powf(s->y - s->e.vis_esp.py, 2.0f));
			if (i > 10)
				move_enemy(iw, s);
		}
	}
	else if (s->e.status == 3 && clock() - s->e.previous_picture_change > CLKS_P_S / 5)
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
	else if (s->e.status == 4 && clock() - s->e.previous_picture_change > CLKS_P_S / 5)
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
	{
		if (s->e.health < 10 || ((i = enemy_sees_player(iw, s)) != -1 && i < 1000))
		{
			s->e.status = 0;
			s->t_numb = 0;
			s->t = iw->t_enemies[s->t_numb];
			s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
		}
	}
	sprite_physics(iw, s);
	if (s->e.health < 0 && s->e.status < 4)
	{
		Mix_PlayChannel(5, iw->sounds.env[20], 0);
		s->e.status = 4;
	}
}

void	enemy_intelligence1(t_sdl *iw, t_sprite *s)
{
	int		i;

	//printf("STATUS %d\n", s->e.status);
	if (s->e.status == 0)
	{
		if ((i = enemy_sees_player(iw, s)) != -1 && (i < 4000 || s->e.health < ENEMY_HEALTH1))
			s->e.status = 2;
	}
	else if (s->e.status == 2)
	{
		if (clock() - s->e.previous_picture_change > CLKS_P_S / 3)
		{
			s->e.previous_picture_change = clock();
			if (s->t_numb <= 8 || s->t_numb >= 12)
				s->t_numb = 10;
			else
				s->t_numb++;
			s->t = iw->t_enemies[s->t_numb];
			s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
		}
		if ((i = enemy_sees_player(iw, s)) != -1)
		{
			if (i > 2000)
				move_enemy(iw, s);
			else
				s->e.status = 3;
		}
		else if (s->e.vis_esp.curr_sector != -1)
		{
			i = (int)sqrtf(powf(s->x - s->e.vis_esp.px, 2.0f) + powf(s->y - s->e.vis_esp.py, 2.0f));
			if (i > 10)
				move_enemy(iw, s);
			else
			{
				s->t_numb = 8;
				s->e.status = 0;
			}
			
		}
	}
	else if (s->e.status == 3 && clock() - s->e.previous_picture_change > CLKS_P_S / 2)
	{
		s->e.previous_picture_change = clock();
		if (s->t_numb < 13)
		{
			s->t_numb = 13;
			iw->p.health -= ENEMY_DAMAGE1 * iw->menu.count;
			if (!Mix_Playing(6))
				Mix_PlayChannel(6, iw->sounds.env[14], 0);
			if (!Mix_Playing(4))
				Mix_PlayChannel(4, iw->sounds.env[10], 0);
		}
		else if (s->t_numb == 13)
			s->t_numb = 14;
		else
		{
			s->e.status = 0;
			s->t_numb = 8;
		}
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}
	else if (s->e.status == 4 && clock() - s->e.previous_picture_change > CLKS_P_S / 4)
	{
		s->e.previous_picture_change = clock();
		if (s->t_numb < 15)
			s->t_numb = 15;
		else if (s->t_numb < 19)
			s->t_numb++;
		else
			s->e.status = 5;
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}
	else if (s->e.status == 1)
		if ((i = enemy_sees_player(iw, s)) != -1 && (i < 1000 || s->e.health < ENEMY_HEALTH1))
			s->e.status = 0;
	if (s->e.health <= 0 && s->e.status < 4)
	{
		Mix_PlayChannel(6, iw->sounds.env[21], 0);
		s->e.status = 4;
	}
	sprite_physics(iw, s);
}

void	enemy_intelligence2(t_sdl *iw, t_sprite *s)
{
	int	i;

	i = 0;
	if (s->e.status == 0)
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
	else if (s->e.status == 1)
	{
		if ((s->e.health < ENEMY_HEALTH2) || ((i = enemy_sees_player(iw, s)) != -1 && i <= 2000))
		{	
			s->e.status = 0;
			s->t_numb = 20;
		}
	}
	else if (s->e.status == 2)
	{

		if (clock() - s->e.previous_picture_change > CLKS_P_S / 7)
		{
			s->e.previous_picture_change = clock();
			if (s->t_numb == 20 || s->t_numb == 25)
				s->t_numb = 22;
			else if (s->t_numb == 22)
				s->t_numb = 23;
			else if (s->t_numb == 23)
				s->t_numb = 24;
			else if (s->t_numb == 24)
				s->t_numb = 22;
			s->t = iw->t_enemies[s->t_numb];
			s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
		}
		if ((i = enemy_sees_player(iw, s)) != -1)
		{		
			if (i > 4000)
				move_enemy(iw, s);
			else
				s->e.status = 3;
		}
		else if (s->e.vis_esp.curr_sector != -1)
		{
			i = (int)sqrtf(powf(s->x - s->e.vis_esp.px, 2.0f) + powf(s->y - s->e.vis_esp.py, 2.0f));
			if (i > 10)
				move_enemy(iw, s);
			else
			{
				s->t_numb = 20;
				s->e.status = 0;
			}
			
		}
	}
	else if (s->e.status == 3 && clock() - s->e.previous_picture_change > CLKS_P_S / 5)
	{
	 	s->e.previous_picture_change = clock();
		if (s->t_numb != 25)
		{
			s->t_numb = 25;
			iw->p.health -= ENEMY_DAMAGE2 * iw->menu.count;
			if (!Mix_Playing(4))
				Mix_PlayChannel(4, iw->sounds.env[9], 0);
			if (!Mix_Playing(7))
				Mix_PlayChannel(7, iw->sounds.env[11], 0);
		}
		else if (s->t_numb == 25)
			s->t_numb = 20;
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
		if ((i = enemy_sees_player(iw, s)) == -1)
			s->e.status = 2;
	}
	else if (s->e.status == 4 && clock() - s->e.previous_picture_change > CLKS_P_S / 7)
	{
		s->e.previous_picture_change = clock();
		if (s->t_numb < 26)
			s->t_numb = 26;
		else if (s->t_numb == 26)
			s->t_numb = 27;
		else if (s->t_numb == 27)
			s->t_numb = 28;
		else if (s->t_numb == 28)
			s->t_numb = 29;
		else if (s->t_numb == 29)
			s->e.status = 5;
		s->t = iw->t_enemies[s->t_numb];
		s->t_kernel = &iw->k.m_t_enemies[s->t_numb];
	}

	if (s->e.health <= 0 && s->e.status < 4)
	{
		Mix_PlayChannel(7, iw->sounds.env[19], 0);
		s->e.status = 4;
	}
	sprite_physics(iw, s);
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
				enemy_intelligence2(iw, tmp);
			tmp->e.prev_update_time = clock();
		}
		tmp = tmp->next;
	}
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	draw_miss(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.w = WINDOW_W / 10;
	rect.h = rect.w * iw->hud.miss->h / iw->hud.miss->w;
	rect.x = WINDOW_W / 2 - rect.w / 2;
	rect.y = WINDOW_H / 3 - WINDOW_H / 10 * (clock() - iw->hud.miss_time) / CLKS_P_S;
	ft_scaled_blit(iw->hud.miss, iw->sur, &rect);
}

void	draw_save(t_sdl *iw)
{
	SDL_Rect	rect;

	rect.w = WINDOW_W / 4;
	rect.h = rect.w * iw->hud.saved->h / iw->hud.saved->w;
	rect.x = WINDOW_W / 2 - rect.w / 2;
	rect.y = 20 + WINDOW_H / 20 * (clock() - iw->hud.saved_time) / CLKS_P_S;
	ft_scaled_blit(iw->hud.saved, iw->sur, &rect);
}

void	damaging_enemy(t_sdl *iw, int damage, int max_distance)
{
	if (iw->v.look_portal != 0 && iw->v.look_portal->glass != -1
		&& iw->guns.gun_in_hands != 0)
		iw->v.look_portal->glass = -1;
	if (iw->v.look_sprite == 0 || iw->v.look_sprite->type != 2 || iw->v.look_sprite->e.status >= 4)
		return;
	/*if ((int)sqrtf(powf(iw->p.x - iw->v.look_sprite->x, 2.0f) +
			powf(iw->p.y - iw->v.look_sprite->y, 2.0f) +
			powf(iw->p.z - iw->v.look_sprite->z, 2.0f)) < max_distance)*/
	if (iw->v.look_sprite->plen < max_distance)
		iw->v.look_sprite->e.health -= damage;
	else
		iw->hud.miss_time = clock();
}

void	attack(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0 && iw->guns.status == 0 &&
		clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		damaging_enemy(iw, 3, 1000);
		iw->guns.status = 1;
		iw->guns.t = 18;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[0] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[0] / iw->guns.max_bullets[0];
	}
	else if (iw->guns.gun_in_hands == 1 && iw->guns.status == 0 && clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		damaging_enemy(iw, 3, 5000);
		Mix_PlayChannel(3, iw->sounds.env[6], 0);
		iw->guns.status = 1;
		iw->guns.t = 1;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[1] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[1] / iw->guns.max_bullets[1];
	}
	else if (iw->guns.gun_in_hands == 2 && iw->guns.status == 0 && clock() - iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 1;
		iw->guns.t = 8;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[2] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[2] / iw->guns.max_bullets[2];
	}
}

void	reload_gun(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0)
	{
		iw->guns.bullets[iw->guns.gun_in_hands] = iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
	}
	else if (iw->guns.gun_in_hands == 1)
	{
		if (iw->guns.bullets_in_stock[1] < iw->guns.max_bullets[1])
		{
			iw->guns.gun_in_hands = 0;
			iw->guns.status = 3;
		}
		else if (iw->guns.t == 0)
		{
			Mix_PlayChannel(3, iw->sounds.env[7], 0);
			iw->guns.t = 2;
		}
		else if (iw->guns.t < 6)
			iw->guns.t++;
		else
		{
			iw->guns.bullets_in_stock[1] -= iw->guns.max_bullets[1] - iw->guns.bullets[1];;
			iw->guns.bullets[iw->guns.gun_in_hands] = iw->guns.max_bullets[iw->guns.gun_in_hands];
			iw->hud.shell = 100;
			iw->guns.status = 0;
			iw->guns.t = 0;
			iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
		}
	}
	else if (iw->guns.gun_in_hands == 2)
	{
		if (iw->guns.bullets_in_stock[2] < iw->guns.max_bullets[2])
		{
			iw->guns.gun_in_hands = 0;
			iw->guns.status = 3;
		}
		else if (iw->guns.t == 7 || iw->guns.t == 8 || iw->guns.t == 9)
		{
			Mix_PlayChannel(3, iw->sounds.env[8], 0);
			iw->guns.t = 10;
		}
		else if (iw->guns.t < 16)
			iw->guns.t++;
		else
		{
			iw->guns.bullets_in_stock[2] -= iw->guns.max_bullets[2] - iw->guns.bullets[2];
			iw->guns.bullets[iw->guns.gun_in_hands] = iw->guns.max_bullets[iw->guns.gun_in_hands];
			iw->hud.shell = 100;
			iw->guns.status = 0;
			iw->guns.t = 7;
			iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
		}
	}
	iw->guns.prev_update_time = clock();
}

void	guns_loop(t_sdl *iw)
{
	if (iw->guns.status == 1 && iw->guns.gun_in_hands == 0 && clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		iw->guns.status = 0;
		iw->guns.t = 17;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 1 && clock() - iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 0;
		iw->guns.t = 0;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 2 && clock() - iw->guns.prev_update_time > CLKS_P_S / 10)
	{
		if (iw->guns.t == 8)
		{
			damaging_enemy(iw, 2, 10000);
			Mix_PlayChannel(3, iw->sounds.env[5], 0);
			iw->guns.t = 9;
		}
		else
		{
			if (!iw->v.left_mouse_pressed ||
				iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			{
				iw->guns.status = 0;
				iw->guns.t = 7;
			}
			else
			{
				iw->guns.t = 8;
				iw->guns.bullets[iw->guns.gun_in_hands]--;
				iw->hud.shell = 100 * iw->guns.bullets[iw->guns.gun_in_hands] /
					iw->guns.max_bullets[iw->guns.gun_in_hands];
			}
			if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
				iw->guns.status = 2;
		}
		iw->guns.prev_update_time = clock();
	}
}

void	guns_movements(t_sdl *iw)
{
	int		t;

	if (iw->v.game_mode &&
		(iw->v.front != 1 || iw->v.back != 1 || iw->v.left != 1 || iw->v.right != 1))
	{
		//printf("FPS %d\n", iw->v.fps);
		if (iw->v.fps != 0)
			t = 150 / iw->v.fps;
		else
			t = 10;
		iw->v.weapon_change_x += iw->v.sprint * ((iw->v.weapon_change_xdir > 0) ? t : -t);
		if (abs(iw->v.weapon_change_x) > WEAPONS_MOVING_CHANGE_VALUE)
		{
			iw->v.weapon_change_xdir *= -1;
			iw->v.weapon_change_x = ((iw->v.weapon_change_x > 0) ? WEAPONS_MOVING_CHANGE_VALUE : -WEAPONS_MOVING_CHANGE_VALUE);
		}
		iw->v.weapon_change_y += iw->v.sprint * ((iw->v.weapon_change_ydir > 0) ? t : -t);
		if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE || iw->v.weapon_change_y < 10)
			iw->v.weapon_change_ydir *= -1;
		if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE)
			iw->v.weapon_change_y = WEAPONS_MOVING_CHANGE_VALUE;
		if (iw->v.weapon_change_y < 10)
			iw->v.weapon_change_y = 10;
	}
	if (iw->guns.status == 3)
	{
		iw->v.weapon_change_y_hide += 500 / iw->v.fps;
		if (iw->v.weapon_change_y_hide >= iw->guns.t_rect[iw->guns.t].h)
		{
			if (iw->guns.gun_in_hands == 0)
				iw->guns.t = 17;
			else if (iw->guns.gun_in_hands == 1)
				iw->guns.t = 0;
			else if (iw->guns.gun_in_hands == 2)
				iw->guns.t = 7;
			iw->guns.status = 0;
		}
	}
	else if (iw->v.weapon_change_y_hide > 0)
	{
		iw->v.weapon_change_y_hide -= 1000 / iw->v.fps;
		if (iw->v.weapon_change_y_hide < 0)
			iw->v.weapon_change_y_hide = 0;
	}
}

void	death(t_sdl *iw)
{
	if (iw->v.game_mode && (iw->d.cs < 0 || iw->p.health < 1))
	{
		image_loop(iw, iw->hud.dead);
		get_birth_def(iw);
	}
}

void	set_checkpoint(t_sdl *iw, t_sprite *s)
{
	t_keyb_inp	*tmp;

	Mix_PlayChannel(2, iw->sounds.env[15], 0);
	tmp = iw->checkpoints;
	while (tmp)
	{
		if (tmp->sprite == s)
			break;
		tmp = tmp->next;
	}
	if (!tmp)
		return;
	iw->l.start_x = s->x;
	iw->l.start_y = s->y;
	iw->v.last_checkpoint = s;
	iw->v.last_to_write = tmp;
	iw->hud.saved_time = clock();
}

void	check_checkpoints(t_sdl *iw)
{
	t_sprite	*tmp;

	tmp = *iw->sprite;
	while (tmp)
	{
		if (tmp->type == 0 && tmp->t_numb == 1 &&
			tmp->draweble && tmp->plen < CHECKPOINT_DIST)
		{
			if (tmp != iw->v.last_checkpoint)
				set_checkpoint(iw, tmp);
			return;
		}
		tmp = tmp->next;
	}
}

void	sound_loop(t_sdl *iw)
{
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(iw->sounds.music[iw->v.next_music], 0);
		iw->v.next_music++;
		if (iw->v.next_music >= MUSIC_COUNT)
			iw->v.next_music = 0;
	}
}

void	environment_loop(t_sdl *iw)
{
	// steps
	if (iw->v.fly_mode == 0 && iw->v.fall == 1 &&
		(iw->v.front != 1 || iw->v.back != 1 || iw->v.left != 1 || iw->v.right != 1))
	{
		if (!Mix_Playing(0))
			Mix_PlayChannel(0, iw->sounds.env[0], -1);
	}
	else if (iw->v.fly_mode == 0 && Mix_Playing(0))
		Mix_HaltChannel(0);
	if (iw->v.fly_mode && !Mix_Playing(0))
		Mix_PlayChannel(0, iw->sounds.env[18], -1);
}

void	loop(t_sdl *iw)
{
	int		t;
	float	jsz;

	if ((double)(clock() - iw->loop_update_time) < (double)CLKS_P_S / (double)MAX_FPS)
		return;
	sound_loop(iw);
	environment_loop(iw);
	if (iw->v.left_mouse_pressed && iw->v.mouse_mode == 1)
		attack(iw);
	if (iw->guns.status != 0)
		guns_loop(iw);
	if (iw->guns.status == 2 && clock() - iw->guns.prev_update_time > CLKS_P_S / 8)
		reload_gun(iw);
	guns_movements(iw);
	if (iw->v.game_mode)
		check_checkpoints(iw);
	//////////////////////////checkpoints
	if (iw->v.jetpack != 1 && clock() - iw->v.jetpack > JETPACK_TIME)
	{
		iw->v.jetpack = 1;
		iw->v.fly_mode = 0;
		iw->v.fall = 1;
		iw->v.fly_down = 1;
		iw->v.fly_up = 1;
	}
	if (iw->v.rot_right != 1)
	{
		iw->p.rot += (ROTATION_SPEED_PER_HALF_SEC * (double)(clock() - iw->v.rot_right)
			/ (float)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot >= G360)
			iw->p.rot -= G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_right = clock();
	}
	if (iw->v.rot_left != 1)
	{
		iw->p.rot -= (ROTATION_SPEED_PER_HALF_SEC * (double)(clock() - iw->v.rot_left)
			/ (float)CLKS_P_S * 2.0f) * G1;
		while (iw->p.rot < 0.0f)
			iw->p.rot += G360;
		iw->p.introt = (int)(iw->p.rot / G1);
		iw->v.rot_left = clock();
	}
	if (iw->v.rot_up != 1 && iw->p.rotup < 2 * WINDOW_H)
	{
		iw->p.rotup += 2 * WINDOW_H * (clock() - iw->v.rot_up) / CLKS_P_S;
		iw->v.rot_up = clock();
	}
	if (iw->v.rot_down != 1 && iw->p.rotup > -2 * WINDOW_H)
	{
		iw->p.rotup -= 2 * WINDOW_H * (clock() - iw->v.rot_down) / CLKS_P_S;
		iw->v.rot_down = clock();
	}
	//do_animations(iw);
	death(iw);
	if (iw->v.game_mode && iw->d.cs == iw->l.win_sector)
	{
		image_loop(iw, iw->hud.win);
		exit_x(iw);
	}
	if (iw->d.cs >= 0)
	{
		iw->v.plrzu = get_ceil_z(iw, iw->p.x, iw->p.y);
		iw->v.plrzd = get_floor_z(iw, iw->p.x, iw->p.y);
		if (iw->v.crouch != 0 && iw->v.crouch_pressed == 0
			&& iw->v.plrzu - iw->v.plrzd >= PLAYER_HEIGHT + PLAYER_HEAD_SIZE)
			iw->v.crouch = 0;
		if (iw->v.front != 1)
		{
			move(iw, 0, &iw->v.front);
			//iw->v.front = clock();
		}
		if (iw->v.back != 1)
		{
			move(iw, 180, &iw->v.back);
			//iw->v.back = clock();
		}
		if (iw->v.left != 1)
		{
			move(iw, 270, &iw->v.left);
			//iw->v.left = clock();
		}
		if (iw->v.right != 1)
		{
			move(iw, 90, &iw->v.right);
			//iw->v.right = clock();
		}
		if (iw->v.fall != 1 && !iw->v.fly_mode)
		{
			t = clock();
			iw->p.z -= (int)(iw->l.accel * ((double)(t - iw->v.fall) /
				(double)CLKS_P_S) * 50.0f);
		}
		else if (iw->v.jump_time != 1)
		{
			jsz = (float)(clock() - iw->v.jump_time) / (float)CLKS_P_S * (float)JUMP_HEIGHT *
				iw->l.accel / 10.0f;
			if ((int)jsz >= iw->v.jump)
			{
				iw->p.z += iw->v.jump;
				iw->v.jump_time = 1;
			}
			else
			{
				iw->p.z += (int)jsz;
				iw->v.jump -= (int)jsz;
			}

		}
		if (iw->v.fall == 1 && iw->v.jump_time == 1
			&& (iw->p.z - iw->v.plrzd) > PLAYER_HEIGHT && !iw->v.fly_mode)
			iw->v.fall = clock();
		if (iw->v.fly_up != 1)
		{
			iw->p.z += (int)(FLY_SPEED * (float)(clock() - iw->v.fly_up) / (float)CLKS_P_S);
			iw->v.fly_up = clock();
		}
		if (iw->v.fly_down != 1)
		{
			iw->p.z -= (int)(FLY_SPEED * (float)(clock() - iw->v.fly_down) / (float)CLKS_P_S);
			iw->v.fly_down = clock();
		}
		if (iw->sectors[iw->d.cs].cl.t >= 0 && iw->p.z + PLAYER_HEAD_SIZE - iw->v.crouch > iw->v.plrzu && iw->v.fly_mode != 2)
			iw->p.z = iw->v.plrzu - PLAYER_HEAD_SIZE + iw->v.crouch;
		else if (iw->p.z - iw->v.plrzd < PLAYER_HEIGHT && iw->v.fly_mode != 2)
		{
			iw->p.z = iw->v.plrzd + PLAYER_HEIGHT;
			iw->v.fall = 1;
		}
	}
	else if (iw->v.fly_mode == 2)
	{
		if (iw->v.front != 1)
			move(iw, 0, &iw->v.front);
		if (iw->v.back != 1)
			move(iw, 180, &iw->v.back);
		if (iw->v.left != 1)
			move(iw, 270, &iw->v.left);
		if (iw->v.right != 1)
			move(iw, 90, &iw->v.right);
	}
	else
		iw->v.fall = 1;
	do_sector_animations(iw);
	do_wall_animations(iw);
	if (iw->v.fly_mode != 2)
		check_walls_collisions(iw);
	if (iw->v.game_mode)
		check_enemies(iw);
	update(iw);
	iw->v.fps = (int)((float)CLKS_P_S / (float)(clock() - iw->loop_update_time));
	iw->loop_update_time = clock();
}

void	main_loop(t_sdl *iw)
{
	SDL_Event e;

	iw->quit = 0;
	while (!iw->quit)
	{
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT)
				iw->quit = 1;
			else if (e.type == SDL_KEYDOWN)
			{
				key_down_repeat(e.key.keysym.scancode, iw);
				if (e.key.repeat == 0)
					key_down(e.key.keysym.scancode, iw);
			}
			else if (e.type == SDL_KEYUP)
				key_up(e.key.keysym.scancode, iw);
			else if (e.type == SDL_MOUSEMOTION)
			{
				mouse_move(e.motion.xrel, e.motion.yrel, iw);
				iw->v.mouse_x = e.motion.x;
				iw->v.mouse_y = e.motion.y;
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
			{
				mouse_buttonleft_up(e.button.x, e.button.y, iw);
				iw->bag.click_x = e.button.x;
        		iw->bag.click_y = e.button.y;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && iw->v.game_mode)
				iw->v.left_mouse_pressed = 1;
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
				mouse_buttonright_up(e.button.x, e.button.y, iw);
			else if (e.type == SDL_MOUSEWHEEL && e.wheel.y != 0)
				mouse_wheel(&e, iw);
		loop(iw);
	}
}

void	get_wall_line(t_sdl *iw, int wall)
{
	iw->walls[wall].l.a = (float)(iw->walls[wall].next->y - iw->walls[wall].y);
	iw->walls[wall].l.b = (float)(iw->walls[wall].x - iw->walls[wall].next->x);
	iw->walls[wall].l.c = (float)(iw->walls[wall].next->x * iw->walls[wall].y -
		iw->walls[wall].x * iw->walls[wall].next->y);
}

void	create_map(t_sdl *iw)
{
	int		i;
	int		j;

	i = -1;
	while (++i < iw->v.sc)
	{
		if (iw->sectors[i].fr.n != 0)
			iw->sectors[i].fr.n->d = -iw->sectors[i].fr.n->a * iw->sectors[i].fr.x -
			iw->sectors[i].fr.n->b * iw->sectors[i].fr.y - iw->sectors[i].fr.n->c * iw->sectors[i].fr.z;
		if (iw->sectors[i].cl.n != 0)
			iw->sectors[i].cl.n->d = -iw->sectors[i].cl.n->a * iw->sectors[i].cl.x -
			iw->sectors[i].cl.n->b * iw->sectors[i].cl.y - iw->sectors[i].cl.n->c * iw->sectors[i].cl.z;
		j = iw->sectors[i].sw - 1;
		while (++j < iw->sectors[i].sw + iw->sectors[i].nw)
			get_wall_line(iw, j);
	}
}

int		get_sector(t_sdl *iw)
{
	int sec;

	sec = iw->v.ls - 1;
	while (++sec < iw->v.sc)
		if (in_sec(sec, iw) != 0)
			return (sec);
	sec = -1;
	while (++sec < iw->v.ls)
		if (in_sec(sec, iw) != 0)
			return (sec);
	return (-1);
}

void	set_top_bottom(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i <= WINDOW_W)
	{
		iw->d.top[i] = 0;
		iw->d.bottom[i] = WINDOW_H;
	}
}

float	get_k_angle(float rot)
{
	if (rot < G180)
		return (G180 - rot);
	else
		return (G360 - rot);
}

void	get_left_right_lines_points(t_sdl *iw)
{
	float	na;
	float	nk;

	na = iw->p.rot - iw->v.angle;
	if (na < 0.0f)
		na += G360;
	nk = get_k_angle(na);
	iw->d.left_line.a = tanf(nk);
	iw->d.left_line.b = -1.0f;
	iw->d.left_line.c = (float)iw->p.y - iw->d.left_line.a * (float)iw->p.x;
	if (na < 180.0f * G1)
		iw->d.left_point.y = (float)iw->p.y - 1.0f;
	else
		iw->d.left_point.y = (float)iw->p.y + 1.0f;
	iw->d.left_point.x = (iw->d.left_line.b * iw->d.left_point.y + iw->d.left_line.c) / (-iw->d.left_line.a);

	na = iw->p.rot + iw->v.angle;
	if (na > G360)
		na -= G360;
	nk = get_k_angle(na);
	iw->d.right_line.a = tanf(nk);
	iw->d.right_line.b = -1.0f;
	iw->d.right_line.c = (float)iw->p.y - iw->d.right_line.a * (float)iw->p.x;
	if (na < 180.0f * G1)
		iw->d.right_point.y = (float)iw->p.y - 1.0f;
	else
		iw->d.right_point.y = (float)iw->p.y + 1.0f;
	iw->d.right_point.x = (iw->d.right_line.b * iw->d.right_point.y + iw->d.right_line.c) / (-iw->d.right_line.a);
}

void	get_screen_line(t_sdl *iw)
{
	float	na;

	na = iw->p.rot + G90;
	if (na > G360)
		na -= G360;
	na = get_k_angle(na);
	iw->d.screen.a = tanf(na);
	iw->d.screen.b = -1.0f;
	iw->d.screen.c = (float)iw->p.y - iw->d.screen.a * (float)iw->p.x;
	iw->d.screen_len = sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
}

void	get_direction(t_sdl *iw)
{
	if (iw->p.rot > G90 && iw->p.rot < G90 * 3)
		iw->d.view_dir.x = -1;
	else
		iw->d.view_dir.x = 1;
	if (iw->p.rot < G180)
		iw->d.view_dir.y = -1;
	else
		iw->d.view_dir.y = 1;
}

void	free_walls(t_sdl *iw)
{
	t_save_wall	*tmp;
	t_save_wall	*tmp2;

	tmp = iw->d.vw;
	while (tmp != 0)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	iw->d.vw = 0;
}

void	free_pairs(t_sdl *iw)
{
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;

	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	iw->d.vwp = 0;
}

void	add_wall(t_sdl *iw, t_save_wall *tmp)
{
	t_save_wall	*tmp2;

	if (iw->d.vw == 0 || /*iw->d.vw->len > tmp->len*/iw->d.vw->x > tmp->x)
	{
		tmp->next = iw->d.vw;
		iw->d.vw = tmp;
	}
	else
	{
		tmp2 = iw->d.vw;
		while (tmp2->next != 0 && /*tmp2->next->len <= tmp->len*/tmp2->next->x < tmp->x)
			tmp2 = tmp2->next;
		if (tmp2->next != 0)
			tmp->next = tmp2->next;
		tmp2->next = tmp;
	}
}

int		get_floor_z(t_sdl *iw, int x, int y)
{
	if (iw->sectors[iw->d.cs].fr.n == 0)
		return (iw->sectors[iw->d.cs].fr.z);
	else
		return ((iw->sectors[iw->d.cs].fr.n->a * x + iw->sectors[iw->d.cs].fr.n->b *
			y + iw->sectors[iw->d.cs].fr.n->d) / (-iw->sectors[iw->d.cs].fr.n->c));
}

int		get_ceil_z(t_sdl *iw, int x, int y)
{
	if (iw->sectors[iw->d.cs].cl.n == 0)
		return (iw->sectors[iw->d.cs].cl.z);
	else
		return ((iw->sectors[iw->d.cs].cl.n->a * x + iw->sectors[iw->d.cs].cl.n->b *
			y + iw->sectors[iw->d.cs].cl.n->d) / (-iw->sectors[iw->d.cs].cl.n->c));
}

int		get_floor_z_sec(t_sdl *iw, int x, int y, int sector)
{
	if (iw->sectors[sector].fr.n == 0)
		return (iw->sectors[sector].fr.z);
	else
		return ((iw->sectors[sector].fr.n->a * x + iw->sectors[sector].fr.n->b *
			y + iw->sectors[sector].fr.n->d) / (-iw->sectors[sector].fr.n->c));
}

int		get_ceil_z_sec(t_sdl *iw, int x, int y, int sector)
{
	if (iw->sectors[sector].cl.n == 0)
		return (iw->sectors[sector].cl.z);
	else
		return ((iw->sectors[sector].cl.n->a * x + iw->sectors[sector].cl.n->b *
			y + iw->sectors[sector].cl.n->d) / (-iw->sectors[sector].cl.n->c));
}

float	get_vectors_angle(float x1, float y1, float x2, float y2)
{
	float	k;

	k = (x1 * x2 + y1 * y2) / (sqrtf(x1 * x1 + y1 * y1) * sqrtf(x2 * x2 + y2 * y2));
	if (k > 1.0f)
		return (acos(1.0f));
	else if (k < -1.0f)
		return (acos(-1.0f));
	return (acosf(k));
}

void	get_visible_walls2(t_sdl *iw, int wall, float lang)
{
	t_save_wall	*w;

	w = (t_save_wall *)malloc(sizeof(t_save_wall));
	w->x = (int)(lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	w->len = sqrtf(powf((float)(iw->p.x - iw->walls[wall].x), 2.0f) + powf((float)(iw->p.y - iw->walls[wall].y), 2.0f));
	w->plen = fabsf(iw->d.screen.a * (float)iw->walls[wall].x + iw->d.screen.b * (float)iw->walls[wall].y + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	if ((int)w->plen == 0)
		w->plen = 1.0f;
	w->olen = 0.0f;
	w->p.x = iw->walls[wall].x;
	w->p.y = iw->walls[wall].y;
	w->wall = &iw->walls[wall];
	w->zu = get_ceil_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->zd = get_floor_z(iw, iw->walls[wall].x, iw->walls[wall].y);
	w->next = 0;
	add_wall(iw, w);
}

void	get_visible_walls(t_sdl *iw)
{
	int		wall;
	float	lang;
	float	rang;

	wall = iw->sectors[iw->d.cs].sw;
	while (wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
	{
		lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
			(float)(iw->walls[wall].x - iw->p.x), (float)(iw->walls[wall].y - iw->p.y));
		rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
			(float)(iw->walls[wall].x - iw->p.x), (float)(iw->walls[wall].y - iw->p.y));
		if ((lang <= 2 * iw->v.angle && rang <= 2 * iw->v.angle) || lang == 0.0f || rang == 0.0f)
			get_visible_walls2(iw, wall, lang);
		wall++;
	}
}

int		cross_two_lines(t_line2d *l1, t_line2d *l2, t_intpoint2d *p)
{
	t_line2d	*tmp;
	float		py;

	if (l1->a == l2->a && l1->b == l2->b)
		return (0);
	if (l1->a == 0 && l2->a != 0)
	{
		tmp = l1;
		l1 = l2;
		l2 = tmp;
	}
	else if (l1->a == 0)
		return (0);
	py = (l2->a * l1->c - l1->a * l2->c) / (l1->a * l2->b - l2->a * l1->b);
	p->y = (int)roundf(py);
	if (l2->b == 0 && l2->a != 0)
		p->x = (int)roundf((l2->b * py + l2->c) / (-l2->a));
	else
		p->x = (int)roundf((l1->b * py + l1->c) / (-l1->a));
	return (1);
}

int		ft_min(int p1, int p2)
{
	if (p1 < p2)
		return (p1);
	return (p2);
}

int		ft_max(int p1, int p2)
{
	if (p1 >= p2)
		return (p1);
	return (p2);
}

int		point_in_front_and_on_wall(t_sdl *iw, t_intpoint2d *p, int wall)
{
	float	side;

	//(((d->rx - iw->p.x) * d->xdir) >= 0) && (((d->ry - iw->p.y) * d->ydir) >= 0)
	side = iw->d.screen.a * p->x + iw->d.screen.b * p->y + iw->d.screen.c;
	//if ((iw->d.view_dir.x < 0 && side < 0) || (iw->d.view_dir.x > 0 && side > 0))
	//if (((int)p->x - iw->p.x) ^ iw->d.view_dir.x >= 0 && ((int)p->y - iw->p.y) ^ iw->d.view_dir.y >= 0)
	if (side * iw->d.view_dir.y < 0)
		if ((p->x >= ft_min(iw->walls[wall].x, iw->walls[wall].next->x) && p->x <= ft_max(iw->walls[wall].x, iw->walls[wall].next->x))
			&& (p->y >= ft_min(iw->walls[wall].y, iw->walls[wall].next->y) && p->y <= ft_max(iw->walls[wall].y, iw->walls[wall].next->y)))
			return (1);
	return (0);
}

int		visible_wall(t_sdl *iw, int wall)
{
	if ((iw->p.x - iw->walls[wall].x) * (iw->walls[wall].next->y - iw->walls[wall].y) -
		(iw->p.y - iw->walls[wall].y) * (iw->walls[wall].next->x - iw->walls[wall].x) >= 0)
		return (1);
	return (0);
}

int		if_not_in_vw(t_sdl *iw, t_wall *wall)
{
	t_save_wall	*tmp;

	tmp = iw->d.vw;
	while (tmp != 0)
	{
		if (tmp->wall == wall)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	add_lr_wall(t_sdl *iw, t_intpoint2d *p, t_wall *wall, int x)
{
	t_save_wall	*tmp;

	tmp = (t_save_wall *)malloc(sizeof(t_save_wall));
	tmp->x = x;
	tmp->wall = wall;
	tmp->len = sqrtf(powf((float)(iw->p.x - p->x), 2.0f) + powf((float)(iw->p.y - p->y), 2.0f));
	tmp->plen = fabsf(iw->d.screen.a * (float)p->x + iw->d.screen.b * (float)p->y + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	if ((int)tmp->plen == 0)
		tmp->plen = 1.0f;
	tmp->olen = sqrtf(powf(p->x - wall->x, 2.0f) + powf(p->y - wall->y, 2.0f));
	tmp->p = *p;
	tmp->zd = get_floor_z(iw, p->x, p->y);
	tmp->zu = get_ceil_z(iw, p->x, p->y);
	tmp->next = 0;
	//printf("Adding x %d px %d py %d len %f plen %f\n", tmp->x, p->x, p->y, tmp->len, tmp->plen);
	add_wall(iw, tmp);
}

void	get_all_intersection_line(t_sdl *iw, t_line2d *nl, int right)
{
	int		wall;
	t_intpoint2d	p;

	wall = iw->sectors[iw->d.cs].sw - 1;
	while (++wall < iw->sectors[iw->d.cs].sw + iw->sectors[iw->d.cs].nw)
		if (if_not_in_vw(iw, ((right == 0) ? &iw->walls[wall] : iw->walls[wall].next))
			&& visible_wall(iw, wall) && cross_two_lines(nl, &iw->walls[wall].l, &p)
			&& point_in_front_and_on_wall(iw, &p, wall))
			add_lr_wall(iw, &p, ((right == 0) ? &iw->walls[wall] : iw->walls[wall].next), right * WINDOW_W);
}

void	get_left_right_visible_walls(t_sdl *iw)
{
	get_all_intersection_line(iw, &iw->d.left_line, 0);
	get_all_intersection_line(iw, &iw->d.right_line, 1);
}

t_save_wall	*find_next_vis_wall(t_sdl *iw, t_save_wall *left)
{
	t_save_wall	*right;

	right = iw->d.vw;
	while (right != 0)
	{
		if (right->wall == left->wall->next)
			break;
		right = right->next;
	}
	return (right);
}

void	put_wall_pixel(t_brez *b, int x, int y)
{
	if (x != b->prev_x)
	{
		*(b->wall_y++) = y;
		b->prev_x = x;
	}
}

void	print_brez(t_brez *b, int d, int d1, int d2)
{
	int i;

	if (b->k <= 0)
		b->dx = b->dy;
	i = 0;
	while (++i <= b->dx)
	{
		if (d > 0)
		{
			d += d2;
			b->y += b->sy;
		}
		else
			d += d1;
		if (b->k > 0)
			put_wall_pixel(b, b->x, b->y);
		else
			put_wall_pixel(b, b->y, b->x);
		b->x += b->sx;
	}
}

void	brez_line(int *wall_y, t_draw_line line)
{
	t_brez	b;

	*(wall_y++) = line.y0;
	b.prev_x = line.x0;
	b.wall_y = wall_y;
	b.sx = (line.x1 >= line.x0) ? (1) : (-1);
	b.sy = (line.y1 >= line.y0) ? (1) : (-1);
	b.dx = (line.x1 > line.x0) ? (line.x1 - line.x0) : (line.x0 - line.x1);
	b.dy = (line.y1 > line.y0) ? (line.y1 - line.y0) : (line.y0 - line.y1);
	if (abs(line.y1 - line.y0) < abs(line.x1 - line.x0))
	{
		b.x = line.x0 + b.sx;
		b.y = line.y0;
		b.k = 1;
		print_brez(&b, (b.dy * 2) - b.dx, b.dy * 2, (b.dy - b.dx) * 2);
	}
	else
	{
		b.x = line.y0 + b.sy;
		b.y = line.x0;
		b.sy = (line.x1 >= line.x0) ? (1) : (-1);
		b.sx = (line.y1 >= line.y0) ? (1) : (-1);
		b.k = 0;
		print_brez(&b, (b.dx * 2) - b.dy, b.dx * 2, (b.dx - b.dy) * 2);
	}
}

void	draw_between_sectors_bot_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int *tmp)
{
	int		i;
	int		j;
	t_draw_wall_tex	d;

	if (left->wall->t < 0)
		return;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) /
		(sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) *
			sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)(right->x - left->x);
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) /
		(sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) *
			sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;

	d.left_len = 0.0f;
	d.tx = left->olen * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
	while (d.tx >= (float)iw->t[left->wall->t]->w)
		d.tx -= (float)iw->t[left->wall->t]->w;

	j = -1;
	while (++j < right->x - left->x)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.bottom[left->x + j] <= tmp[j])
		{
			d.ang += d.dang;
			continue;
		}
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < tmp[j])
			d.ty = d.zu + iw->tsz[left->wall->t] *
			(d.zu - d.zd) * (float)(tmp[j] - iw->d.wallTop[j]) /
			(float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) /
			(float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
		while (d.ty > (float)iw->t[left->wall->t]->h)
			d.ty -= (float)iw->t[left->wall->t]->h;
		i = tmp[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
		{
			set_pixel2(iw->sur, left->x + j, i, 
				get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx, (int)d.ty), iw->sectors[iw->visited_sectors->sec].light));
			d.ty += d.dty;
			while (d.ty >= (float)iw->t[left->wall->t]->h)
				d.ty -= (float)iw->t[left->wall->t]->h;
		}
		iw->d.bottom[left->x + j] = tmp[j];
		d.ang += d.dang;
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		while (d.tx > (float)iw->t[left->wall->t]->w)
			d.tx -= (float)iw->t[left->wall->t]->w;
	}
}

void	draw_between_sectors_top_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int *tmp)
{
	int		i;
	int		j;
	t_draw_wall_tex	d;

	if (left->wall->t < 0)
		return;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)(right->x - left->x);
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;

	d.left_len = 0.0f;
	d.tx = left->olen * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
	while (d.tx >= (float)iw->t[left->wall->t]->w)
		d.tx -= (float)iw->t[left->wall->t]->w;

	j = -1;
	while (++j < right->x - left->x)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= tmp[j])
		{
			d.ang += d.dang;
			continue;
		}
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < tmp[j])
			d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[left->x + j] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
		while (d.ty > (float)iw->t[left->wall->t]->h)
			d.ty -= (float)iw->t[left->wall->t]->h;
		/*i = tmp[j] - 1;*/
		i = iw->d.top[left->x + j] - 1;
		while (++i < tmp[j] && i < iw->d.bottom[left->x + j])
		{
			set_pixel2(iw->sur, left->x + j, i,
				get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx, (int)d.ty), iw->sectors[iw->visited_sectors->sec].light));
			d.ty += d.dty;
			while (d.ty >= (float)iw->t[left->wall->t]->h)
				d.ty -= (float)iw->t[left->wall->t]->h;
		}
		iw->d.top[left->x + j] = tmp[j];
		d.ang += d.dang;
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		while (d.tx > (float)iw->t[left->wall->t]->w)
			d.tx -= (float)iw->t[left->wall->t]->w;
	}
}

void	draw_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;
	if (iw->sectors[iw->d.cs].cl.t < 0)
	{
		d.rot = (iw->p.rot - iw->v.angle) + (float)left->x /
			(float)WINDOW_W * iw->v.angle * 2.0f;
		if (d.rot < 0.0f)
			d.rot += G360;
		else if (d.rot > G360)
			d.rot -= G360;
		d.sky_x = d.rot * ((float)iw->t[iw->l.skybox]->w) / G360;
		d.dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
		d.dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	}
	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.frcoef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len) * d.frcoef;
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;


		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
		{
			if (iw->sectors[iw->d.cs].cl.t >= 0)
			{
				if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
					i = iw->d.wallTop[j] + 1;
				else
					i = iw->d.bottom[left->x + j] + 1;
				d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
				while (--i >= iw->d.top[left->x + j])
				{
					d.weight = d.wall_dist / d.k;
					d.k += d.clpl;
					d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
					d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
					set_pixel2(iw->sur, left->x + j, i,
						get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
						((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
						((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
							((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
						((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
					), iw->sectors[iw->d.cs].light));
				}
				iw->d.top[left->x + j] = iw->d.wallTop[j];
			}
			else
			{
				d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
				d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
				i = iw->d.top[left->x + j] - 1;
				d.sky_y += d.dy * iw->d.top[left->x + j];
				if (d.sky_x > iw->t[iw->l.skybox]->w)
					d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
				while (++i < iw->d.wallTop[j] && i < iw->d.bottom[left->x + j])
				{
					set_pixel2(iw->sur, j + left->x, i,
						get_pixel(iw->t[iw->l.skybox],
						(int)d.sky_x, (int)d.sky_y));
					d.sky_y += d.dy;
				}
			}
		}
		if (iw->sectors[iw->d.cs].cl.t < 0)
			d.sky_x += d.dx;
		d.ang += d.dang;
	}
}

void	draw_inclined_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;
	if (iw->sectors[iw->d.cs].cl.t < 0)
	{
		d.rot = (iw->p.rot - iw->v.angle) + (float)left->x /
			(float)WINDOW_W * iw->v.angle * 2.0f;
		if (d.rot < 0.0f)
			d.rot += G360;
		else if (d.rot > G360)
			d.rot -= G360;
		d.sky_x = d.rot * ((float)iw->t[iw->l.skybox]->w) / G360;
		d.dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
		d.dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	}

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.frcoef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.clcoef = d.frcoef;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len);
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist * d.frcoef / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				d.frcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
					get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.frcoef) / 2.0f;
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
		{
			if (iw->sectors[iw->d.cs].cl.t >= 0)
			{
				if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
					i = iw->d.wallTop[j] + 1;
				else
					i = iw->d.bottom[left->x + j] + 1;
				d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
				while (--i >= iw->d.top[left->x + j])
				{
					d.weight = d.wall_dist * d.clcoef / d.k;
					d.k += d.clpl;
					d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
					d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
					d.clcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
						get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.clcoef) / 2.0f;
					set_pixel2(iw->sur, left->x + j, i,
						get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
						((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
						((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
							((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
						((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
					), iw->sectors[iw->d.cs].light));
				}
				iw->d.top[left->x + j] = iw->d.wallTop[j];
			}
			else
			{
				d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
				d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
				i = iw->d.top[left->x + j] - 1;
				d.sky_y += d.dy * iw->d.top[left->x + j];
				if (d.sky_x > iw->t[iw->l.skybox]->w)
					d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
				while (++i < iw->d.wallTop[j] && i < iw->d.bottom[left->x + j])
				{
					set_pixel2(iw->sur, j + left->x, i,
						get_pixel(iw->t[iw->l.skybox],
						(int)d.sky_x, (int)d.sky_y));
					d.sky_y += d.dy;
				}
			}
		}
		if (iw->sectors[iw->d.cs].cl.t < 0)
			d.sky_x += d.dx;
		d.ang += d.dang;
	}
}

void	draw_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	/*d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;*/
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, 0, 0);
	d.zd = get_floor_z(iw, 0, 0);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;
	d.frcoef = d.zu - d.zd;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			/*if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;*/
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;

		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len) * d.frcoef;
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;


		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j] && iw->sectors[iw->d.cs].cl.t >= 0)
		{
			if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				i = iw->d.wallTop[j] + 1;
			else
				i = iw->d.bottom[left->x + j] + 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
			while (--i >= iw->d.top[left->x + j])
			{
				d.weight = d.wall_dist / d.k;
				d.k += d.clpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
				), iw->sectors[iw->d.cs].light));
			}
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		}

		if (left->wall->t >= 0)
		{
			d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
			else
				d.ty = d.zu;
			d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
			d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallTop[j] - 1;
			while (++i < iw->d.bottom[left->x + j])
			{
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h),
					iw->sectors[iw->d.cs].light));
				d.ty += d.dty;
			}
			if (iw->d.wallTop[j] < iw->d.top[j + left->x] &&
				iw->d.top[left->x + j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.top[left->x + j] - 1;
			else if (iw->d.wallTop[j] >= iw->d.top[j + left->x] &&
				iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.wallTop[j] - 1;
		}
		//iw->d.top[left->x + j] = iw->d.bottom[left->x + j];

		d.ang += d.dang;
	}
}

void	draw_inclined_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_floor_ceil_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.frpl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.clpl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.frcoef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.clcoef = d.frcoef;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			iw->d.screen_len);
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		//printf("wd %f frcoef %f\n", d.wall_dist, d.frcoef);

		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
		{
			if (iw->d.wallBot[j] < iw->d.top[left->x + j])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallBot[j] - 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.frpl * (float)(i + 1 - iw->d.wallBot[j]);
			while (++i < iw->d.bottom[left->x + j])
			{
				d.weight = d.wall_dist * d.frcoef / d.k;
				d.k += d.frpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				d.frcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
					get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.frcoef) / 2.0f;
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
				), iw->sectors[iw->d.cs].light));
			}
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		}

		if (iw->d.wallTop[j] > iw->d.top[left->x + j] && iw->sectors[iw->d.cs].cl.t >= 0)
		{
			if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				i = iw->d.wallTop[j] + 1;
			else
				i = iw->d.bottom[left->x + j] + 1;
			d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.clpl * (float)(iw->d.wallTop[j] - i + 1);
			while (--i >= iw->d.top[left->x + j])
			{
				d.weight = d.wall_dist * d.clcoef / d.k;
				d.k += d.clpl;
				d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
				d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
				d.clcoef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
					get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.clcoef) / 2.0f;
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
					((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw->d.cs].cl.t]->w - 1) :
					((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)),
						((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h - 1) :
					((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->t[iw->sectors[iw->d.cs].cl.t]->h))
				), iw->sectors[iw->d.cs].light));
			}
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		}

		if (left->wall->t >= 0)
		{
			d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
			d.zu = (float)left->zu + d.left_len * d.zudiff;
			d.zd = (float)left->zd + d.left_len * d.zddiff;
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
			else
				d.ty = d.zu;
			d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
			d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
			if (iw->d.wallTop[j] < iw->d.top[j + left->x])
				i = iw->d.top[left->x + j] - 1;
			else
				i = iw->d.wallTop[j] - 1;
			while (++i < iw->d.bottom[left->x + j])
			{
				set_pixel2(iw->sur, left->x + j, i,
					get_light_color(get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h),
					iw->sectors[iw->d.cs].light));
				d.ty += d.dty;
			}
			if (iw->d.wallTop[j] < iw->d.top[j + left->x] &&
				iw->d.top[left->x + j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.top[left->x + j] - 1;
			else if (iw->d.wallTop[j] >= iw->d.top[j + left->x] &&
				iw->d.wallTop[j] < iw->d.bottom[left->x + j])
				iw->d.bottom[left->x + j] = iw->d.wallTop[j] - 1;
		}
		//if (iw->d.wallTop[j] < iw->d.top[j + left->x])
		//	iw->d.bottom[left->x + j] = iw->d.top[left->x + j] - 1;
		//else
		//	iw->d.bottom[left->x + j] = iw->d.wallTop[j] - 1;
		//iw->d.top[left->x + j] = iw->d.bottom[left->x + j];

		d.ang += d.dang;
	}
}

void	draw_between_sectors_walls(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_draw_line		l;
	int				lz;
	int				rz;
	int				*tmp;

	tmp = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	l.x0 = left->x;
	l.x1 = right->x;
	/*lz = get_floor_z(iw, left->wall->x, left->wall->y);
	rz = get_floor_z(iw, right->wall->x, right->wall->y);*/
	// lz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_floor_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_floor_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(tmp, l);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& tmp[WINDOW_W / 2 - left->x] < WINDOW_H / 2)
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	draw_between_sectors_bot_tex(iw, left, right, tmp);
	/*lz = get_ceil_z(iw, left->wall->x, left->wall->y);
	rz = get_ceil_z(iw, right->wall->x, right->wall->y);*/
	// lz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_ceil_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_ceil_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(tmp, l);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& tmp[WINDOW_W / 2 - left->x] > WINDOW_H / 2)
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	draw_between_sectors_top_tex(iw, left, right, tmp);
	free(tmp);
}

int		*get_between_sectors_walls(t_sdl *iw, t_save_wall *left, t_save_wall *right, int **top)
{
	t_draw_line		l;
	int				lz;
	int				rz;
	int				*bottom;

	bottom = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	*top = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	l.x0 = left->x;
	l.x1 = right->x;
	/*lz = get_floor_z(iw, left->wall->x, left->wall->y);
	rz = get_floor_z(iw, right->wall->x, right->wall->y);*/
	// lz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_floor_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_floor_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(bottom, l);
	/*draw_between_sectors_bot_tex(iw, left, right, tmp);*/

	/*lz = get_ceil_z(iw, left->wall->x, left->wall->y);
	rz = get_ceil_z(iw, right->wall->x, right->wall->y);*/
	// lz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	// rz = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
	lz = get_ceil_z(iw, left->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, left->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	rz = get_ceil_z(iw, right->p.x + iw->walls[left->wall->nextsector_wall].x - left->wall->next->x, right->p.y + iw->walls[left->wall->nextsector_wall].y - left->wall->next->y);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - lz) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - rz) / (int)right->plen + iw->p.rotup;
	brez_line(*top, l);
	/*draw_between_sectors_top_tex(iw, left, right, tmp);*/
	return (bottom);
}

void	fill_portal(t_sdl *iw, t_save_wall *left, t_save_wall *right, t_sdl *iw2)
{
	int		j;

	j = left->x - 1;
	while (++j < right->x)
	{
		if (iw2->d.top[j] > iw->d.top[j])
			iw->d.top[j] = iw2->d.top[j];
		if (iw2->d.bottom[j] < iw->d.bottom[j])
			iw->d.bottom[j] = iw2->d.bottom[j];
	}
}

void	fill_top_skybox(t_sdl *iw2, t_save_wall *left, int len)
{
	int		i;

	i = -1;
	while (++i < len)
		if (iw2->d.wallTop[i] > iw2->d.top[left->x + i])
			iw2->d.top[left->x + i] = iw2->d.wallTop[i];
}

void	fill_bot_by_wallTop(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;

	i = -1;
	while (++i < len)
		if (iw->d.wallTop[i] < iw->d.bottom[left->x + i])
			iw->d.bottom[left->x + i] = iw->d.wallTop[i];
}

void	fill_tb_by_slsr(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < iw->d.screen_left)
		iw->d.top[i] = iw->d.bottom[i];
	i = iw->d.screen_right - 1;
	while (++i <= WINDOW_W)
		iw->d.top[i] = iw->d.bottom[i];
}

void	change_saved_top_bot_between_lines(t_sdl *iw, int len)
{
	int		j;

	// j = -1;
	// while (++j < len)
	// {
	// 	if (iw->d.top[j + left_x] > iw->d.save_top_betw[j])
	// 		iw->d.save_top_betw[j] = iw->d.top[j + left_x];
	// 	if (iw->d.bottom[left_x + j] < iw->d.save_bot_betw[j])
	// 		iw->d.save_bot_betw[j] = iw->d.bottom[left_x + j];
	// }
	if (iw->d.wallTop[0] > iw->d.save_top_betw[0])
	{
		j = -1;
		while (++j < len)
			iw->d.save_top_betw[j] = iw->d.wallTop[j];
	}
	if (iw->d.wallBot[0] < iw->d.save_bot_betw[0])
	{
		j = -1;
		while (++j < len)
			iw->d.save_bot_betw[j] = iw->d.wallBot[j];
	}
}

void	draw_glass_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		j;
	int		i;
	t_draw_glass	d;

	/*printf("GLAAAAAAAAAAASSSSSSSSS\n");
	j = -1;
	while (++j < len)
	{
		i = iw->d.save_top_betw[j] - 1;
		while (++i < iw->d.save_bot_betw[j])
			set_pixel2(iw->sur, j + left->x, i, 0x00FF00);
	}*/
	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));

	i = iw->d.cs;
	iw->d.cs = left->wall->nextsector;
	d.nleft_zu = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	d.nleft_zd = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	if (left->zu < d.nleft_zu)
	{
		d.zudiff = (right->zu - left->zu) / d.len_lr;
		d.nleft_zu = left->zu;
	}
	else
	{
		d.nright_zu = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
		d.zudiff = (d.nright_zu - d.nleft_zu) / d.len_lr;
	}
	if (left->zd > d.nleft_zd)
	{
		d.zddiff = (right->zd - left->zd) / d.len_lr;
		d.nleft_zd = left->zd;
	}
	else
	{
		d.nright_zd = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
		d.zddiff = (d.nright_zd - d.nleft_zd) / d.len_lr;
	}
	iw->d.cs = i;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sinf(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->glass]->w * iw->tsz[left->wall->glass] / 1000.0f;
		d.zu = (float)d.nleft_zu + d.left_len * d.zudiff;
		d.zd = (float)d.nleft_zd + d.left_len * d.zddiff;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->glass]->h / 1000.0f) /
			(float)(iw->d.save_bot_betw[j] - iw->d.save_top_betw[j]) * iw->tsz[left->wall->glass];
		d.ty = 0.0f;
		if (iw->d.save_top_betw[j] < iw->d.top[left->x + j])
		{
			d.ty += (float)(iw->d.top[left->x + j] - iw->d.save_top_betw[j]) * d.dty;
			i = iw->d.top[left->x + j] - 1;
		}
		else
			i = iw->d.save_top_betw[j] - 1;

		while (++i < iw->d.save_bot_betw[j] && i < iw->d.bottom[j + left->x])
		{
			d.pixel = get_pixel(iw->t[left->wall->glass],
				(int)d.tx % iw->t[left->wall->glass]->w,
				(int)d.ty % iw->t[left->wall->glass]->h);
			if (d.pixel != 0x010000)
				set_pixel2(iw->sur, left->x + j, i, get_light_color(d.pixel, iw->sectors[iw->d.cs].light));
			d.ty += d.dty;
		}
		d.ang += d.dang;
	}
}

// int		portal_not_visited(t_sdl *iw, t_wall *wall)
// {
// 	t_visited_portal	*p;

// 	p = iw->visited_portals;
// 	while (p)
// 	{
// 		if (wall == p->wall)
// 			return (0);
// 		p = p->next;
// 	}
// 	return (1);
// }

int		sector_visited(t_sdl *iw, int sec)
{
	t_visited_sector	*s;

	s = iw->visited_sectors;
	while (s)
	{
		if (sec == s->sec)
			return (1);
		s = s->next;
	}
	return (0);
}

void	draw_glass_sprites(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (tmp1->num_sec == iw->d.cs && tmp1->draweble)
		{
			draw_sprite(iw, tmp1);
			tmp1->draweble = 0;
		}
		tmp1 = tmp1->next; 
	}
}

void	draw_start(t_sdl *iw);
void	draw_skybox(t_sdl *iw);
void	draw_next_sector(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_sdl	iw2;
	t_visited_sector	*tmp;
	int					i;

	//printf("next_sector\n");
	iw2 = *iw;
	//	if (iw->sectors[iw->d.cs].cl.t < 0)
	fill_top_skybox(&iw2, left, right->x - left->x + 1);
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	iw->d.save_bot_betw = get_between_sectors_walls(&iw2, left, right, &iw->d.save_top_betw);
	draw_between_sectors_walls(&iw2, left, right);
	i = left->x - 1;
	while (++i < right->x)
	{
		iw->d.top[i] = iw2.d.top[i];
		iw->d.bottom[i] = iw2.d.bottom[i];
	}
	if (sector_visited(iw, iw2.d.cs))
	{
		free(iw->d.save_bot_betw);
		free(iw->d.save_top_betw);
		return;
	}
	//fill_portal(iw, left, right, &iw2);
	get_direction(&iw2);
	get_screen_line(&iw2);
	get_left_right_lines_points(&iw2);
	//recalculate_angle(&iw2); 
	iw2.d.vw = 0;
	iw2.d.vwp = 0;
	iw2.d.screen_left = left->x;
	iw2.d.screen_right = right->x;
	fill_tb_by_slsr(&iw2);
	get_visible_walls(&iw2);
	get_left_right_visible_walls(&iw2);
	iw2.d.prev_sector = iw->d.cs;
	iw2.d.prev_sector_wall = left->wall;

	iw->sectors[iw2.d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw2.d.cs;
	tmp->next = iw2.visited_sectors;
	iw2.visited_sectors = tmp;
	draw_start(&iw2);
	iw2.visited_sectors = iw2.visited_sectors->next;
	free(tmp);

	if (left->wall->glass >= 0)
	{
		if (iw->sectors[iw2.d.cs].cl.t < 0)
			draw_skybox(&iw2);
		sort_sprites(iw);
		draw_glass_sprites(&iw2);
		change_saved_top_bot_between_lines(iw, right->x - left->x + 1);
		draw_glass_tex(iw, left, right, right->x - left->x + 1);
	}
	free(iw->d.save_bot_betw);
	free(iw->d.save_top_betw);

	//fill_bot_by_wallTop(iw, left, right->x - left->x);
	fill_portal(iw, left, right, &iw2);
	/*iw->d.top = iw2.d.top;
	iw->d.bottom = iw2.d.bottom;*/
}

void	draw_next_sector_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_sdl	iw2;
	t_visited_sector	*tmp;

	iw2 = *iw;
	iw2.p.x += iw->walls[left->wall->nextsector_wall].x - left->wall->next->x;
	iw2.p.y += iw->walls[left->wall->nextsector_wall].y - left->wall->next->y;
	iw2.d.cs = left->wall->nextsector;
	iw->d.save_bot_betw = get_between_sectors_walls(&iw2, left, right, &iw->d.save_top_betw);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& (iw->d.save_top_betw[WINDOW_W / 2 - left->x] > WINDOW_H / 2 ||
			iw->d.save_bot_betw[WINDOW_W / 2 - left->x] < WINDOW_H / 2))
	{
		*(iw->v.look_wall) = left->wall;
		*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
	}
	if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
		draw_floor_ceil_betw_tex_kernel(iw, left, right, len);
	else
		draw_inclined_floor_ceil_betw_tex_kernel(iw, left, right, len);
	if ((iw->d.wallBot[0] < 0 && iw->d.wallBot[len - 1] < 0) ||
		(iw->d.wallTop[0] >= WINDOW_H && iw->d.wallTop[len - 1] >= WINDOW_H)
		|| sector_visited(iw, iw2.d.cs))
	{
		free(iw->d.save_bot_betw);
		free(iw->d.save_top_betw);
		return;
	}
	/*fill_portal_rev(iw, &iw2);*/
	get_direction(&iw2);
	get_screen_line(&iw2);
	get_left_right_lines_points(&iw2);
	iw2.d.vw = 0;
	iw2.d.vwp = 0;
	if (left->x > iw2.d.screen_left)
		iw2.d.screen_left = left->x;
	if (right->x < iw2.d.screen_right)
		iw2.d.screen_right = right->x;
	fill_tb_by_slsr(&iw2);
	get_visible_walls(&iw2);
	get_left_right_visible_walls(&iw2);
	iw2.d.prev_sector = iw->d.cs;
	iw2.d.prev_sector_wall = left->wall;
	if (left->wall->glass >= 0)
	{
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_top,
			iw->k.m_save_top, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
		clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_bottom,
			iw->k.m_save_bottom, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
	}

	iw->sectors[iw2.d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw2.d.cs;
	tmp->next = iw2.visited_sectors;
	iw2.visited_sectors = tmp;
	draw_start(&iw2);
	iw2.visited_sectors = iw2.visited_sectors->next;
	free(tmp);

	if (left->wall->glass >= 0)
	{
		if (iw->sectors[iw2.d.cs].cl.t < 0)
			draw_skybox_kernel(&iw2);
		sort_sprites(iw);
		draw_glass_sprites_kernel(&iw2);
		change_saved_top_bot_between_lines(iw, len);
		draw_glass_tex_kernel(iw, left, right, len);
		/*clReleaseMemObject(iw->k.m_save_bottom);
		clReleaseMemObject(iw->k.m_save_top);*/
	}
	free(iw->d.save_bot_betw);
	free(iw->d.save_top_betw);
}

int		draw_picture(t_sdl *iw, t_picture *pic)
{
	t_draw_picture	d;
	int		i;
	int		j;

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx0 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d.rx0 = -(int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx1 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d.rx1 = -(int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));

	d.plen = fabsf(iw->d.screen.a * (float)pic->x0 + iw->d.screen.b * (float)pic->y0 + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	d.ry0_up = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	d.ry0_down = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	d.plen = fabsf(iw->d.screen.a * (float)pic->x1 + iw->d.screen.b * (float)pic->y1 + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);

	d.ry1_up = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	d.ry1_down = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	d.pic_x = 0;
	d.down = d.ry0_down - d.ry1_down;
	d.up = d.ry0_up - d.ry1_up;
	d.dx = d.rx0 - d.rx1;
	d.dy_down = d.down / d.dx;
	d.dy_up = d.up / d.dx;
	i = d.rx1;
	if (i < 0)
	{
		d.pic_x += iw->t[pic->t]->w * abs(i) / d.dx;
		d.dy_down += (float)(d.down / d.dx * abs(i));
		d.dy_up += (float)(d.up / d.dx * abs(i));
		i = 0;
	}
	while (i++ <= d.rx0 && i <= WINDOW_W)
	{
		if (iw->d.top_save[i] >= iw->d.bottom_save[i])
			d.pic_x += iw->t[pic->t]->w / d.dx;
		else
		{
			d.dy_plus = iw->t[pic->t]->h / ((d.ry1_down + (float)d.dy_down) - (d.ry1_up + (float)d.dy_up));
			j = d.ry1_up + (float)d.dy_up;
			if (j >= iw->d.top_save[i])
				d.pic_y = 0;
			else
			{
				d.pic_y = d.dy_plus * (iw->d.top_save[i] - j);
				j = iw->d.top_save[i];
			}
			while (j++ < d.ry1_down + (float)d.dy_down && j < iw->d.bottom_save[i])
			{
				d.pixel = get_pixel(iw->t[pic->t], (int)d.pic_x, (int)d.pic_y);
				if (d.pixel != 0x010000)
					set_pixel2(iw->sur, i, j, get_light_color(d.pixel, 
						iw->sectors[iw->d.cs].light));
				d.pic_y += d.dy_plus;
			}
			d.pic_x += iw->t[pic->t]->w / d.dx;
			d.dy_down += (float)(d.down / d.dx);
			d.dy_up += (float)(d.up / d.dx);
		}
	}
	if (d.rx1 <= WINDOW_W / 2 && d.rx0 >= WINDOW_W / 2)
		return (1);
	return (0);
}

void	draw_pictures(t_sdl *iw, t_save_wall *left)
{
	t_picture	*pic;

	pic = left->wall->p;
	while (pic != 0)
	{
		if (draw_picture(iw, pic) && *(iw->v.look_picture) == 0)
			*(iw->v.look_picture) = pic;
		pic = pic->next;
	}
}

void	draw_all(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	if (left->wall->nextsector == -1)
	{
		/*draw_ceil_tex(iw, left, right, len);
		draw_inclined_floor_tex_kernel(iw, left, right, len);
		draw_wall_tex_kernel(iw, left, right, len);*/
		if (left->wall->p != 0)
		{
			// i = -1;
			// while (++i <= WINDOW_W)
			// {
			// 	iw->d.top_save[i] = iw->d.top[i];
			// 	iw->d.bottom_save[i] = iw->d.bottom[i];
			// }
			ft_memcpy(iw->d.top_save, iw->d.top, WINDOW_W * sizeof(int));
			ft_memcpy(iw->d.bottom_save, iw->d.bottom, WINDOW_W * sizeof(int));
		}
		if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_wall_floor_ceil_tex(iw, left, right, len);
		else
			draw_inclined_wall_floor_ceil_tex(iw, left, right, len);
		if (left->wall->p != 0)
			draw_pictures(iw, left);
	}
	else
	{
		if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_floor_ceil_tex(iw, left, right, len);
		else
			draw_inclined_floor_ceil_tex(iw, left, right, len);
		draw_next_sector(iw, left, right);
	}
}

void	draw_all_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	if (left->wall->nextsector == -1)
	{
		/*draw_ceil_tex(iw, left, right, len);
		draw_inclined_floor_tex_kernel(iw, left, right, len);
		draw_wall_tex_kernel(iw, left, right, len);*/
		if (left->wall->p != 0)
		{
			clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_top,
				iw->k.m_save_top2, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
			clEnqueueCopyBuffer(iw->k.command_queue, iw->k.m_bottom,
				iw->k.m_save_bottom2, 0, 0, WINDOW_W * sizeof(int), 0, NULL, NULL);
		}
		if (iw->sectors[iw->d.cs].fr.n == 0 && iw->sectors[iw->d.cs].cl.n == 0)
			draw_wall_floor_ceil_tex_kernel(iw, left, right, len);
		else
			draw_inclined_wall_floor_ceil_tex_kernel(iw, left, right, len);
		if (left->wall->p != 0)
			draw_pictures_kernel(iw, left);
	}
	else
		draw_next_sector_kernel(iw, left, right, len);
}

void	draw_left_right(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_draw_line		l;

	if (left->x >= right->x)
		return;
	//printf("malloc\n");
	iw->d.wallTop = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	iw->d.wallBot = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	//printf("malloced\n");
	if (!iw->d.wallTop || !iw->d.wallBot)
		return;
	l.x0 = left->x;
	l.x1 = right->x;
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zd) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zd) / (int)right->plen + iw->p.rotup;
	brez_line(iw->d.wallBot, l);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zu) / (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zu) / (int)right->plen + iw->p.rotup;
	brez_line(iw->d.wallTop, l);
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1 && left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
		&& iw->d.screen_left < WINDOW_W / 2 && iw->d.screen_right > WINDOW_W / 2)
	{
		if (left->wall->nextsector != -1 && iw->v.look_portal == 0)
			iw->v.look_portal = left->wall;
		if (left->wall->nextsector == -1)
		{
			*(iw->v.look_wall) = left->wall;
			*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
		}
		else if (left->wall->nextsector == iw->d.prev_sector)
		{
			*(iw->v.look_wall) = left->wall;
			*(iw->v.look_sector) = &iw->sectors[iw->d.cs];
		}
	}
	if (iw->v.kernel)
		draw_all_kernel(iw, left, right, right->x - left->x + 1);
	else
		draw_all(iw, left, right, right->x - left->x + 1);
	//printf("draw lpx %d lpy %d rpx %d rpy %d lplen %f lx %d rx %d\n", left->wall->x, left->wall->y, right->wall->x, right->wall->y, left->plen, left->x, right->x);
	/*SDL_UpdateWindowSurface(iw->win);
	system("PAUSE");*/
	free(iw->d.wallBot);
	free(iw->d.wallTop);
}

void	add_pair(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;

	tmp = (t_save_wall_pairs *)malloc(sizeof(t_save_wall_pairs));
	tmp->left = left;
	tmp->right = right;
	tmp->next = 0;
	if (iw->d.vwp == 0)
		iw->d.vwp = tmp;
	else
	{
		tmp2 = iw->d.vwp;
		while (tmp2->next != 0)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
}

t_save_wall_pairs	*get_closest_between_pair(t_save_wall_pairs	*pair)
{
	t_save_wall_pairs	*save;
	t_save_wall_pairs	*tmp;

	save = 0;
	tmp = pair->next;
	while (tmp != 0)
	{
		//(bx-ax)*(py-ay)-(by-ay)*(px-ax)
		if (tmp != pair)
			if ((tmp->left->x >= pair->left->x && tmp->left->x < pair->right->x &&
				((pair->left->wall->x - pair->right->wall->x) * (tmp->left->p.y - pair->right->wall->y) -
				(pair->left->wall->y - pair->right->wall->y) * (tmp->left->p.x - pair->right->wall->x) > 0)) ||
					(tmp->right->x > pair->left->x && tmp->right->x <= pair->right->x &&
				((pair->left->wall->x - pair->right->wall->x) * (tmp->right->p.y - pair->right->wall->y) -
						(pair->left->wall->y - pair->right->wall->y) * (tmp->right->p.x - pair->right->wall->x) > 0)))
				save = tmp;
		tmp = tmp->next;
	}
	return (save);
}

int		check_look_picture(t_sdl *iw)
{
	t_picture	*pic;

	if (*(iw->v.look_wall) == 0 || *(iw->v.look_picture) == 0)
		return (0);
	pic = (*(iw->v.look_wall))->p;
	while (pic)
	{
		if (pic == *(iw->v.look_picture))
			return (1);
		pic = pic->next;
	}
	return (0);
}

void	new_sort_pairs(t_sdl *iw)
{
	t_save_wall_pairs	start;
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;
	t_save_wall_pairs	*after;
	int					i;

	start.next = iw->d.vwp;
	tmp = &start;
	i = 0;
	while (tmp->next != 0)
	{
		i++;
		if (i > 1000)
			break;
		if ((after = get_closest_between_pair(tmp->next)) != 0)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			tmp2->next = after->next;
			after->next = tmp2;
		}
		else
			tmp = tmp->next;
		//////
		/*t_save_wall_pairs *ttt;
		ttt = iw->d.vwp;
		while (ttt != 0)
		{
			printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
			ttt = ttt->next;
		}
		printf("\n\n");*/
		/////
	}
	iw->d.vwp = start.next;
}

void	draw_skybox(t_sdl *iw)
{
	int j;
	int i;
	t_draw_skybox	d;

	d.rot = iw->p.rot - iw->v.angle;
	if (d.rot < 0.0f)
		d.rot += G360;
	d.dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
	d.sky_x = d.rot * ((float)iw->t[iw->l.skybox]->w) / G360
		+ d.dx * (float)iw->d.screen_left;
	d.dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	j = iw->d.screen_left - 1;
	while (++j < iw->d.screen_right)
	{
		d.sky_y = -iw->p.rotup + 2 * WINDOW_H;
		d.sky_y = (d.sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
		d.sky_y += d.dy * iw->d.top[j];
		i = iw->d.top[j] - 1;
		while (++i <= iw->d.bottom[j] && i < WINDOW_H)
		{
			set_pixel2(iw->sur, j, i,
				get_pixel(iw->t[iw->l.skybox],
				(int)d.sky_x, (int)d.sky_y));
			d.sky_y += d.dy;
		}
		d.sky_x += d.dx;
		if (d.sky_x >= iw->t[iw->l.skybox]->w)
			d.sky_x = d.sky_x - iw->t[iw->l.skybox]->w;
		iw->d.bottom[j] = iw->d.top[j];
	}
}

// int		get_max(int i1, int i2)
// {
// 	if (i1 > i2)
// 		return (i1);
// 	else
// 		return (i2);
// }

// int		get_min(int i1, int i2)
// {
// 	if (i1 < i2)
// 		return (i1);
// 	else
// 		return (i2);
// }
// SPRITES DRAW /////////////////////////////////////////////////////////////////////////////////////////////////////
void	draw_sprite(t_sdl *iw, t_sprite *sprite)
{
	int		i;
	int		j;
	int		stripe;
	int		y;
	float	koef;
	int		texX;
	int		texY;
	int		colour;

	sprite->spriteheight = 2 * sprite->spritewidth * sprite->t->h / sprite->t->w;
	if (abs(sprite->z - iw->p.z) > 1500)
		sprite->spriteheight = (float)sprite->spriteheight * 1.0f /
		(float)(abs(sprite->z - iw->p.z) / 1500.0f);
	sprite->ey = WINDOW_H * (iw->p.z + (int)sprite->pplen / 2 - sprite->z) / (int)sprite->pplen + iw->p.rotup;
	sprite->sy = sprite->ey - sprite->spriteheight;
	i = 0;
	stripe = sprite->sx - 1;
	if (stripe < -1)
	{
		i -= stripe + 1;
		stripe = -1;
	}
	while (++stripe < sprite->ex && stripe < WINDOW_W)
	{
		j = 0;
		koef = (float)sprite->spritewidth * 2.0f / (float)sprite->t->w;
		texX = (int)fabsf((float)i / koef);
		if ((sprite->top[stripe] < sprite->bottom[stripe])
			&& sprite->top[stripe] != -1)
		{
			y = sprite->sy - 1;
			if (y < -1)
			{
				j -= y + 1;
				y = -1;
			}
			while (++y < sprite->ey && y < WINDOW_H)
			{
				if (sprite->sy < WINDOW_H && sprite->bottom[stripe] >y && sprite->top[stripe] < y)
				{
					koef = (float)sprite->spriteheight / sprite->t->h;
					texY = (int)(j / koef);
					colour = get_pixel(sprite->t, texX, texY);
					if (colour != 0x010000)
						set_pixel2(iw->sur, stripe, y,
							get_light_color(colour, iw->sectors[sprite->num_sec].light));
				}
				j++;
			}
	  	}
		i++;
	}
}

void	draw_sprites(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (iw->sectors[tmp1->num_sec].visited && tmp1->draweble && tmp1->taken == 0)
		{
			if (tmp1->top[WINDOW_W / 2] != -1 && tmp1->sx < WINDOW_W / 2 && tmp1->ex > WINDOW_W / 2 &&
				tmp1->top[WINDOW_W / 2] < WINDOW_H / 2 && tmp1->bottom[WINDOW_W / 2] > WINDOW_H / 2
				&& tmp1->sy < WINDOW_H / 2 && tmp1->ey > WINDOW_H / 2)
				iw->v.look_sprite = tmp1;
			draw_sprite(iw, tmp1);
		}
		tmp1 = tmp1->next; 
	}
}

int		find_point(t_save_wall_pairs *tmp, t_sprite *tmp1)
{
	int		i;
	int ax = tmp->right->p.x-tmp->left->p.x;
	int ay = tmp->right->p.y-tmp->left->p.y;
	int bx = tmp1->x - tmp->left->p.x;
	int by = tmp1->y - tmp->left->p.y;
	i = ax * by - ay * bx;
	if (i > 0)
		return (0);
	else 
		return (1);
}

void	calculate_sprites_once(t_sdl *iw)
{
	t_sprite    *tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (!iw->sectors[tmp1->num_sec].visited || tmp1->draweble == 1)
		{
			tmp1 = tmp1->next;
			continue;
		}
		float lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(tmp1->x - iw->p.x ), (float)(tmp1->y - iw->p.y ));
		float rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
				(float)(tmp1->x - iw->p.x ), (float)(tmp1->y - iw->p.y ));
		tmp1->x_s = (int)(lang * (float)WINDOW_W / (2.0f * iw->v.angle));
		
		if (rang > 2.0f * iw->v.angle)
			tmp1->x_s = -tmp1->x_s;
		//	printf("x_s = %d\n", tmp1->x_s);
		
		tmp1->plen = sqrtf(powf((float)(iw->p.x - tmp1->x), 2.0f) +  powf((float)(iw->p.y - tmp1->y), 2.0f)) + 1.0f;
		tmp1->pplen = fabsf(iw->d.screen.a * (float)tmp1->x + iw->d.screen.b * (float)tmp1->y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b) + 1.0f;
		//printf("div  %f\n", tmp1->pplen / tmp1->plen);
		if (tmp1->pplen / tmp1->plen >= 0.5f)
			tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w) / tmp1->pplen) * tmp1->scale);
		else
			tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w) / tmp1->plen) * tmp1->scale);
		tmp1->sx = tmp1->x_s - tmp1->spritewidth;
		tmp1->ex = tmp1->x_s + tmp1->spritewidth;
		if (!(tmp1->sx > iw->d.screen_right || tmp1->ex < iw->d.screen_left))
			tmp1->draweble = 1;
		tmp1 = tmp1->next;
	}
}

void	get_sprites_top_bottom(t_sdl *iw, t_save_wall_pairs	*tmp)
{
	t_sprite    *tmp1;
	int j;
	int	jend;

	if (iw->v.kernel)
	{
		clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_top, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
		clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_bottom, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	}
	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (!iw->sectors[tmp1->num_sec].visited || !tmp1->draweble || iw->d.cs != tmp1->num_sec)
		{
			tmp1 = tmp1->next;
			continue;
		}
		if ((tmp1->sx >= tmp->left->x && tmp1->sx <= tmp->right->x)
			|| (tmp1->ex >= tmp->left->x && tmp1->ex<= tmp->right->x)
			|| (tmp1->sx <= tmp->left->x && tmp1->ex >= tmp->right->x))
			if (find_point(tmp, tmp1) == 1)
			{
				j = ft_max(tmp1->sx, ft_max(tmp->left->x, iw->d.screen_left)) - 1;
				jend = ft_min(tmp1->ex, ft_min(tmp->right->x, iw->d.screen_right));
				while (++j <= jend)
					if (tmp1->top[j] == -1)
					{
						tmp1->top[j] = iw->d.top[j];
						tmp1->bottom[j] = iw->d.bottom[j];
					}
			}
		tmp1 = tmp1->next;
	}
}

void	switch_nexts_sprites(t_sprite *s1, t_sprite *s2)
{
	t_sprite	*tmp;
	t_sprite	*tmp2;

	if (!s1 || !s2)
		return;
	if (s2->next == s1)
	{
		s2->next = s2->next->next;
		s1->next = s1->next->next;
		s2->next->next = s1;
	}
	else
	{
		tmp = s1->next->next;
		s1->next->next = s2->next->next;
		tmp2 = s2->next;
		s2->next = s1->next;
		s1->next = tmp2;
		tmp2->next = tmp;
	}
}

void	sort_sprites(t_sdl *iw)
{
	t_sprite	head;
	t_sprite	*tmp1;
	t_sprite	*tmp2;
	t_sprite	*max;

	if (!*iw->sprite)
		return;
	head.next = *iw->sprite;
	tmp1 = &head;
	while (tmp1->next->next != 0)
	{
		if (!iw->sectors[tmp1->next->num_sec].visited || !tmp1->next->draweble)
		{
			tmp1 = tmp1->next;
			continue;
		}
		max = 0;
		tmp2 = tmp1->next;
		while (tmp2->next != 0)
		{
			if (iw->sectors[tmp2->next->num_sec].visited && tmp2->next->draweble)
				//if (max == 0 || max->next->plen < tmp2->next->plen)
				if (tmp2->next->plen > tmp1->next->plen && (max == 0 || max->next->plen < tmp2->next->plen))
					max = tmp2;
			tmp2 = tmp2->next;
		}
		switch_nexts_sprites(max, tmp1);
		tmp1 = tmp1->next;
	}
	*iw->sprite = head.next;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	save_walls(t_sdl *iw)
{
	t_save_wall	*tmp;

	if (!iw->d.vw)
		return;
	tmp = iw->d.vw;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = *(iw->vw_save);
	*(iw->vw_save) = iw->d.vw;
	iw->d.vw = 0;
}

void	draw_start(t_sdl *iw)
{
	t_save_wall *left;
	t_save_wall	*right;
	t_save_wall_pairs	*tmp;

	if (iw->d.vw == 0)
		return;
	left = iw->d.vw;
	while (left != 0)
	{
		if ((right = find_next_vis_wall(iw, left)) != 0 && left->x < right->x)
			if (!(right->x < iw->d.screen_left || left->x > iw->d.screen_right))
				add_pair(iw, left, right);
		//draw_left_right(iw, left, right);
		left = left->next;
	}

	//////
	/*t_save_wall_pairs *ttt;
	ttt = iw->d.vwp;
	while (ttt != 0)
	{
		printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
		ttt = ttt->next;
	}
	printf("\n\n");*/
	/////

	new_sort_pairs(iw);

	//////
	/*printf("sorted:\n");
	ttt = iw->d.vwp;
	while (ttt != 0)
	{
		printf("left: x %d len %f; right: x %d len %f\n", ttt->left->x, ttt->left->len, ttt->right->x, ttt->right->len);
		ttt = ttt->next;
	}
	printf("\n\n");*/
	/////
	calculate_sprites_once(iw);
	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		get_sprites_top_bottom(iw, tmp);
		draw_left_right(iw, tmp->left, tmp->right);
		tmp = tmp->next;
		/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_sur, CL_TRUE, 0,
			WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);*/
		//SDL_UpdateWindowSurface(iw->win);
		// char s[500];
		// read(0, s, 10);
		//system("PAUSE");
	}

	save_walls(iw);
	//free_walls(iw);
	free_pairs(iw);
}

void	draw(t_sdl *iw)
{
	t_visited_sector	*tmp;

	t_sprite *tmp1;
	int j;

	j = 0;
	tmp1 = *iw->sprite;
	while(tmp1 != 0)
	{
		tmp1->draweble = 0;
		j = 0;
		while (j < WINDOW_W)
		{
			tmp1->top[j] = -1;
			j++;
		}
		tmp1 = tmp1->next;
	}

	j = -1;
	while (++j < iw->v.sc)
		iw->sectors[j].visited = 0;

	set_top_bottom(iw);
	if ((iw->d.cs = get_sector(iw)) == -1)
		return;
	iw->p.z -= iw->v.crouch;
	iw->v.ls = iw->d.cs;
	get_direction(iw);
	get_screen_line(iw);
	get_left_right_lines_points(iw);
	iw->d.vw = 0;
	iw->d.vwp = 0;
	iw->d.screen_left = 0;
	iw->d.screen_right = WINDOW_W - 1;
	get_visible_walls(iw);
	get_left_right_visible_walls(iw);

	//////////
	/*t_save_wall *tmp;
	tmp = iw->d.vw;
	while (tmp != 0)
	{
		if (tmp->wall == &iw->walls[0] || tmp->wall == &iw->walls[1] || tmp->wall == &iw->walls[7] || tmp->wall == &iw->walls[8])
		printf("x %d len %f\n", tmp->x, tmp->len, tmp->wall->x, tmp->wall->y, tmp->zu,
			tmp->zd, tmp->plen);
		tmp = tmp->next;
	}
	printf("\n\n");*/
	////////////
	iw->d.prev_sector = -1;
	iw->d.prev_sector_wall = 0;
	if (iw->v.kernel)
	{
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_top, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_bottom, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	}
	if (iw->v.mouse_mode == 1)
	{
		iw->v.look_portal = 0;
		*(iw->v.look_wall) = 0;
		*(iw->v.look_sector) = 0;
		*(iw->v.look_picture) = 0;
		iw->v.look_sprite = 0;
	}
	iw->sectors[iw->d.cs].visited = 1;
	tmp = (t_visited_sector *)malloc(sizeof(t_visited_sector));
	tmp->sec = iw->d.cs;
	tmp->next = iw->visited_sectors;
	iw->visited_sectors = tmp;
	draw_start(iw);
	iw->visited_sectors = iw->visited_sectors->next;
	free(tmp);
	if (!check_look_picture(iw))
		*(iw->v.look_picture) = 0;
	if (!iw->v.kernel)
		draw_skybox(iw);
	else
		draw_skybox_kernel(iw);

	sort_sprites(iw);
	if (iw->v.kernel)
		draw_sprites_kernel(iw);
	else
		draw_sprites(iw);
	
	if (iw->v.game_mode && !iw->v.kernel)
		draw_gun(iw);
	else if (iw->v.game_mode)
		draw_gun_kernel(iw);
	if (iw->v.kernel)
		iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_sur, CL_TRUE, 0,
			WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	if (iw->v.wall_anim != 0)
		draw_selected_walls_to_be_animated(iw);
	iw->d.vw = *(iw->vw_save);
	*(iw->vw_save) = 0;
	free_walls(iw);
	iw->p.z += iw->v.crouch;
	//draw_gun(iw);
}

void	read_textures(t_sdl *iw)
{
	iw->t_sur[0] = SDL_LoadBMP("textures/0.bmp");
	iw->tsz[0] = 1.0f;
	iw->t_sur[1] = SDL_LoadBMP("textures/1.bmp");
	iw->tsz[1] = 1.0f;
	iw->t_sur[2] = SDL_LoadBMP("textures/2.bmp");
	iw->tsz[2] = 1.0f;
	iw->t_sur[3] = SDL_LoadBMP("textures/3.bmp");
	iw->tsz[3] = 1.0f;
	iw->t_sur[4] = SDL_LoadBMP("textures/4.bmp");
	iw->tsz[4] = 1.0f;
	iw->t_sur[5] = SDL_LoadBMP("textures/5.bmp");
	iw->tsz[5] = 1.0f;
	iw->t_sur[6] = SDL_LoadBMP("textures/6.bmp");
	iw->tsz[6] = 1.0f;
	iw->t_sur[7] = SDL_LoadBMP("textures/7.bmp");
	iw->tsz[7] = 1.0f;
	iw->t_sur[8] = SDL_LoadBMP("textures/8.bmp");
	iw->tsz[8] = 1.0f;
	iw->t_sur[9] = SDL_LoadBMP("textures/9.bmp");
	iw->tsz[9] = 1.0f;
	iw->t_sur[10] = SDL_LoadBMP("textures/10.bmp");
	iw->tsz[10] = 1.0f;
	iw->t_sur[11] = SDL_LoadBMP("textures/11.bmp");
	iw->tsz[11] = 1.0f;
	iw->t_sur[12] = SDL_LoadBMP("textures/12.bmp");
	iw->tsz[12] = 1.0f;
	iw->t_sur[13] = SDL_LoadBMP("textures/13.bmp");
	iw->tsz[13] = 1.0f;
	iw->t_sur[14] = SDL_LoadBMP("textures/14.bmp");
	iw->tsz[14] = 1.0f;
	iw->t_sur[15] = SDL_LoadBMP("textures/15.bmp");
	iw->tsz[15] = 1.0f;
	iw->t_sur[16] = SDL_LoadBMP("textures/16.bmp");
	iw->tsz[16] = 1.0f;
	iw->t_sur[17] = SDL_LoadBMP("textures/17.bmp");
	iw->tsz[17] = 1.0f;
	iw->t_sur[18] = SDL_LoadBMP("textures/18.bmp");
	iw->tsz[18] = 1.0f;
	iw->t_sur[19] = SDL_LoadBMP("textures/19.bmp");
	iw->tsz[19] = 1.0f;
	iw->t_sur[20] = SDL_LoadBMP("textures/20.bmp");
	iw->tsz[20] = 1.0f;
	iw->t_sur[21] = SDL_LoadBMP("textures/21.bmp");
	iw->tsz[21] = 1.0f;
	iw->t_sur[22] = SDL_LoadBMP("textures/22.bmp");
	iw->tsz[22] = 1.0f;
	iw->t_sur[23] = SDL_LoadBMP("textures/23.bmp");
	iw->tsz[23] = 1.0f;
	iw->t_sur[24] = SDL_LoadBMP("textures/24.bmp");
	iw->tsz[24] = 1.0f;
	
	iw->bag.button_sur[0] = SDL_LoadBMP("interface_textures/backpack/frame.bmp");
	iw->bag.button_sur[1] = SDL_LoadBMP("interface_textures/backpack/del.bmp");
	iw->bag.button_sur[2] = SDL_LoadBMP("interface_textures/backpack/ok.bmp");

	iw->menu.icons_sur[0] = SDL_LoadBMP("interface_textures/menu/0.bmp");
	iw->menu.icons_sur[1] = SDL_LoadBMP("interface_textures/menu/1.bmp");
	iw->menu.icons_sur[2] = SDL_LoadBMP("interface_textures/menu/2.bmp");
	iw->menu.icons_sur[3] = SDL_LoadBMP("interface_textures/menu/3.bmp");
	iw->menu.icons_sur[4] = SDL_LoadBMP("interface_textures/menu/4.bmp");
	iw->menu.icons_sur[5] = SDL_LoadBMP("interface_textures/menu/5.bmp");

	iw->map.player_sur = SDL_LoadBMP("interface_textures/map/player.bmp");
	iw->hud.enot_sur = SDL_LoadBMP("interface_textures/HUD/enot.bmp");
	iw->hud.miss_sur = SDL_LoadBMP("interface_textures/HUD/miss.bmp");
	iw->hud.dead_sur = SDL_LoadBMP("interface_textures/HUD/groot_lose.bmp");
	iw->hud.win_sur = SDL_LoadBMP("interface_textures/HUD/groot_win.bmp");
	iw->hud.saved_sur = SDL_LoadBMP("interface_textures/HUD/saved.bmp");
}

void	read_sprites_textures(t_sdl *iw)
{
	iw->t_decor_sur[0] = SDL_LoadBMP("sprites/decorations/0.bmp");
	iw->t_decor_sur[1] = SDL_LoadBMP("sprites/decorations/1.bmp");
	/*iw->t_decor[1] = SDL_LoadBMP("sprites/decorations/1.bmp");
	iw->t_decor[2] = SDL_LoadBMP("sprites/decorations/2.bmp");
	iw->t_decor[3] = SDL_LoadBMP("sprites/decorations/3.bmp");*/

	iw->t_enemies_sur[0] = SDL_LoadBMP("sprites/enemies/0.bmp");
	iw->t_enemies_sur[1] = SDL_LoadBMP("sprites/enemies/1.bmp");
	iw->t_enemies_sur[2] = SDL_LoadBMP("sprites/enemies/2.bmp");
	iw->t_enemies_sur[3] = SDL_LoadBMP("sprites/enemies/3.bmp");
	iw->t_enemies_sur[4] = SDL_LoadBMP("sprites/enemies/4.bmp");
	iw->t_enemies_sur[5] = SDL_LoadBMP("sprites/enemies/5.bmp");
	iw->t_enemies_sur[6] = SDL_LoadBMP("sprites/enemies/6.bmp");
	iw->t_enemies_sur[7] = SDL_LoadBMP("sprites/enemies/7.bmp");
	iw->t_enemies_sur[8] = SDL_LoadBMP("sprites/enemies/8.bmp");
	iw->t_enemies_sur[9] = SDL_LoadBMP("sprites/enemies/9.bmp");
	iw->t_enemies_sur[10] = SDL_LoadBMP("sprites/enemies/10.bmp");
	iw->t_enemies_sur[11] = SDL_LoadBMP("sprites/enemies/11.bmp");
	iw->t_enemies_sur[12] = SDL_LoadBMP("sprites/enemies/12.bmp");
	iw->t_enemies_sur[13] = SDL_LoadBMP("sprites/enemies/13.bmp");
	iw->t_enemies_sur[14] = SDL_LoadBMP("sprites/enemies/14.bmp");
	iw->t_enemies_sur[15] = SDL_LoadBMP("sprites/enemies/15.bmp");
	iw->t_enemies_sur[16] = SDL_LoadBMP("sprites/enemies/16.bmp");
	iw->t_enemies_sur[17] = SDL_LoadBMP("sprites/enemies/17.bmp");
	iw->t_enemies_sur[18] = SDL_LoadBMP("sprites/enemies/18.bmp");
	iw->t_enemies_sur[19] = SDL_LoadBMP("sprites/enemies/19.bmp");

	iw->t_enemies_sur[20] = SDL_LoadBMP("sprites/enemies/20.bmp");
	iw->t_enemies_sur[21] = SDL_LoadBMP("sprites/enemies/21.bmp");
	iw->t_enemies_sur[22] = SDL_LoadBMP("sprites/enemies/22.bmp");
	iw->t_enemies_sur[23] = SDL_LoadBMP("sprites/enemies/23.bmp");
	iw->t_enemies_sur[24] = SDL_LoadBMP("sprites/enemies/24.bmp");
	iw->t_enemies_sur[25] = SDL_LoadBMP("sprites/enemies/25.bmp");
	iw->t_enemies_sur[26] = SDL_LoadBMP("sprites/enemies/26.bmp");
	iw->t_enemies_sur[27] = SDL_LoadBMP("sprites/enemies/27.bmp");
	iw->t_enemies_sur[28] = SDL_LoadBMP("sprites/enemies/28.bmp");
	iw->t_enemies_sur[29] = SDL_LoadBMP("sprites/enemies/29.bmp");
	

	iw->t_pickup_sur[0] = SDL_LoadBMP("sprites/to_pick_up/0.bmp");
	iw->t_pickup_sur[1] = SDL_LoadBMP("sprites/to_pick_up/1.bmp");
	iw->t_pickup_sur[2] = SDL_LoadBMP("sprites/to_pick_up/2.bmp");
	iw->t_pickup_sur[3] = SDL_LoadBMP("sprites/to_pick_up/3.bmp");
	iw->t_pickup_sur[4] = SDL_LoadBMP("sprites/to_pick_up/4.bmp");
	iw->t_pickup_sur[5] = SDL_LoadBMP("sprites/to_pick_up/5.bmp");
	iw->t_pickup_sur[6] = SDL_LoadBMP("sprites/to_pick_up/6.bmp");
	iw->t_pickup_sur[7] = SDL_LoadBMP("sprites/to_pick_up/7.bmp");
	iw->t_pickup_sur[8] = SDL_LoadBMP("sprites/to_pick_up/8.bmp");
	iw->t_pickup_sur[9] = SDL_LoadBMP("sprites/to_pick_up/9.bmp");
	iw->t_pickup_sur[10] = SDL_LoadBMP("sprites/to_pick_up/10.bmp");
}

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

// void add_sprite(t_sdl *iw, int x, int y, int z, int t, int num, int type, float scale)
// {
// 	t_sprite *tmp;

// 	tmp = (t_sprite *)malloc(sizeof(t_sprite));
// 	tmp->x = x;
// 	tmp->y = y;
// 	tmp->z = z;
// 	//tmp->t = t;
// 	tmp->num_sec = num;
// 	tmp->next = *iw->sprite;
// 	tmp->type = type;
// 	tmp->scale = scale;
// 	tmp->e.vis_esp.curr_sector = -1;
// 	tmp->e.previous_picture_change = 1;
// 	tmp->e.prev_update_time = 1;
// 	tmp->fall_time = 1;

// 	tmp->taken = 0;

// 	//tmp->e.previous_move_to_player_time = 1;
// 	(*iw->sprite) = tmp;
// 	tmp->t_numb = t;
// 	if (type == 0)
// 	{
// 		tmp->t = iw->t_decor[t];
// 		tmp->t_kernel = &iw->k.m_t_decor[t];
// 	}
// 	else if (type == 1)
// 	{
// 		tmp->t = iw->t_pickup[t];
// 		tmp->t_kernel = &iw->k.m_t_pickup[t];
// 	}
// 	else if (type == 2)
// 	{
// 		tmp->t = iw->t_enemies[t];
// 		tmp->t_kernel = &iw->k.m_t_enemies[t];
// 	}
	
// }

void	get_birth_def(t_sdl *iw)
{
	iw->p.x = iw->l.start_x;
	iw->p.y = iw->l.start_y;
	iw->p.z = -1000000;
	iw->p.introt = iw->l.start_rot;
	iw->p.rot = (float)iw->p.introt * G1;
	iw->p.rotup = 0;
	iw->v.front = 1;
	iw->v.back = 1;
	iw->v.left = 1;
	iw->v.right = 1;
	iw->v.rot_left = 1;
	iw->v.rot_right = 1;
	iw->v.rot_up = 1;
	iw->v.rot_down = 1;
	iw->v.fall = 1;
	iw->v.jump_time = 1;
	iw->v.jump = 0;
	iw->v.fly_mode = 0;
	iw->v.fly_down = 1;
	iw->v.fly_up = 1;
	iw->p.health = 100;
	iw->v.jetpack = 1;
	iw->v.sprint = 1;
	iw->v.left_mouse_pressed = 0;
	iw->v.crouch = 0;
	iw->v.crouch_pressed = 0;
}

void	get_def_new(t_sdl *iw)
{
	iw->v.ls = 0;
	iw->v.angle = (float)WINDOW_W / (float)WINDOW_H * 22.0f * G1;// 0.698132f;
	
	iw->loop_update_time = clock();
	iw->v.fps = 0;
	iw->winrect.x = 0;
	iw->winrect.y = 0;
	iw->winrect.w = WINDOW_W;
	iw->winrect.h = WINDOW_H;
	iw->v.tex_to_fill = 0;
	iw->v.scroll_first_tex = 0;
	iw->v.mouse_mode = 1;
	iw->v.scroll_tex_rect.h = 100;
	iw->v.scroll_tex_rect.w = WINDOW_W;
	iw->v.scroll_tex_rect.x = 0;
	iw->v.scroll_tex_rect.y = WINDOW_H;
	
	*(iw->v.look_wall) = 0;
	
	*(iw->v.look_sector) = 0;
	
	*(iw->v.look_picture) = 0;
	iw->v.look_sprite = 0;
	iw->v.look_portal = 0;
	iw->v.changing_fc = 0;
	iw->v.chang_fc_rect.h = 100;
	iw->v.chang_fc_rect.w = 40;
	iw->v.chang_fc_rect.x = 0;
	iw->v.chang_fc_rect.y = WINDOW_H + 100;
	iw->v.picture_changing = 0;
	iw->visited_sectors = 0;

	
	// *iw->sprite = 0;

	iw->v.f_button_mode = 0;
	
	iw->v.submenu_mode = 0;
	iw->v.sector_anim = 0;

	
	iw->v.wall_anim = 0;
	
	*(iw->vw_save) = 0;

	iw->v.sprites_select_mode = 0;
	iw->v.scroll_decor_sprites = 0;
	iw->v.scroll_pickup_sprites = 0;
	iw->v.selected_sprite_type = 0;
	iw->v.selected_sprite = 0;
	iw->v.sprite_editing = 0;

	

	iw->hud.rad = 100;
	
	iw->hud.shell = 100;

	iw->v.weapon_change_x = 0;
	iw->v.weapon_change_y = 20;
	iw->v.weapon_change_xdir = 1;
	iw->v.weapon_change_ydir = 1;
	iw->v.weapon_change_y_hide = 0;

	iw->bag.bag = 0;
	iw->bag.count_items = 0;
	iw->bag.selected_item = 0;

	iw->map.back = 0;
	iw->map.pl_rect.x = WINDOW_W / 2 - WINDOW_W / 100;
	iw->map.pl_rect.y = WINDOW_H / 2 + WINDOW_H / 34;
	iw->map.pl_rect.w = 20;
	iw->map.pl_rect.h = 20;

	iw->menu.count = 1;

	
	iw->hud.miss_time = 1;
	iw->v.have_clocks = 0;
	iw->v.last_checkpoint = 0;
	iw->hud.saved_time = 1;
	iw->v.last_to_write = 0;

	iw->v.next_music = 0;
	Mix_VolumeMusic(15);
	int		i;
	i = -1;
	while (++i < ENV_COUNT)
		Mix_Volume(i, 60);
	Mix_Volume(4, 15);
	get_birth_def(iw);
}

void	get_kernel_mem(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < TEXTURES_COUNT)
	{
		iw->k.m_t[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t[i]->pitch * iw->t[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t[i], CL_TRUE, 0,
			iw->t[i]->pitch * iw->t[i]->h, iw->t[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
	{
		iw->k.m_t_decor[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_decor[i]->pitch * iw->t_decor[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_decor[i], CL_TRUE, 0,
			iw->t_decor[i]->pitch * iw->t_decor[i]->h, iw->t_decor[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
	{
		iw->k.m_t_enemies[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_enemies[i]->pitch * iw->t_enemies[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_enemies[i], CL_TRUE, 0,
			iw->t_enemies[i]->pitch * iw->t_enemies[i]->h, iw->t_enemies[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
	{
		iw->k.m_t_pickup[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_pickup[i]->pitch * iw->t_pickup[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_pickup[i], CL_TRUE, 0,
			iw->t_pickup[i]->pitch * iw->t_pickup[i]->h, iw->t_pickup[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
	{
		iw->k.m_t_weap[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_weap[i]->pitch * iw->t_weap[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_weap[i], CL_TRUE, 0,
			iw->t_weap[i]->pitch * iw->t_weap[i]->h, iw->t_weap[i]->pixels, 0, NULL, NULL);
	}
	iw->k.m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_sur = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY,
		(WINDOW_W + 1) * (WINDOW_H + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		26 * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		19 * sizeof(float), NULL, &iw->k.ret);
	iw->k.m_top_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_bot_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top2 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom2 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top3 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom3 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
}

void	calculate_not_squared_wall_picture(t_wall *wall, t_picture *pic)
{
	float	alpha;

	alpha = get_vectors_angle(wall->next->x - wall->x, wall->next->y - wall->y,
		((wall->next->x > wall->x) ? 10 : -10), 0);
	pic->x1 = (float)wall->x + (float)pic->left_plus * cosf(alpha) *
		((wall->next->x > wall->x) ? 1.0f : -1.0f);
	pic->y1 = (float)wall->y + (float)pic->left_plus * sinf(alpha) *
		((wall->next->y > wall->y) ? 1.0f : -1.0f);

	pic->x0 = (float)wall->x + (float)(pic->left_plus + pic->tw) * cosf(alpha) *
		((wall->next->x > wall->x) ? 1.0f : -1.0f);
	pic->y0 = (float)wall->y + (float)(pic->left_plus + pic->tw) * sinf(alpha) *
		((wall->next->y > wall->y) ? 1.0f : -1.0f);
}

void	calculate_picture(t_sdl *iw, t_wall *wall, t_picture *pic)
{
	if (wall->x == wall->next->x)
	{
		pic->x1 = wall->x;
		pic->x0 = wall->x;
		if (wall->y > wall->next->y)
		{
			pic->y1 = wall->y - pic->left_plus;
			pic->y0 = pic->y1 - pic->tw;
		}
		else
		{
			pic->y1 = wall->y + pic->left_plus;
			pic->y0 = pic->y1 + pic->tw;
		}
	}
	else if (wall->y == wall->next->y)
	{
		pic->y1 = wall->y;
		pic->y0 = wall->y;
		if (wall->x > wall->next->x)
		{
			pic->x1 = wall->x - pic->left_plus;
			pic->x0 = pic->x1 - pic->tw;
		}
		else
		{
			pic->x1 = wall->x + pic->left_plus;
			pic->x0 = pic->x1 + pic->tw;
		}
	}
	else
		calculate_not_squared_wall_picture(wall, pic);

	pic->zd = pic->zu - pic->tw * iw->t[pic->t]->h * 120 / iw->t[pic->t]->w / 100;
}

void	add_picture1(t_sdl *iw)
{
	t_picture	*tmp;

	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 15;
	tmp->next = 0;
	iw->walls[5].p = tmp;
	calculate_picture(iw, &iw->walls[5], tmp);
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 5;
	tmp->next = 0;
	iw->walls[18].p = tmp;
	calculate_picture(iw, &iw->walls[18], tmp);
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 6;
	tmp->next = 0;
	iw->walls[19].p = tmp;
	calculate_picture(iw, &iw->walls[19], tmp);
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 600;
	tmp->zu = 1000;
	tmp->tw = 500;
	tmp->t = 7;
	tmp->next = 0;
	iw->walls[20].p = tmp;
	calculate_picture(iw, &iw->walls[20], tmp);
}

//	GET_SECTOR_WAYS_FUNCTIONS //////////////////////////////////////////////////////////

void	free_way(t_sector_ways **way)
{
	t_sector_way	*tmp;
	t_sector_way	*tmp2;

	if (*way == 0)
		return;
	tmp = (*way)->way_start;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	free(*way);
	*way = 0;
}

int		sector_in_way(t_sdl *iw, t_sector_ways *way, int sector)
{
	t_sector_way	*tmp;

	if (way == 0)
		return (0);
	tmp = way->way_start;
	while (tmp)
	{
		if (iw->walls[tmp->portal].nextsector == sector)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	add_portal_in_way2(t_sector_ways *tmp, int portal)
{
	t_sector_way	*nw;
	t_sector_way	*w;

	nw = (t_sector_way *)malloc(sizeof(t_sector_way));
	nw->portal = portal;
	nw->next = 0;
	if (tmp->way_start == 0)
		tmp->way_start = nw;
	else
	{
		w = tmp->way_start;
		while (w->next)
			w = w->next;
		w->next = nw;
	}
}

t_sector_ways	*add_portal_in_way(t_sector_ways *current_way, int portal)
{
	t_sector_ways	*tmp;
	t_sector_way	*way;

	tmp = (t_sector_ways *)malloc(sizeof(t_sector_ways));
	tmp->way_start = 0;
	if (current_way == 0)
	{
		tmp->way_start = (t_sector_way *)malloc(sizeof(t_sector_way));
		tmp->way_start->next = 0;
		tmp->way_start->portal = portal;
		return (tmp);
	}
	way = current_way->way_start;
	while (way)
	{
		add_portal_in_way2(tmp, way->portal);
		way = way->next;
	}
	add_portal_in_way2(tmp, portal);
	return (tmp);
}

void	go_in_sector_way(t_sdl *iw, t_get_sectors_ways *g, t_sector_ways *current_way)
{
	int				wall;
	int				save;

	if (g->current == g->to)
	{
		current_way->next = g->ways;
		g->ways = current_way;
		return;
	}
	wall = iw->sectors[g->current].sw - 1;
	while (++wall < iw->sectors[g->current].sw + iw->sectors[g->current].nw)
	{
		if (iw->walls[wall].nextsector == -1 ||
			sector_in_way(iw, current_way, iw->walls[wall].nextsector) ||
			iw->walls[wall].nextsector == g->from)
			continue;
		save = g->current;
		g->current = iw->walls[wall].nextsector;
		go_in_sector_way(iw, g, add_portal_in_way(current_way, wall));
		g->current = save;
	}
	free_way(&current_way);
}

void	get_sectors_ways2(t_sdl *iw)
{
	t_get_sectors_ways	g;

	g.from = -1;
	while (++g.from < iw->v.sc)
	{
		g.to = -1;
		while (++g.to < iw->v.sc)
		{
			if (g.from == g.to)
				continue;
			g.ways = 0;
			g.current = g.from;
			go_in_sector_way(iw, &g, 0);
			iw->ways[g.from][g.to] = g.ways;
		}
	}
}

void	get_sectors_ways(t_sdl *iw)
{
	int		i;
	int		j;

	iw->ways = (t_sector_ways ***)malloc(iw->v.sc * sizeof(t_sector_ways **));
	i = -1;
	while (++i < iw->v.sc)
	{
		iw->ways[i] = (t_sector_ways **)malloc(iw->v.sc * sizeof(t_sector_ways *));
		j = -1;
		while (++j < iw->v.sc)
			iw->ways[i][j] = 0;
	}
	get_sectors_ways2(iw);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void	get_packaging_texture(t_packaging_texture **pack, SDL_Surface *sur)
{
	*pack = (t_packaging_texture *)malloc(sizeof(t_packaging_texture));
	(*pack)->w = sur->w;
	(*pack)->h = sur->h;
	(*pack)->pitch = sur->pitch;
	(*pack)->bpp = sur->format->BytesPerPixel;
	(*pack)->pixels = malloc(sur->pitch * sur->h);
	ft_memcpy((*pack)->pixels, sur->pixels, sur->pitch * sur->h);
	SDL_FreeSurface(sur);
}

void	get_packaging_textures(t_sdl *iw)
{
	int		i;

	get_packaging_texture(&iw->hud.enot, iw->hud.enot_sur);
	get_packaging_texture(&iw->hud.miss, iw->hud.miss_sur);
	get_packaging_texture(&iw->hud.win, iw->hud.win_sur);
	get_packaging_texture(&iw->hud.dead, iw->hud.dead_sur);
	get_packaging_texture(&iw->hud.saved, iw->hud.saved_sur);
	i = -1;
	while (++i < TEXTURES_COUNT)
		get_packaging_texture(&iw->t[i], iw->t_sur[i]);
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_decor[i], iw->t_decor_sur[i]);
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_enemies[i], iw->t_enemies_sur[i]);
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_pickup[i], iw->t_pickup_sur[i]);
	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_weap[i], iw->t_weap_sur[i]);

	get_packaging_texture(&iw->bag.button[0], iw->bag.button_sur[0]);
	get_packaging_texture(&iw->bag.button[1], iw->bag.button_sur[1]);
	get_packaging_texture(&iw->bag.button[2], iw->bag.button_sur[2]);

	i = -1;
	while (++i < 6)
		get_packaging_texture(&iw->menu.icons[i], iw->menu.icons_sur[i]);
	get_packaging_texture(&iw->map.player, iw->map.player_sur);
}

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

void	get_guns_center(t_sdl *iw, int i, int scale)
{
	scale /= 2;
	iw->guns.t_rect[i].w = iw->t_weap[i]->w * WINDOW_W / scale;
	iw->guns.t_rect[i].h = iw->guns.t_rect[i].w * iw->t_weap[i]->h / iw->t_weap[i]->w;
	iw->guns.t_rect[i].x = (WINDOW_W - iw->guns.t_rect[i].w) / 2;
	iw->guns.t_rect[i].y = (WINDOW_H - iw->guns.t_rect[i].h) / 2;
}

void	get_guns_center_down(t_sdl *iw, int i, int scale)
{
	scale /= 2;
	iw->guns.t_rect[i].w = iw->t_weap[i]->w * WINDOW_W / scale;
	iw->guns.t_rect[i].h = iw->guns.t_rect[i].w * iw->t_weap[i]->h / iw->t_weap[i]->w;
	iw->guns.t_rect[i].x = (WINDOW_W - iw->guns.t_rect[i].w) / 2;
	iw->guns.t_rect[i].y = WINDOW_H - iw->guns.t_rect[i].h;
}

void	get_guns(t_sdl *iw)
{
	iw->guns.t = 17;
	iw->guns.max_bullets[0] = 1;
	iw->guns.max_bullets[1] = 4;
	iw->guns.max_bullets[2] = 10;
	iw->guns.max_bullets_in_stock[0] = 0;
	iw->guns.max_bullets_in_stock[1] = 40;
	iw->guns.max_bullets_in_stock[2] = 100;
	iw->guns.bullets[0] = iw->guns.max_bullets[0];
	iw->guns.bullets[1] = iw->guns.max_bullets[1];//0;
	iw->guns.bullets[2] = iw->guns.max_bullets[2];//0;
	iw->guns.bullets_in_stock[0] = 0;
	iw->guns.bullets_in_stock[1] = 8;//iw->guns.max_bullets_in_stock[1];
	iw->guns.bullets_in_stock[2] = 10;// iw->guns.max_bullets_in_stock[2];

	iw->guns.gun_in_hands = 0;
	iw->guns.status = 0;

	get_guns_center_down(iw, 17, 2000);
	get_guns_center_down(iw, 18, 2000);
	get_guns_center_down(iw, 0, 3000);
	iw->guns.t_rect[0].x += iw->guns.t_rect[0].w / 3 * 2;
	get_guns_center_down(iw, 1, 3000);
	iw->guns.t_rect[1].x += iw->guns.t_rect[1].w * 4 / 5;
	get_guns_center(iw, 2, 1700);
	iw->guns.t_rect[2].x += WINDOW_W / 10;
	iw->guns.t_rect[2].y -= WINDOW_H / 5;
	get_guns_center(iw, 3, 1700);
	iw->guns.t_rect[3].x += WINDOW_W / 9;
	iw->guns.t_rect[3].y -= WINDOW_H / 3;
	get_guns_center(iw, 4, 2200);
	iw->guns.t_rect[4].x += WINDOW_W / 8;
	iw->guns.t_rect[4].y -= WINDOW_H / 3;
	get_guns_center(iw, 5, 2200);
	iw->guns.t_rect[5].x += WINDOW_W / 7;
	iw->guns.t_rect[5].y -= WINDOW_H / 5;
	get_guns_center(iw, 6, 2200);
	iw->guns.t_rect[6].x += WINDOW_W / 6;
	iw->guns.t_rect[6].y += WINDOW_H / 10;
	get_guns_center_down(iw, 7, 2500);
	iw->guns.t_rect[7].x += iw->guns.t_rect[7].w * 3 / 5;
	get_guns_center_down(iw, 8, 2500);
	iw->guns.t_rect[8].x += iw->guns.t_rect[8].w * 3 / 5;
	get_guns_center_down(iw, 9, 2500);
	iw->guns.t_rect[9].x += iw->guns.t_rect[9].w * 3 / 5;
	get_guns_center_down(iw, 10, 1500);
	get_guns_center_down(iw, 11, 1500);
	get_guns_center_down(iw, 12, 1500);
	get_guns_center_down(iw, 13, 1500);
	get_guns_center_down(iw, 14, 1500);
	get_guns_center_down(iw, 15, 1500);
	get_guns_center_down(iw, 16, 1500);

	iw->guns.prev_update_time = clock();
}

//void	find_max_min(t_sdl *iw)
//{
//	int sec;
//	int w;
//
//	sec = -1;
//	iw->map.max_x = iw->walls->x;
//	iw->map.max_y = iw->walls->y;
//	iw->map.min_x = iw->walls->x;
//	iw->map.min_y = iw->walls->y;
//    while (++sec < iw->v.sc)
//    {
//        w = iw->sectors[sec].sw - 1;
//        while (++w < iw->sectors[sec].sw + iw->sectors[sec].nw)
//	{
//		iw->map.max_x = ft_max(iw->map.max_x, iw->walls[w].x);
//		iw->map.max_y = ft_max(iw->map.max_y, iw->walls[w].y);
//		iw->map.min_x = ft_min(iw->map.min_x, iw->walls[w].x);
//		iw->map.min_y = ft_min(iw->map.min_y, iw->walls[w].y);
//	}
//	}
//}

void	game_start_menu(t_sdl *iw)
{
	SDL_Rect player;
	SDL_Rect zast;
	SDL_Rect diff;

	player.x = 0;
	player.y = 0;
	player.w = WINDOW_W;
	player.h = WINDOW_H;
	zast.x = WINDOW_W / 10;
	zast.y = WINDOW_H / 8;
	zast.w = WINDOW_W / 4;
	zast.h = WINDOW_H / 8;
	diff.w = WINDOW_W / 6;
	diff.h = WINDOW_H / 10;
	diff.x = zast.x + (zast.w - diff.w) / 2;
	diff.y = zast.y + (zast.h - diff.h) / 2;
	ft_scaled_blit(iw->menu.icons[0], iw->sur, &player);
	if (iw->menu.count == 1)
		ft_scaled_blit(iw->menu.icons[2],iw->sur, &zast);
	else 
		ft_scaled_blit(iw->menu.icons[1], iw->sur, &zast);
	ft_scaled_blit(iw->menu.icons[3], iw->sur, &diff);
	zast.y += zast.h + WINDOW_H / 10;
	diff.y = zast.y + (zast.h - diff.h) / 2;
	if (iw->menu.count == 2)
		ft_scaled_blit(iw->menu.icons[2],iw->sur, &zast);
	else
		ft_scaled_blit(iw->menu.icons[1],iw->sur, &zast);
	ft_scaled_blit(iw->menu.icons[4], iw->sur, &diff);
	zast.y += zast.h + WINDOW_H / 10;
	diff.y = zast.y + (zast.h - diff.h) / 2;
	if (iw->menu.count == 3)
		ft_scaled_blit(iw->menu.icons[2],iw->sur, &zast);
	else 
		ft_scaled_blit(iw->menu.icons[1],iw->sur, &zast);
	ft_scaled_blit(iw->menu.icons[5], iw->sur, &diff);
	SDL_UpdateWindowSurface(iw->win);
}

void	menu_loop(t_sdl *iw)
{
	SDL_Event	e;
	int			quit;

	game_start_menu(iw);
	quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.scancode == 81)
				{
					iw->menu.count += ((iw->menu.count < 3) ? 1 : -2);
					game_start_menu(iw);
				}
				else if (e.key.keysym.scancode == 82)
				{
					iw->menu.count -= ((iw->menu.count > 1) ? 1 : -2);
					game_start_menu(iw);
				}
				else if (e.key.keysym.scancode == 40)
					quit = 1;
				else if (e.key.keysym.scancode == 41)
					exit_x(iw);
			}
			
	}
}

void	image_loop(t_sdl *iw, t_packaging_texture *tex)
{
	SDL_Event	e;
	int			quit;
	SDL_Rect	rect;

	rect.w = WINDOW_W;
	rect.h = WINDOW_H;
	rect.x = 0;
	rect.y = 0;
	ft_scaled_blit2(tex, iw->sur, &rect);
	SDL_UpdateWindowSurface(iw->win);
	quit = 0;
	while (!quit)
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.scancode == 41)
					exit_x(iw);
				else if (e.key.keysym.scancode == 40)
					quit = 1;
			}
}

void	free_sector_ways(t_sdl *iw)
{
	int		i;
	int		j;
	t_sector_ways	*tmp;
	t_sector_way	*tmp2;
	t_sector_ways	*tmp_f;
	t_sector_way	*tmp2_f;

	i = -1;
	while (++i < iw->v.sc)
	{
		j = -1;
		while (++j < iw->v.sc)
		{
			tmp = iw->ways[i][j];
			while (tmp)
			{
				tmp2 = tmp->way_start;
				while (tmp2)
				{
					tmp2_f = tmp2;
					tmp2 = tmp2->next;
					free(tmp2_f);
				}
				tmp_f = tmp;
				tmp = tmp->next;
				free(tmp_f);
			}
		}
		free(iw->ways[i]);
	}
	free(iw->ways);
	
}

// int		main3d(void)
// {
// 	t_sdl	iw;
	
// 	iw.v.game_mode = 1;
// 	get_def(&iw);
// 	read_textures(&iw);
// 	read_sprites_textures(&iw);
// 	read_weapons_textures(&iw);
// 	get_packaging_textures(&iw);
// 	get_kernel_mem(&iw);
// 	get_kernels(&iw);
// 	get_guns(&iw);
// 	//enemy
// 	// add_sprite(&iw, 4700, -900, 0, 9, 0, 0, 0.5f);
// 	// (*iw.sprite)->type = 2;
// 	// (*iw.sprite)->e.enemy_numb = 1;
// 	// (*iw.sprite)->e.health = ENEMY_HEALTH1;
// 	// (*iw.sprite)->e.status = 1;
// 	// (*iw.sprite)->t = iw.t_enemies[9];
// 	// (*iw.sprite)->t_kernel = &iw.k.m_t_enemies[9];

// 	//add_sprite(&iw, 9151, 2272, 0, 20, 1, 0, 0.5f);
// 	// (*iw.sprite)->type = 2;
// 	// (*iw.sprite)->e.enemy_numb = 2;
// 	// (*iw.sprite)->e.health = ENEMY_HEALTH2;
// 	// (*iw.sprite)->e.status = 0;
// 	// (*iw.sprite)->t = iw.t_enemies[20];
// 	// (*iw.sprite)->t_kernel = &iw.k.m_t_enemies[20];
// 	//pickup

// 	// add_sprite(&iw, 7240, 2640, 200, 0, 1, 0, 0.5f);
// 	// add_sprite(&iw, 6520, 2298, 200, 0, 1, 2, 0.5f);

// 	//add_sprite(&iw, 8640, 2200, 200, 1, 1, 0, 0.1f);
// 	// iw.checkpoints = (t_keyb_inp *)malloc(sizeof(t_keyb_inp));
// 	// iw.checkpoints->next = 0;
// 	// ft_memcpy(iw.checkpoints->s, "Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!", 32);
// 	// iw.checkpoints->s_len = 32;
// 	// iw.checkpoints->sprite = *iw.sprite;
// 	// add_sprite(&iw, 7500, 2000, 200, 1, 1, 1, 0.5f);
// 	// add_sprite(&iw, 7500, 3000, 200, 2, 1, 1, 0.5f);
// 	// // add_sprite(&iw, 7500, 3000, 200, 3, 1, 1, 0.5f);

// 	// add_sprite(&iw, 8500, 1500, 200, 3, 1, 1, 0.5f);
// 	// add_sprite(&iw, 9000, 5500, 200, 4, 1, 1, 0.5f);
// 	// add_sprite(&iw, 10000, 500, 200, 5, 1, 1, 0.5f);

// 	// add_sprite(&iw, 9500, 5000, 200, 6, 1, 1, 0.5f);
// 	// add_sprite(&iw, 7500, 3000, 200, 7, 1, 1, 0.5f);
// 	// add_sprite(&iw, 7500, 3000, 200, 8, 1, 1, 0.5f);

// 	// add_sprite(&iw, 8500, 1000, 200, 9, 1, 1, 0.5f);
// 	// add_sprite(&iw, 8000, 5500, 200, 10, 1, 1, 0.5f);

// 	/*add_sprite(&iw,8640,2200,400, 0, 1, 0, 1.0f);
// 	add_sprite(&iw,6520,2298,200, 0, 1, 0, 1.0f);*/


// 	SDL_Init(SDL_INIT_EVERYTHING);
// 	TTF_Init();
// 	iw.arial_font = TTF_OpenFont("fonts/ARIAL.TTF", 24);
// 	SDL_SetRelativeMouseMode(iw.v.mouse_mode);
// 	if (!iw.v.game_mode)
// 		iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
// 			WINDOW_W, WINDOW_H + 300, SDL_WINDOW_SHOWN);
// 	else
// 		iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
// 			WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
// 	//SDL_SetWindowFullscreen(iw.win, SDL_WINDOW_FULLSCREEN_DESKTOP);
// 	//iw.ren = SDL_CreateRenderer(iw.win, -1, 0);
// 	iw.sur = SDL_GetWindowSurface(iw.win);
// 	if (iw.v.game_mode)
// 	{
// 		image_loop(&iw, iw.t[iw.l.story]);
// 		menu_loop(&iw);
// 	}
// 	circle(&iw.hud, FOOTX, FOOTY);
// 	draw_tex_to_select(&iw);
// 	draw_decor_tex_to_select(&iw);
// 	draw_menu(&iw);
// 	// draw
// 	//get_map(&iw);

// 	find_max_min(&iw);

// 	set_sprites_z(&iw);
// 	get_sectors_ways(&iw);
// 	add_picture1(&iw); ///////////////
// 	create_map(&iw);
// 	draw(&iw);
// 	SDL_UpdateWindowSurface(iw.win);
// 	main_loop(&iw);
// 	SDL_FreeSurface(iw.sur);
// 	SDL_DestroyWindow(iw.win);
// 	TTF_Quit();
// 	SDL_Quit();
// 	// system("PAUSE");
// 	return (0);
// }

void	undo_wall_animation(t_sdl *iw, t_picture *pic)
{
	t_wall_animation	*tmp;
	t_wall_animation	*tmp2;
	t_wall_animation	*start;

	if (iw->wall_animations == 0)
		return;
	start = iw->wall_animations;
	if (iw->wall_animations->trigger == pic)
	{
		tmp2 = iw->wall_animations;
		do_wall_animation_step_dx(iw, tmp2, -tmp2->curr_dx);
		do_wall_animation_step_dy(iw, tmp2, -tmp2->curr_dy);
		tmp2->status = 0;
		tmp2->curr_dx = 0;
		tmp2->curr_dy = 0;
		iw->wall_animations = iw->wall_animations->next;
	}
	else
	{
		tmp = iw->wall_animations;
		while (tmp->next)
		{
			if (tmp->next->trigger == pic)
			{
				tmp2 = tmp->next;
				do_wall_animation_step_dx(iw, tmp2, -tmp2->curr_dx);
				do_wall_animation_step_dy(iw, tmp2, -tmp2->curr_dy);
				tmp2->status = 0;
				tmp2->curr_dx = 0;
				tmp2->curr_dy = 0;
				tmp->next = tmp->next->next;
				return;
			}
			tmp = tmp->next;
		}
	}
	iw->wall_animations = start;
}

void	undo_animations2(t_sdl *iw, t_picture *pic)
{
	t_sector_animation	a;
	t_sector_animation	*tmp;
	t_sector_animation	*tmp2;

	a.next = iw->sector_animations;
	tmp = &a;
	while (tmp->next)
	{
		if (tmp->next->trigger == pic)
		{
			tmp2 = tmp->next;
			do_sector_animation_step(iw, tmp2, -tmp2->curr_dy);
			tmp2->curr_dy = 0;
			tmp2->status = 0;
		}
		tmp = tmp->next;
	}
	undo_wall_animation(iw, pic);
}

void	undo_animations(t_sdl *iw)
{
	int		sec;
	int		w;
	t_picture	*tmp;

	sec = -1;
	while (++sec < iw->v.sc)
	{
		w = iw->sectors[sec].sw - 1;
		while (++w < iw->sectors[sec].sw + iw->sectors[sec].nw)
		{
			tmp = iw->walls[w].p;
			while (tmp)
			{
				undo_animations2(iw, tmp);
				tmp = tmp->next;
			}
		}
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
		return;
	}
	tmp = malloc(MAX_MUSIC_SIZE);
	iw->sounds.music_pack_size[i] = read(fd, tmp, MAX_MUSIC_SIZE);
	close(fd);
	iw->sounds.music_pack[i] = malloc(iw->sounds.music_pack_size[i]);
	ft_memcpy(iw->sounds.music_pack[i], tmp, iw->sounds.music_pack_size[i]);
	free(tmp);
	iw->sounds.music_rw[i] = SDL_RWFromConstMem(iw->sounds.music_pack[i], iw->sounds.music_pack_size[i]);
	iw->sounds.music[i] = Mix_LoadMUS_RW(iw->sounds.music_rw[i], 0);
}

void	get_sounds_game(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < MUSIC_COUNT)
	{
		iw->sounds.music_rw[i] = SDL_RWFromConstMem(iw->sounds.music_pack[i], iw->sounds.music_pack_size[i]);
		iw->sounds.music[i] = Mix_LoadMUS_RW(iw->sounds.music_rw[i], 0);
	}
}

void	get_sounds(t_sdl *iw)
{
	SDL_RWops	*d;
	void		*c;
	size_t	size;

	get_music(iw, 0, "sound/background/0.mp3");
	get_music(iw, 1, "sound/background/1.mp3");
	get_music(iw, 2, "sound/background/2.mp3");
	get_music(iw, 3, "sound/background/3.mp3");
	get_music(iw, 4, "sound/background/4.mp3");
	get_music(iw, 5, "sound/background/5.mp3");

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