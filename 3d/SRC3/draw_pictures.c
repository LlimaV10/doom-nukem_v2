#include "../guardians.h"

int		draw_picture(t_sdl *iw, t_picture *pic)
{
	t_draw_picture	d;
	int		i;
	int		j;

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx0 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d.rx0 = -(int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx1 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d.rx1 = -(int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));

	d.plen = fabsf(iw->d.screen.a * (float)pic->x0 + iw->d.screen.b * (float)pic->y0 + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	d.ry0_up = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	d.ry0_down = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	d.plen = fabsf(iw->d.screen.a * (float)pic->x1 + iw->d.screen.b * (float)pic->y1 + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);

	d.ry1_up = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	d.ry1_down = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	d.pic_x = 0;
	d.down = d.ry0_down - d.ry1_down;
	d.up = d.ry0_up - d.ry1_up;
	d.dx = d.rx0 - d.rx1;
	d.dy_down = d.down / d.dx;
	d.dy_up = d.up / d.dx;
	i = d.rx1;
	if (i < 0)
	{
		d.pic_x += iw->t[pic->t]->w * abs(i) / d.dx;
		d.dy_down += (float)(d.down / d.dx * abs(i));
		d.dy_up += (float)(d.up / d.dx * abs(i));
		i = 0;
	}
	while (i++ <= d.rx0 && i <= WINDOW_W)
	{
		if (iw->d.top_save[i] >= iw->d.bottom_save[i])
			d.pic_x += iw->t[pic->t]->w / d.dx;
		else
		{
			d.dy_plus = iw->t[pic->t]->h / ((d.ry1_down + (float)d.dy_down) - (d.ry1_up + (float)d.dy_up));
			j = d.ry1_up + (float)d.dy_up;
			if (j >= iw->d.top_save[i])
				d.pic_y = 0;
			else
			{
				d.pic_y = d.dy_plus * (iw->d.top_save[i] - j);
				j = iw->d.top_save[i];
			}
			while (j++ < d.ry1_down + (float)d.dy_down && j < iw->d.bottom_save[i])
			{
				d.pixel = get_pixel(iw->t[pic->t], (int)d.pic_x, (int)d.pic_y);
				if (d.pixel != 0x010000)
					set_pixel2(iw->sur, i, j, get_light_color(d.pixel,
						iw->sectors[iw->d.cs].light));
				d.pic_y += d.dy_plus;
			}
			d.pic_x += iw->t[pic->t]->w / d.dx;
			d.dy_down += (float)(d.down / d.dx);
			d.dy_up += (float)(d.up / d.dx);
		}
	}
	if (d.rx1 <= WINDOW_W / 2 && d.rx0 >= WINDOW_W / 2)
		return (1);
	return (0);
}

void	draw_pictures(t_sdl *iw, t_save_wall *left)
{
	t_picture	*pic;

	pic = left->wall->p;
	while (pic != 0)
	{
		if (draw_picture(iw, pic) && *(iw->v.look_picture) == 0)
			*(iw->v.look_picture) = pic;
		pic = pic->next;
	}
}