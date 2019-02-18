#include "guardians.h"

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
		if (b->k > 0 )
		{
			if((b->x > WINDOW_W / 5 &&  b->x < (WINDOW_W - WINDOW_W / 5))
				&& (b->y > WINDOW_H / 5 &&  b->y < (WINDOW_H - WINDOW_H / 5)))
			set_pixel(b->iw->sur, b->x, b->y, b->color);
		}
		else
		{
			if((b->x > WINDOW_H / 5 &&  b->x < (WINDOW_H- WINDOW_H / 5))
				&& (b->y > WINDOW_W / 5 &&  b->y < (WINDOW_W- WINDOW_W / 5)))
			set_pixel(b->iw->sur, b->y, b->x, b->color);
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
    int sec;
    int w;
	int k;
    t_draw_line line;
	// iw->map.min_x = 0;
	// iw->map.max_x = 10000;
	// iw->map.min_y = -5000;
	// iw->map.max_y = 6000;
	k = ft_max((abs(iw->map.min_x)+ abs(iw->map.max_x))/WINDOW_W, (abs(iw->map.min_y)+ abs(iw->map.max_y))/WINDOW_H);
	if (iw->map.max_x > 0 && iw->map.max_x < 0)
		iw->map.mid_x = (iw->map.max_x - iw->map.min_x)/2 - abs(iw->map.min_x);
	else
		iw->map.mid_x = (iw->map.max_x - iw->map.min_x)/2;
	if (iw->map.max_y > 0 && iw->map.max_y < 0)
		iw->map.mid_y = (iw->map.max_y - iw->map.min_y)/2 - abs(iw->map.min_y);
	else
		iw->map.mid_y = (iw->map.max_y - iw->map.min_y)/2;
    sec = -1;
    while (++sec < iw->v.sc)
    {
        w = iw->sectors[sec].sw - 1;
        while (++w < iw->sectors[sec].sw + iw->sectors[sec].nw)
        {
			if(iw->walls[w].nextsector == -1)
			{
				line.x0 = iw->walls[w].x/k + iw->map.min_x/k+iw->map.mid_x/k +(iw->map.mid_x/k - iw->p.x/k);
           	 	line.y0 = -iw->walls[w].y/k + iw->map.max_y/k+iw->map.mid_y/k - (iw->map.mid_y/k - iw->p.y/k);
            	line.x1 = iw->walls[w].next->x/k + iw->map.min_x/k+iw->map.mid_x/k + (iw->map.mid_x/k - iw->p.x/k);
            	line.y1 = -iw->walls[w].next->y/k + iw->map.max_y/k+iw->map.mid_y/k - (iw->map.mid_y/k - iw->p.y/k);
            bresen(iw,line, 0xFF0000);
			}
        }
    }
	SDL_Surface *icon = SDL_LoadBMP("textures/player.bmp");
	SDL_Rect player = {WINDOW_W/2 -10, WINDOW_H/2 - 10, 20, 20};
	SDL_BlitSurface(icon, NULL ,iw->sur, &player);
	SDL_FreeSurface(icon);
}

