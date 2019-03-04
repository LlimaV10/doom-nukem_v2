#include "../guardians.h"

void	draw_inclined_floor_ceil_tex1(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_wall_floor_ceil_tex *d)
{
	d->lv.x = (float)(left->p.x - iw->p.x);
	d->lv.y = (float)(left->p.y - iw->p.y);
	d->rv.x = (float)(right->p.x - iw->p.x);
	d->rv.y = (float)(right->p.y - iw->p.y);
	d->ang = acosf((d->lv.x * d->rv.x + d->lv.y * d->rv.y) /
		(sqrtf(d->lv.x * d->lv.x + d->lv.y * d->lv.y) *
			sqrtf(d->rv.x * d->rv.x + d->rv.y * d->rv.y)));
	d->dang = d->ang / (float)(right->x - left->x + 1);
	d->ang = 0.0f;
	d->rv.x = (float)(-right->p.x + left->p.x);
	d->rv.y = (float)(-right->p.y + left->p.y);
	d->sing = G180 - acosf((d->lv.x * d->rv.x + d->lv.y * d->rv.y) /
		(sqrtf(d->lv.x * d->lv.x + d->lv.y * d->lv.y) *
			sqrtf(d->rv.x * d->rv.x + d->rv.y * d->rv.y)));
	d->lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f)
		+ powf(iw->p.y - left->p.y, 2.0f));
	d->len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f)
		+ powf(left->p.y - right->p.y, 2.0f));
	d->rv.x = (float)(right->p.x - left->p.x) / d->len_lr;
	d->rv.y = (float)(right->p.y - left->p.y) / d->len_lr;
	d->zu = get_ceil_z(iw, iw->p.x, iw->p.y);
}

void	draw_inclined_floor_ceil_tex2(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d->frpl = (float)(d->zu - d->zd) / (float)(iw->p.z - d->zd);
	d->clpl = (float)(d->zu - d->zd) / (float)(d->zu - iw->p.z);
	d->px = (float)iw->p.x / 1000.0f;
	d->py = (float)iw->p.y / 1000.0f;
	if (iw->sectors[iw->d.cs].cl.t < 0)
	{
		d->rot = (iw->p.rot - iw->v.angle) + (float)left->x /
			(float)WINDOW_W * iw->v.angle * 2.0f;
		if (d->rot < 0.0f)
			d->rot += G360;
		else if (d->rot > G360)
			d->rot -= G360;
		d->sky_x = d->rot * ((float)iw->t[iw->l.skybox]->w) / G360;
		d->dx = (float)iw->t[iw->l.skybox]->w / (G360 / (iw->v.angle * 2) * WINDOW_W);
		d->dy = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
	}
}

void	draw_inclined_floor_ceil_tex3(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->left_len = sinf(d->ang) * d->lenpl / sinf(d->sing - d->ang);
	d->r.x = (float)left->p.x + d->rv.x * d->left_len;
	d->r.y = (float)left->p.y + d->rv.y * d->left_len;
	d->frcoef = get_ceil_z(iw, d->r.x, d->r.y) - get_floor_z(iw, d->r.x, d->r.y);
	d->clcoef = d->frcoef;
	d->wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a
		* d->r.x + iw->d.screen.b * d->r.y + iw->d.screen.c) /
		iw->d.screen_len);
	d->r.x /= 1000.0f;
	d->r.y /= 1000.0f;
}

void	draw_inclined_floor_ceil_tex4(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) +
		d->frpl * (float)(d->i + 1 - iw->d.wallBot[d->j]);
	while (++d->i < iw->d.bottom[left->x + d->j])
	{
		d->weight = d->wall_dist * d->frcoef / d->k;
		d->k += d->frpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		d->frcoef = (get_ceil_z(iw, d->floor.x * 1000.0f, d->floor.y * 1000.0f) -
			get_floor_z(iw, d->floor.x * 1000.0f, d->floor.y * 1000.0f)
			+ d->frcoef) / 2.0f;
		set_pixel2(iw->sur, left->x + d->j, d->i,
			get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
			((d->floor.x < 0) ? (((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs].
				fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->
				d.cs].fr.t]->w - 1) : ((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.
					cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)), ((d->floor.y < 0) ?
					(((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[
						iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h -
							1) : ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h)
								% iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			), iw->sectors[iw->d.cs].light));
	}
	iw->d.bottom[left->x + d->j] = iw->d.wallBot[d->j];
}

void	draw_inclined_floor_ceil_tex5_1(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	d->k = (float)(iw->d.wallBot[d->j] - iw->d.wallTop[d->j]) +
		d->clpl * (float)(iw->d.wallTop[d->j] - d->i + 1);
	while (--d->i >= iw->d.top[left->x + d->j])
	{
		d->weight = d->wall_dist * d->clcoef / d->k;
		d->k += d->clpl;
		d->floor.x = d->weight * d->r.x + (1.0f - d->weight) * d->px;
		d->floor.y = d->weight * d->r.y + (1.0f - d->weight) * d->py;
		d->clcoef = (get_ceil_z(iw, d->floor.x * 1000.0f, d->floor.y * 1000.0f) -
			get_floor_z(iw, d->floor.x * 1000.0f, d->floor.y
				* 1000.0f) + d->clcoef) / 2.0f;
		set_pixel2(iw->sur, left->x + d->j, d->i,
			get_light_color(get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
			((d->floor.x < 0) ? (((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d.cs]
				.cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w) + iw->t[iw->sectors[iw
				->d.cs].cl.t]->w - 1) : ((int)(d->floor.x * (float)iw->t[iw->sectors[iw->d
					.cs].cl.t]->w) % iw->t[iw->sectors[iw->d.cs].cl.t]->w)), ((d->floor.y < 0)
						? (((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) % iw->
							t[iw->sectors[iw->d.cs].cl.t]->h) + iw->t[iw->sectors[iw->d.cs].cl.t]->h
							- 1) : ((int)(d->floor.y * (float)iw->t[iw->sectors[iw->d.cs].cl.t]->h) %
								iw->t[iw->sectors[iw->d.cs].cl.t]->h))), iw->sectors[iw->d.cs].light));
	}
	iw->d.top[left->x + d->j] = iw->d.wallTop[d->j];
}

void	draw_inclined_floor_ceil_tex5(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	if (!(iw->d.wallTop[d->j] > iw->d.top[left->x + d->j]))
		return;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		if (iw->d.wallTop[d->j] < iw->d.bottom[left->x + d->j])
			d->i = iw->d.wallTop[d->j] + 1;
		else
			d->i = iw->d.bottom[left->x + d->j] + 1;
		draw_inclined_floor_ceil_tex5_1(iw, left, d);
	}
	else
	{
		d->sky_y = -iw->p.rotup + 2 * WINDOW_H;
		d->sky_y = (d->sky_y * (iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
		d->i = iw->d.top[left->x + d->j] - 1;
		d->sky_y += d->dy * iw->d.top[left->x + d->j];
		if (d->sky_x > iw->t[iw->l.skybox]->w)
			d->sky_x = d->sky_x - iw->t[iw->l.skybox]->w;
		while (++d->i < iw->d.wallTop[d->j] && d->i < iw->d.bottom[left->x + d->j])
		{
			set_pixel2(iw->sur, d->j + left->x, d->i,
				get_pixel(iw->t[iw->l.skybox], (int)d->sky_x, (int)d->sky_y));
			d->sky_y += d->dy;
		}
	}
}

void	draw_inclined_floor_ceil_tex6(t_sdl *iw, t_save_wall *left,
	t_draw_wall_floor_ceil_tex *d)
{
	draw_inclined_floor_ceil_tex5(iw, left, d);
	if (iw->sectors[iw->d.cs].cl.t < 0)
		d->sky_x += d->dx;
	d->ang += d->dang;
}

void	draw_inclined_floor_ceil_tex(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex	d;

	draw_inclined_floor_ceil_tex1(iw, left, right, &d);
	draw_inclined_floor_ceil_tex2(iw, left, &d);
	d.j = -1;
	while (++d.j < len)
	{
		if (iw->d.top[left->x + d.j] >= iw->d.bottom[left->x + d.j])
		{
			d.ang += d.dang;
			if (iw->sectors[iw->d.cs].cl.t < 0)
				d.sky_x += d.dx;
			continue;
		}
		draw_inclined_floor_ceil_tex3(iw, left, &d);
		if (iw->d.wallBot[d.j] < iw->d.bottom[left->x + d.j])
		{
			if (iw->d.wallBot[d.j] < iw->d.top[left->x + d.j])
				d.i = iw->d.top[left->x + d.j] - 1;
			else
				d.i = iw->d.wallBot[d.j] - 1;
			draw_inclined_floor_ceil_tex4(iw, left, &d);
		}
		draw_inclined_floor_ceil_tex6(iw, left, &d);
	}
}