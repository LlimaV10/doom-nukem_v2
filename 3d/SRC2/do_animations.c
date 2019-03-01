#include "../guardians.h"

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