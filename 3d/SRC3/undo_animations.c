#include "../guardians.h"

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