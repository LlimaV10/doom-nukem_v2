#include "../guardians.h"

void	add_picture(t_sdl *iw, t_wall *wall)
{
	t_picture	*tmp;

	if (iw->v.submenu_mode != 0 || iw->v.f_button_mode != 0)
		return;
	tmp = (t_picture *)malloc(sizeof(t_picture));
	tmp->left_plus = 500;
	tmp->zu = get_ceil_z(iw, wall->x, wall->y) - 100;
	tmp->tw = 500;
	tmp->t = iw->v.tex_to_fill;
	tmp->next = wall->p;
	wall->p = tmp;
	if (iw->v.tex_to_fill == 17 || iw->v.tex_to_fill == 18)
	{
		iw->v.f_button_mode = 1;
		iw->v.f_button_pointer = (void *)tmp;
	}
	else if (iw->v.tex_to_fill == 19)
	{
		iw->v.submenu_mode = 4;
		draw_submenu(iw);
		iw->v.f_button_pointer = (void *)tmp;
	}
	calculate_picture(iw, wall, tmp);
}

void	delete_wall_animation(t_sdl *iw, t_picture *pic)
{
	t_wall_animation	*tmp;
	t_wall_animation	*tmp2;

	if (iw->wall_animations == 0)
		return;
	if (iw->wall_animations->trigger == pic)
	{
		tmp2 = iw->wall_animations;
		do_wall_animation_step_dx(iw, tmp2, -tmp2->curr_dx);
		do_wall_animation_step_dy(iw, tmp2, -tmp2->curr_dy);
		iw->wall_animations = iw->wall_animations->next;
		free(tmp2);
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
				tmp->next = tmp->next->next;
				free(tmp2);
				return;
			}
			tmp = tmp->next;
		}
	}
}

void	delete_light_and_animations(t_sdl *iw, t_picture *pic)
{
	int		sec;
	t_sector_animation	a;
	t_sector_animation	*tmp;
	t_sector_animation	*tmp2;

	sec = -1;
	while (++sec < iw->v.sc)
		if (iw->sectors[sec].light == pic)
			iw->sectors[sec].light = 0;
	a.next = iw->sector_animations;
	tmp = &a;
	while (tmp->next)
	{
		if (tmp->next->trigger == pic)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			do_sector_animation_step(iw, tmp2, -tmp2->curr_dy);
			free(tmp2);
		}
		else
			tmp = tmp->next;
	}
	iw->sector_animations = a.next;
	delete_wall_animation(iw, pic);
}

void	delete_picture(t_wall *wall, t_picture *pic, t_sdl *iw)
{
	t_picture	*tmp;

	if (iw->v.submenu_mode != 0 || iw->v.f_button_mode != 0)
		return;
	delete_light_and_animations(iw, pic);
	if (pic == wall->p)
	{
		wall->p = wall->p->next;
		free(pic);
	}
	else
	{
		tmp = wall->p;
		while (tmp->next != 0)
		{
			if (tmp->next == pic)
				break;
			tmp = tmp->next;
		}
		if (tmp->next != 0)
		{
			tmp->next = tmp->next->next;
			free(pic);
		}
	}
}