#include "../guardians.h"

void	transparency(t_sdl	*iw)
{
	int i;
	int j;
	Uint32 pix;
	int color;

	i = WINDOW_W / 5 - 10;
	while (i++ <= WINDOW_W - (WINDOW_W / 5) + 9)
	{
		j = WINDOW_H / 5 - 10;
		while (j++ <= WINDOW_H - (WINDOW_H / 5) + 9)
		{
			if ((i >= WINDOW_W / 5 - 10 && i <= WINDOW_W / 5) || (j >= WINDOW_H / 5 - 10 && j <= WINDOW_H / 5) || 
				(i <= WINDOW_W - (WINDOW_W / 5) + 10 && i >=  WINDOW_W - (WINDOW_W / 5)) || 
				(j <= WINDOW_H - (WINDOW_H / 5) + 10 && j >=  WINDOW_H - (WINDOW_H / 5)))
				set_pixel2(iw->sur, i, j, 0x999999);
			else
			{
				pix = get_pixel_surface(iw->sur, i, j);
				color = (((int)((float)(pix >> 16) * 0.3)) << 16) +
						(((int)((float)((pix >> 8) - (pix >> 16 << 8)) * 0.3)) << 8) +
						(int)((float)(pix - (pix >> 8 << 8)) * 0.3);
				set_pixel2(iw->sur, i, j, color);
			}
		}
	}
}

void	print_brez_m(t_brez *b, int d, int d1, int d2)
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
		{
			if ((b->x > WINDOW_W / 5 && b->x < (WINDOW_W - WINDOW_W / 5))
				&& (b->y > WINDOW_H / 5 && b->y < (WINDOW_H - WINDOW_H / 5)))
				set_pixel2(b->iw->sur, b->x, b->y, b->color);
		}
		else
		{
			if ((b->x > WINDOW_H / 5 && b->x < (WINDOW_H - WINDOW_H / 5))
				&& (b->y > WINDOW_W / 5 && b->y < (WINDOW_W - WINDOW_W / 5)))
				set_pixel2(b->iw->sur, b->y, b->x, b->color);
		}
		b->x += b->sx;
	}
}

void	bresen(t_sdl *iw, t_draw_line line, int color)
{
	t_brez	b;

	b.iw = iw;
	b.color = color;
	b.sx = (line.x1 >= line.x0) ? (1) : (-1);
	b.sy = (line.y1 >= line.y0) ? (1) : (-1);
	b.dx = (line.x1 > line.x0) ? (line.x1 - line.x0) : (line.x0 - line.x1);
	b.dy = (line.y1 > line.y0) ? (line.y1 - line.y0) : (line.y0 - line.y1);
	if (abs(line.y1 - line.y0) < abs(line.x1 - line.x0))
	{
		b.x = line.x0 + b.sx;
		b.y = line.y0;
		b.k = 1;
		print_brez_m(&b, (b.dy * 2) - b.dx, b.dy * 2, (b.dy - b.dx) * 2);
	}
	else
	{
		b.x = line.y0 + b.sy;
		b.y = line.x0;
		b.sy = (line.x1 >= line.x0) ? (1) : (-1);
		b.sx = (line.y1 >= line.y0) ? (1) : (-1);
		b.k = 0;
		print_brez_m(&b, (b.dx * 2) - b.dy, b.dx * 2, (b.dx - b.dy) * 2);
	}
}

void    draw_minimap(t_sdl *iw)
{
	int			sec;
	int			w;
	int			k;
	t_draw_line	line;

	k = 16000 / WINDOW_W;
	sec = -1;
	while (++sec < iw->v.sc)
	{
		w = iw->sectors[sec].sw - 1;
		while (++w < iw->sectors[sec].sw + iw->sectors[sec].nw)
		{
			if (iw->walls[w].nextsector == -1)
			{
				line.x0 = -iw->walls[w].x / k + iw->p.x / k + 7900 / k;
				line.x0 = WINDOW_W / 2 + ((line.x0 > WINDOW_W / 2) ? -abs(line.x0 - WINDOW_W / 2) : abs(line.x0 - WINDOW_W / 2));
				line.x1 = -iw->walls[w].next->x / k + iw->p.x / k + 7900 / k;
				line.x1 = WINDOW_W / 2 + ((line.x1 > WINDOW_W / 2) ? -abs(line.x1 - WINDOW_W / 2) : abs(line.x1 - WINDOW_W / 2));
				line.y0 = -iw->walls[w].y / k + iw->p.y / k + 4750 / k;
				line.y1 = -iw->walls[w].next->y / k + iw->p.y / k + 4750 / k;
				bresen(iw, line, 0xFF0000);
			}
		}
	}
	ft_scaled_blit(iw->map.player, iw->sur, &iw->map.pl_rect);
}

