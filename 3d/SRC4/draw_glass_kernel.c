#include "../guardians.h"

void	draw_glass_tex_kernel2(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_wall_floor_ceil_tex_kernel *d)
{
	if (iw->sectors[iw->d.cs].light == 0 || iw->sectors[iw->d.cs].light->t != 18)
		d->cint[6] = 1;
	else
		d->cint[6] = 0;
	d->cint[1] = iw->t[left->wall->glass]->w;
	d->cint[2] = iw->t[left->wall->glass]->h;
	d->lv.x = (float)(left->p.x - iw->p.x);
	d->lv.y = (float)(left->p.y - iw->p.y);
	d->rv.x = (float)(right->p.x - iw->p.x);
	d->rv.y = (float)(right->p.y - iw->p.y);
	d->ang = acosf((d->lv.x * d->rv.x + d->lv.y * d->rv.y) /
		(sqrtf(d->lv.x * d->lv.x + d->lv.y * d->lv.y) *
			sqrtf(d->rv.x * d->rv.x + d->rv.y * d->rv.y)));
	d->cfloat[0] = d->ang / (float)(right->x - left->x + 1);
	d->rv.x = (float)(-right->p.x + left->p.x);
	d->rv.y = (float)(-right->p.y + left->p.y);
	d->cfloat[2] = G180 - acosf((d->lv.x * d->rv.x + d->lv.y * d->rv.y) /
		(sqrtf(d->lv.x * d->lv.x + d->lv.y * d->lv.y) *
			sqrtf(d->rv.x * d->rv.x + d->rv.y * d->rv.y)));
	d->cfloat[1] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) +
		powf(iw->p.y - left->p.y, 2.0f));
	d->len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) +
		powf(left->p.y - right->p.y, 2.0f));
	d->cint[0] = iw->d.cs;
	iw->d.cs = left->wall->nextsector;
}

void	draw_glass_tex_kernel3(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_wall_floor_ceil_tex_kernel *d)
{
	d->cint[3] = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x,
		iw->walls[left->wall->nextsector_wall].next->y);
	d->cint[4] = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x,
		iw->walls[left->wall->nextsector_wall].next->y);
	if (left->zu < d->cint[3])
	{
		d->cfloat[5] = (right->zu - left->zu) / d->len_lr;
		d->cint[3] = left->zu;
	}
	else
	{
		d->cint[5] = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x,
			iw->walls[left->wall->nextsector_wall].y);
		d->cfloat[5] = (d->cint[5] - d->cint[3]) / d->len_lr;
	}
}

void	draw_glass_tex_kernel4(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, t_draw_wall_floor_ceil_tex_kernel *d)
{
	if (left->zd > d->cint[4])
	{
		d->cfloat[6] = (right->zd - left->zd) / d->len_lr;
		d->cint[4] = left->zd;
	}
	else
	{
		d->cint[5] = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x,
			iw->walls[left->wall->nextsector_wall].y);
		d->cfloat[6] = (d->cint[5] - d->cint[4]) / d->len_lr;
	}
	iw->d.cs = d->cint[0];
	d->cint[0] = left->x;
	d->cint[5] = WINDOW_W;
	d->cfloat[3] = left->olen;
	d->cfloat[4] = iw->tsz[left->wall->glass];
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE,
		0, (right->x - left->x + 1) * sizeof(int),
		iw->d.save_top_betw, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE,
		0, (right->x - left->x + 1) * sizeof(int),
		iw->d.save_bot_betw, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE,
		0, 7 * sizeof(int), d->cint, 0, NULL, NULL);
}

void	draw_glass_tex_kernel(t_sdl *iw, t_save_wall *left,
	t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;

	draw_glass_tex_kernel2(iw, left, right, &d);
	draw_glass_tex_kernel3(iw, left, right, &d);
	draw_glass_tex_kernel4(iw, left, right, &d);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE,
		0, 7 * sizeof(float), d.cfloat, 0, NULL, NULL);
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 3, sizeof(cl_mem),
		(void *)&iw->k.m_t[left->wall->glass]);
	d.global_item_size = len;
	d.local_item_size = 1;
	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel5, 1, NULL,
		&d.global_item_size, &d.local_item_size, 0, NULL, NULL);
	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
}