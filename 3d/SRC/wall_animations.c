#include "../guardians.h"

void	draw_selected_walls_to_be_animated(t_sdl *iw)
{
	t_save_wall	*tmp;
	int			i;
	int			y;

	tmp = *(iw->vw_save);
	while (tmp)
	{
		i = -1;
		while (++i < iw->v.wall_anim->count_walls)
			if (iw->walls[iw->v.wall_anim->walls[i]].next == tmp->wall ||
				iw->walls[iw->v.wall_anim->walls[i]].next->next == tmp->wall)
			{
				y = 0;
				while (++y < WINDOW_H)
					set_pixel2(iw->sur, tmp->x, y, 0xFF0000);
				break;
			}
		tmp = tmp->next;
	}
}

void	add_wall_to_wall_animation2(t_sdl *iw, int add_wall)
{
	int		i;
	t_wall_animation	*tmp;

	tmp = iw->v.wall_anim;
	i = -1;
	while (++i < tmp->count_walls)
		if (add_wall == tmp->walls[i]/* || *(iw->v.look_wall) == iw->walls[tmp->walls[i]].next
			|| (*(iw->v.look_wall))->next == &iw->walls[tmp->walls[i]]*/)
			return;
	tmp->walls[tmp->count_walls] = add_wall;
	tmp->count_walls++;
	if (tmp->count_walls == COUNT_WALLS_TO_ANIM)
	{
		iw->v.f_button_mode = 0;
		iw->v.f_button_pointer = 0;
		iw->v.submenu_mode = 5;
		draw_submenu(iw);
	}
}

void	add_wall_to_wall_animation(t_sdl *iw)
{
	t_wall_animation	*tmp;
	int					add_wall;

	add_wall = get_wall_by_pointer(iw, *(iw->v.look_sector), *(iw->v.look_wall));
	if (iw->walls[add_wall].nextsector != -1 ||
		iw->walls[add_wall].next->nextsector != -1 ||
		iw->walls[add_wall].next->next->nextsector != -1)
		return;
	if (iw->v.wall_anim == 0)
	{
		tmp = (t_wall_animation *)malloc(sizeof(t_wall_animation));
		tmp->dx = 0;
		tmp->dy = 0;
		tmp->curr_dx = 0;
		tmp->curr_dy = 0;
		tmp->speed = 1;
		tmp->status = 0;
		tmp->count_walls = 1;
		tmp->walls[0] = add_wall;
		tmp->trigger = (t_picture *)iw->v.f_button_pointer;
		iw->v.wall_anim = tmp;
	}
	else
		add_wall_to_wall_animation2(iw, add_wall);
}

void	calculate_pictures_list(t_sdl *iw, t_wall *wall, t_picture *p)
{
	while (p)
	{
		calculate_picture(iw, wall, p);
		p = p->next;
	}
}

void	do_wall_animation_step_dx(t_sdl *iw, t_wall_animation *a, int dx)
{
	int		i;

	i = -1;
	while (++i < a->count_walls)
	{
		if (a->moving_type == 1 &&
			iw->walls[a->walls[i]].next->next->y > iw->walls[a->walls[i]].next->y)
		{
			iw->walls[a->walls[i]].next->x -= dx;
			iw->walls[a->walls[i]].next->next->x -= dx;
		}
		else
		{
			iw->walls[a->walls[i]].next->x += dx;
			iw->walls[a->walls[i]].next->next->x += dx;
		}
		get_wall_line2(&iw->walls[a->walls[i]]);
		get_wall_line2(iw->walls[a->walls[i]].next);
		get_wall_line2(iw->walls[a->walls[i]].next->next);
		calculate_pictures_list(iw, &iw->walls[a->walls[i]], iw->walls[a->walls[i]].p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next, iw->walls[a->walls[i]].next->p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next->next, iw->walls[a->walls[i]].next->next->p);
	}
}

void	do_wall_animation_step_dy(t_sdl *iw, t_wall_animation *a, int dy)
{
	int		i;

	i = -1;
	while (++i < a->count_walls)
	{
		if (a->moving_type == 2 &&
			iw->walls[a->walls[i]].next->next->x > iw->walls[a->walls[i]].next->x)
		{
			iw->walls[a->walls[i]].next->y -= dy;
			iw->walls[a->walls[i]].next->next->y -= dy;
		}
		else
		{
			iw->walls[a->walls[i]].next->y += dy;
			iw->walls[a->walls[i]].next->next->y += dy;
		}
		get_wall_line2(&iw->walls[a->walls[i]]);
		get_wall_line2(iw->walls[a->walls[i]].next);
		get_wall_line2(iw->walls[a->walls[i]].next->next);
		calculate_pictures_list(iw, &iw->walls[a->walls[i]], iw->walls[a->walls[i]].p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next, iw->walls[a->walls[i]].next->p);
		calculate_pictures_list(iw, iw->walls[a->walls[i]].next->next, iw->walls[a->walls[i]].next->next->p);
	}
}

void	exit_editing_wall_animation(t_sdl *iw)
{
	///////////////////////
	if (iw->v.submenu_mode >= 7)
	{
		do_wall_animation_step_dx(iw, iw->v.wall_anim, -iw->v.wall_anim->dx);
		do_wall_animation_step_dy(iw, iw->v.wall_anim, -iw->v.wall_anim->dy);
	}
	free(iw->v.wall_anim);
	iw->v.wall_anim = 0;
	iw->v.submenu_mode = 0;
	draw_submenu(iw);
}

void	change_wall_animation_status(t_sdl *iw, t_picture *p)
{
	t_wall_animation	*tmp;

	tmp = iw->wall_animations;
	while (tmp)
	{
		if (tmp->trigger == p)
		{
			tmp->status = ((tmp->status == 0) ? 1 : 0);
			return;
		}
		tmp = tmp->next;
	}
}