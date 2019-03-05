/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_skybox_kernel.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 14:38:19 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 14:38:20 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_skybox_kernel2(t_sdl *iw, int *cint, float *cfloat)
{
	cint[0] = WINDOW_H;
	cint[1] = WINDOW_W;
	cint[2] = iw->t[iw->l.skybox]->w;
	cint[3] = iw->t[iw->l.skybox]->h;
	cint[4] = iw->d.screen_left;
	cfloat[2] = iw->p.rot;
	cfloat[3] = iw->v.angle;
	cfloat[0] = ((-iw->p.rotup + 2 * WINDOW_H) *
		(iw->t[iw->l.skybox]->h)) / (4 * WINDOW_H);
	cfloat[1] = (float)iw->t[iw->l.skybox]->h / (float)(4 * WINDOW_H);
}

void	draw_skybox_kernel(t_sdl *iw)
{
	int		cint[5];
	float	cfloat[4];
	size_t	global_item_size;
	size_t	local_item_size;

	draw_skybox_kernel2(iw, cint, cfloat);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint,
		CL_TRUE, 0, 5 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat,
		CL_TRUE, 0, 4 * sizeof(float), cfloat, 0, NULL, NULL);
	iw->k.ret = clSetKernelArg(iw->k.kernel4, 3, sizeof(cl_mem),
		(void *)&iw->k.m_t[iw->l.skybox]);
	global_item_size = iw->d.screen_right - iw->d.screen_left;
	local_item_size = 1;
	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel4, 1,
	NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	iw->k.ret = clFlush(iw->k.command_queue);
	iw->k.ret = clFinish(iw->k.command_queue);
}
