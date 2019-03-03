#include "../guardians.h"

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