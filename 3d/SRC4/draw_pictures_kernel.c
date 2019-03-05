#include "../guardians.h"

int		draw_picture_kernel2(t_sdl *iw, t_picture *pic, t_draw_picture *d)
{
	if (iw->sectors[iw->d.cs].light == 0 || iw->sectors[iw->d.cs].light->t != 18)
		d->cint[8] = 1;
	else
		d->cint[8] = 0;
	d->cint[1] = iw->t[pic->t]->h;
	d->cint[4] = iw->t[pic->t]->w;
	d->cint[5] = WINDOW_W;
	d->cint[6] = iw->t[pic->t]->bpp;
	d->cint[7] = iw->t[pic->t]->pitch;
	d->lang = get_vectors_angle(iw->d.left_point.x -
		(float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	d->rang = get_vectors_angle(iw->d.right_point.x -
		(float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	if (d->rang < iw->v.angle * 2)
		d->rx0 = (int)(d->lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		return (0);
	d->lang = get_vectors_angle(iw->d.left_point.x -
		(float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	d->rang = get_vectors_angle(iw->d.right_point.x -
		(float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	return (1);
}

int		draw_picture_kernel3(t_sdl *iw, t_picture *pic, t_draw_picture *d)
{
	if (d->rang < iw->v.angle * 2)
		d->rx1 = (int)(d->lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d->rx1 = -(int)(d->lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	if (d->rx1 >= WINDOW_W)
		return (0);
	d->plen = fabsf(iw->d.screen.a * (float)pic->x0 + iw->d.screen.b *
		(float)pic->y0 + iw->d.screen.c) / sqrtf(iw->d.screen.a *
			iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	d->ry0_up = WINDOW_H * (iw->p.z + (int)d->plen / 2 - pic->zu)
		/ (int)(d->plen + 1) + iw->p.rotup;
	d->ry0_down = WINDOW_H * (iw->p.z + (int)d->plen / 2 - pic->zd)
		/ (int)(d->plen + 1) + iw->p.rotup;
	d->plen = fabsf(iw->d.screen.a * (float)pic->x1 +
		iw->d.screen.b * (float)pic->y1 + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	d->cint[3] = WINDOW_H * (iw->p.z + (int)d->plen / 2 - pic->zu)
		/ (int)(d->plen + 1) + iw->p.rotup;
	d->cint[2] = WINDOW_H * (iw->p.z + (int)d->plen / 2 - pic->zd)
		/ (int)(d->plen + 1) + iw->p.rotup;
	d->cfloat[0] = 0.0f;
	d->down = d->ry0_down - d->cint[2];
	d->up = d->ry0_up - d->cint[3];
	d->dx = d->rx0 - d->rx1;
	d->cint[0] = d->rx1;
	return (1);
}

void	draw_picture_kernel4(t_sdl *iw, t_picture *pic, t_draw_picture *d)
{
	d->cfloat[2] = d->down / d->dx;
	d->cfloat[3] = d->down / d->dx;
	d->cfloat[4] = d->up / d->dx;
	d->cfloat[5] = d->up / d->dx;
	d->cfloat[1] = (float)iw->t[pic->t]->w / d->dx;
	if (d->cint[0] < 0)
	{
		d->cfloat[0] += d->cfloat[1] * (float)abs(d->cint[0]);
		d->cfloat[2] += d->cfloat[3] * (float)abs(d->cint[0]);
		d->cfloat[4] += d->cfloat[5] * (float)abs(d->cint[0]);
		d->cint[0] = 0;
	}
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint,
		CL_TRUE, 0, 9 * sizeof(int), d->cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat,
		CL_TRUE, 0, 6 * sizeof(float), d->cfloat, 0, NULL, NULL);
	iw->k.ret = clSetKernelArg(iw->k.kernel6, 3, sizeof(cl_mem),
		(void *)&iw->k.m_t[pic->t]);
}

int		draw_picture_kernel(t_sdl *iw, t_picture *pic)
{
	t_draw_picture	d;

	if (!draw_picture_kernel2(iw, pic, &d))
		return (0);
	if (!draw_picture_kernel3(iw, pic, &d))
		return (0);
	draw_picture_kernel4(iw, pic, &d);
	if (d.rx0 < WINDOW_W)
		d.global_item_size = d.rx0 - d.cint[0];
	else
		d.global_item_size = WINDOW_W - d.cint[0];
	d.local_item_size = 1;
	if (d.global_item_size <= WINDOW_W)
		iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel6, 1,
			NULL, &d.global_item_size, &d.local_item_size, 0, NULL, NULL);
	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	if (d.rx1 <= WINDOW_W / 2 && d.rx0 >= WINDOW_W / 2)
		return (1);
	return (0);
}

void	draw_pictures_kernel(t_sdl *iw, t_save_wall *left)
{
	t_picture	*pic;

	pic = left->wall->p;
	while (pic != 0)
	{
		if (draw_picture_kernel(iw, pic) && *(iw->v.look_picture) == 0)
			*(iw->v.look_picture) = pic;
		pic = pic->next;
	}
}