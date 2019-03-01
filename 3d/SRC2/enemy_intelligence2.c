#include "../guardians.h"

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