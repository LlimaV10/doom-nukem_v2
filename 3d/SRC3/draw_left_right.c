#include "../guardians.h"

void	draw_all(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	if (left->wall->nextsector == -1)
	{
		if (left->wall->p != 0)
		{
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

void	draw_left_right2(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	if (*(iw->v.look_wall) == 0 && iw->v.mouse_mode == 1
		&& left->x < WINDOW_W / 2 && right->x > WINDOW_W / 2
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
}

void	draw_left_right(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_draw_line		l;

	if (left->x >= right->x)
		return;
	iw->d.wallTop = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	iw->d.wallBot = (int *)malloc((right->x - left->x + 1) * sizeof(int));
	if (!iw->d.wallTop || !iw->d.wallBot)
		return;
	l.x0 = left->x;
	l.x1 = right->x;
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zd)
		/ (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zd)
		/ (int)right->plen + iw->p.rotup;
	brez_line(iw->d.wallBot, l);
	l.y0 = WINDOW_H * (iw->p.z + (int)left->plen / 2 - left->zu)
		/ (int)left->plen + iw->p.rotup;
	l.y1 = WINDOW_H * (iw->p.z + (int)right->plen / 2 - right->zu)
		/ (int)right->plen + iw->p.rotup;
	brez_line(iw->d.wallTop, l);
	draw_left_right2(iw, left, right);
	free(iw->d.wallBot);
	free(iw->d.wallTop);
}