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