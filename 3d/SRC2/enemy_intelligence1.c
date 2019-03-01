#include "../guardians.h"

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