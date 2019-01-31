void	draw_wall(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;
	int		j;

	j = left->x - 1;
	while (++j < left->x + len)
	{
		i = iw->d.top[j] - 1;
		while (++i < iw->d.bottom[j])
			set_pixel(iw->sur, j, i, 0x00FF00);
		iw->d.top[j] = iw->d.bottom[j];
	}
}

void	draw_wall_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;

	/*d.left_len = 0.0f;
	d.tx = left->olen * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
	while (d.tx >= (float)iw->t[left->wall->t]->w)
		d.tx -= (float)iw->t[left->wall->t]->w;*/

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < iw->d.top[j + left->x])
			d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
		i = iw->d.top[left->x + j] - 1;
		while (++i < iw->d.bottom[left->x + j])
		{
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h));
			d.ty += d.dty;
		}
		iw->d.top[left->x + j] = iw->d.bottom[left->x + j];
		d.ang += d.dang;
	}
}

void	draw_floor(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallBot[j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
			continue;
		if (iw->d.wallBot[j] < iw->d.top[left->x + j])
			i = iw->d.top[left->x + j] - 1;
		else
			i = iw->d.wallBot[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0x0000FF);
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
	}
}

void	draw_inclined_floor_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.pl = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallBot[j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.coef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b));
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallBot[j] < iw->d.top[left->x + j])
			i = iw->d.top[left->x + j] - 1;
		else
			i = iw->d.wallBot[j] - 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(i + 1 - iw->d.wallBot[j]);
		while (++i < iw->d.bottom[left->x + j])
		{
			d.weight = d.wall_dist * d.coef / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			d.coef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
				get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.coef) / 2.0f;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		d.ang += d.dang;
	}
}

void	draw_floor_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, 0, 0);
	d.zd = get_floor_z(iw, 0, 0);
	d.coef = d.zu - d.zd;
	d.pl = d.coef / (float)(iw->p.z - d.zd);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallBot[j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b) / d.coef);
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallBot[j] < iw->d.top[left->x + j])
			i = iw->d.top[left->x + j] - 1;
		else
			i = iw->d.wallBot[j] - 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(i + 1 - iw->d.wallBot[j]);
		while (++i < iw->d.bottom[left->x + j])
		{
			d.weight = d.wall_dist / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].fr.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallBot[j] < iw->d.bottom[left->x + j])
			iw->d.bottom[left->x + j] = iw->d.wallBot[j];
		d.ang += d.dang;
	}
}

void	draw_ceil(t_sdl *iw, t_save_wall *left, int len)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallTop[j] <= iw->d.top[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
			continue;
		i = iw->d.top[left->x + j] - 1;
		while (++i < iw->d.wallTop[j] && i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0x00FFFF);
		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
			iw->d.top[left->x + j] = iw->d.wallTop[j];
	}
}

void	draw_inclined_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	d.pl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallTop[j] <= iw->d.top[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.coef = get_ceil_z(iw, d.r.x, d.r.y) - get_floor_z(iw, d.r.x, d.r.y);
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b));
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
			i = iw->d.wallTop[j] + 1;
		else
			i = iw->d.bottom[left->x + j] + 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(iw->d.wallTop[j] - i + 1);
		while (--i >= iw->d.top[left->x + j])
		{
			d.weight = d.wall_dist * d.coef / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			d.coef = (get_ceil_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) -
				get_floor_z(iw, d.floor.x * 1000.0f, d.floor.y * 1000.0f) + d.coef) / 2.0f;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		d.ang += d.dang;
	}
}

void	draw_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_floor_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.rv.x = (float)(right->p.x - left->p.x) / d.len_lr;
	d.rv.y = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, 0, 0);
	d.zd = get_floor_z(iw, 0, 0);
	d.coef = d.zu - d.zd;
	d.pl = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	d.px = (float)iw->p.x / 1000.0f;
	d.py = (float)iw->p.y / 1000.0f;

	j = -1;
	while (++j < len)
	{
		if (iw->d.wallTop[j] <= iw->d.top[left->x + j] ||
			iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.r.x = (float)left->p.x + d.rv.x * d.left_len;
		d.r.y = (float)left->p.y + d.rv.y * d.left_len;
		d.wall_dist = (float)WINDOW_H / (fabsf(iw->d.screen.a * d.r.x + iw->d.screen.b * d.r.y + iw->d.screen.c) /
			sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b)) * d.coef;
		d.r.x /= 1000.0f;
		d.r.y /= 1000.0f;
		if (iw->d.wallTop[j] < iw->d.bottom[left->x + j])
			i = iw->d.wallTop[j] + 1;
		else
			i = iw->d.bottom[left->x + j] + 1;
		d.k = (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) + d.pl * (float)(iw->d.wallTop[j] - i + 1);
		while (--i >= iw->d.top[left->x + j])
		{
			d.weight = d.wall_dist / d.k;
			d.k += d.pl;
			d.floor.x = d.weight * d.r.x + (1.0f - d.weight) * d.px;
			d.floor.y = d.weight * d.r.y + (1.0f - d.weight) * d.py;
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[iw->sectors[iw->d.cs].cl.t],
				((d.floor.x < 0) ? (((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w) + iw->t[iw->sectors[iw->d.cs].fr.t]->w - 1) :
				((int)(d.floor.x * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->w) % iw->t[iw->sectors[iw->d.cs].fr.t]->w)),
					((d.floor.y < 0) ? (((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h) + iw->t[iw->sectors[iw->d.cs].fr.t]->h - 1) :
				((int)(d.floor.y * (float)iw->t[iw->sectors[iw->d.cs].fr.t]->h) % iw->t[iw->sectors[iw->d.cs].fr.t]->h))
			));
		}
		if (iw->d.wallTop[j] > iw->d.top[left->x + j])
			iw->d.top[left->x + j] = iw->d.wallTop[j];
		d.ang += d.dang;
	}
}

void	draw_between_sectors_bot(t_sdl *iw, t_save_wall *left, int len, int *tmp)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.bottom[left->x + j] <= tmp[j])
			continue;
		i = tmp[j] - 1;
		while (++i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0xFF0000);
		iw->d.bottom[left->x + j] = tmp[j];
	}
}

void	draw_between_sectors_top(t_sdl *iw, t_save_wall *left, int len, int *tmp)
{
	int		i;
	int		j;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j] ||
			iw->d.top[left->x + j] >= tmp[j])
			continue;
		i = iw->d.top[left->x + j] - 1;
		while (++i < tmp[j] && i < iw->d.bottom[left->x + j])
			set_pixel(iw->sur, left->x + j, i, 0xFF0000);
		iw->d.top[left->x + j] = tmp[j];
	}
}

void	draw_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	int		cint[16];
	float	cfloat[15];

	cint[0] = 0;
	cint[1] = 0;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	else
	{
		cint[4] = iw->t[iw->l.skybox]->w;
		cint[5] = iw->t[iw->l.skybox]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[12] = iw->d.screen_left;
	cint[13] = iw->d.screen_right;
	cint[14] = iw->sectors[iw->d.cs].cl.t;
	cint[15] = iw->p.rotup;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = iw->p.rot;
	cfloat[14] = iw->v.angle;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[0] = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	cfloat[2] = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[1] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	cfloat[3] = (float)(right->p.x - left->p.x) / d.len_lr;
	cfloat[4] = (float)(right->p.y - left->p.y) / d.len_lr;

	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);

	cint[11] = d.zu - d.zd;
	cfloat[9] = (float)cint[11] / ((float)iw->p.z - (float)d.zd);
	cfloat[10] = (float)cint[11] / ((float)d.zu - (float)iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	// m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 14 * sizeof(int), NULL, &iw->k.ret);
	// m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 13 * sizeof(float), NULL, &iw->k.ret);

	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 16 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 15 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fc_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->l.skybox]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_cint);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);



	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
	//printf("kernel run ret %d\n", iw->k.ret);
		// clReleaseMemObject(m_wallTop);
		// clReleaseMemObject(m_wallBot);
		// clReleaseMemObject(m_cint);
		// clReleaseMemObject(m_cfloat);
}

void	draw_inclined_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	int		cint[23];
	float	cfloat[15];

	cint[0] = 0;
	cint[1] = 0;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	else
	{
		cint[4] = iw->t[iw->l.skybox]->w;
		cint[5] = iw->t[iw->l.skybox]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[19] = iw->d.screen_left;
	cint[20] = iw->d.screen_right;
	cint[21] = iw->sectors[iw->d.cs].cl.t;
	cint[22] = iw->p.rotup;

	if (iw->sectors[iw->d.cs].fr.n == 0)
	{
		cint[11] = 0;
		cint[12] = 0;
		cint[13] = -1;
		cint[14] = iw->sectors[iw->d.cs].fr.z;
	}
	else
	{
		cint[11] = iw->sectors[iw->d.cs].fr.n->a;
		cint[12] = iw->sectors[iw->d.cs].fr.n->b;
		cint[13] = iw->sectors[iw->d.cs].fr.n->c;
		cint[14] = iw->sectors[iw->d.cs].fr.n->d;
	}
	if (iw->sectors[iw->d.cs].cl.n == 0)
	{
		cint[15] = 0;
		cint[16] = 0;
		cint[17] = -1;
		cint[18] = iw->sectors[iw->d.cs].cl.z;
	}
	else
	{
		cint[15] = iw->sectors[iw->d.cs].cl.n->a;
		cint[16] = iw->sectors[iw->d.cs].cl.n->b;
		cint[17] = iw->sectors[iw->d.cs].cl.n->c;
		cint[18] = iw->sectors[iw->d.cs].cl.n->d;
	}

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = iw->p.rot;
	cfloat[14] = iw->v.angle;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[0] = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	cfloat[2] = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[1] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	cfloat[3] = (float)(right->p.x - left->p.x) / d.len_lr;
	cfloat[4] = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	cfloat[9] = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	cfloat[10] = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	// m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 21 * sizeof(int), NULL, &iw->k.ret);
	// m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 13 * sizeof(float), NULL, &iw->k.ret);

	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 23 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 15 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fci_ret %d\n", iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->l.skybox]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);


	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
	//printf("kernel run ret %d\n", iw->k.ret);
		// clReleaseMemObject(m_wallTop);
		// clReleaseMemObject(m_wallBot);
		// clReleaseMemObject(m_cint);
		// clReleaseMemObject(m_cfloat);
}

//int
	//0 - wall_width - NULL
	//1 - wall_height - NULL
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - frcoef
//12 - screen_left
//13 - screen_right

//14 - ceil->t
//15 - rotup

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py

//13 - p.rot
//14 - v.angle


// top && bottom + left->x

__kernel void draw_floor_ceil_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;

	float	rot;
	float	sky_x;
	float	sky_y;
	float	dy;

	//printf("5");
	j = get_global_id(0);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[12] ||
		cint[8] + j >= cint[13])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]) * (float)cint[11];
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}

	if (cint[14] < 0)
	{
		rot = (cfloat[13] - cfloat[14]) + (float)(cint[8] + j) /
			(float)cint[6] * cfloat[14] * 2.0f;
		if (rot < 0.0f)
			rot += 6.2831852f;
		else if (rot > 6.2831852f)
			rot -= 6.2831852f;
		sky_x = rot * (float)cint[4] / 6.2831852f;
		dy = (float)cint[5] / (float)(4 * cint[7]);
	}

	if (wallTop[j] > top[j])
	{
		if (cint[14] >= 0)
		{
			if (wallTop[j] < bottom[j])
				i = wallTop[j] + 1;
			else
				i = bottom[j] + 1;
			k = (float)(wallBot[j] - wallTop[j]) +
				cfloat[10] * (float)(wallTop[j] - i + 1);
			while (--i >= top[j])
			{
				weight = wall_dist / k;
				k += cfloat[10];
				floorx = weight * rx + (1.0f - weight) * cfloat[11];
				floory = weight * ry + (1.0f - weight) * cfloat[12];
				tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
					+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
				wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
			}
		}
		else
		{
			sky_y = ((-cint[15] + 2 * cint[7]) * cint[5]) / (4 * cint[7]) +
				dy * top[j];
			i = top[j] - 1;
			while (++i < wallTop[j] && i < bottom[j])
			{
				tp = (int)sky_x * 3 + (int)sky_y * 3 * cint[4];
				wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
				sky_y += dy;
			}
		}
		top[j] = wallTop[j];
	}

	/*if (bottom[j] > bottom_betw[j] || top[j] < top_betw[j])
	{
		tx = (cfloat[13] + left_len) *
			(float)cint[0] * cfloat[14] / 1000.0f;
		dty = ((float)(cfloat[15] - cfloat[16]) *
			(float)cint[1] / 1000.0f) /
			(float)(wallBot[j] - wallTop[j]) * cfloat[14];
	}

	if (bottom[j] > bottom_betw[j])
	{
		if (wallTop[j] < bottom_betw[j])
			ty = cfloat[15] + cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(bottom_betw[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = ty * (float)cint[1] / 1000.0f;

		i = bottom_betw[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		bottom[j] = bottom_betw[j];
	}

	if (top[j] < top_betw[j])
	{
		if (wallTop[j] < top_betw[j])
			ty = cfloat[15] + cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = ty * (float)cint[1] / 1000.0f;
		i = top[j] - 1;
		while (++i < top_betw[j] && i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		top[j] = top_betw[j];
	}*/
}

//int
	//0 - wall_width - NULL
	//1 - wall_height - NULL
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - floorA
//12 - floorB
//13 - floorC
//14 - floorD
//15 - ceilA
//16 - ceilB
//17 - ceilC
//18 - ceilD
	//19 - left_zu
	//20 - left_zd
//21 - screen_left - 19
//22 - screen_right - 20

//14 - ceil->t - 21
//15 - rotup - 22


//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py

//13 - p.rot
//14 - v.angle

// top && bottom + left->x

__kernel void draw_inclined_floor_ceil_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	int		frcoef;
	int		clcoef;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;

	float	rot;
	float	sky_x;
	float	sky_y;
	float	dy;

	//printf("6");
	j = get_global_id(0);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[19] ||
		cint[8] + j >= cint[20])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	frcoef = (cint[15] * (int)rx + cint[16] * (int)ry + cint[18]) / cint[17] * -1 -
		(cint[11] * (int)rx + cint[12] * (int)ry + cint[14]) / cint[13] * -1;
	clcoef = frcoef;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]);
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist * frcoef / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			frcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
				(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + frcoef) / 2.0f;
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}

	if (cint[21] < 0)
	{
		rot = (cfloat[13] - cfloat[14]) + (float)(cint[8] + j) /
			(float)cint[6] * cfloat[14] * 2.0f;
		if (rot < 0.0f)
			rot += 6.2831852f;
		else if (rot > 6.2831852f)
			rot -= 6.2831852f;
		sky_x = rot * (float)cint[4] / 6.2831852f;
		dy = (float)cint[5] / (float)(4 * cint[7]);
	}

	if (wallTop[j] > top[j])
	{
		if (cint[21] >= 0)
		{
			if (wallTop[j] < bottom[j])
				i = wallTop[j] + 1;
			else
				i = bottom[j] + 1;
			k = (float)(wallBot[j] - wallTop[j]) +
				cfloat[10] * (float)(wallTop[j] - i + 1);
			while (--i >= top[j])
			{
				weight = wall_dist * clcoef / k;
				k += cfloat[10];
				floorx = weight * rx + (1.0f - weight) * cfloat[11];
				floory = weight * ry + (1.0f - weight) * cfloat[12];
				clcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
					(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + clcoef) / 2.0f;
				tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
					+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
				wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
			}
		}
		else
		{
			sky_y = ((-cint[22] + 2 * cint[7]) * cint[5]) / (4 * cint[7]) +
				dy * top[j];
			i = top[j] - 1;
			while (++i < wallTop[j] && i < bottom[j])
			{
				tp = (int)sky_x * 3 + (int)sky_y * 3 * cint[4];
				wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
				sky_y += dy;
			}
		}
		top[j] = wallTop[j];
	}
}