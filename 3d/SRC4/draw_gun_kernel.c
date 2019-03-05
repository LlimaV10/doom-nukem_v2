/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_gun_kernel.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 14:37:44 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 14:37:46 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_gun_kernel2(t_sdl *iw, int *cint, int *to_i)
{
	cint[3] = iw->guns.t_rect[iw->guns.t].x + iw->v.weapon_change_x;
	cint[4] = iw->guns.t_rect[iw->guns.t].y +
		iw->v.weapon_change_y + iw->v.weapon_change_y_hide;
	if (cint[4] < 0)
		cint[0] = 0;
	else
		cint[0] = cint[4];
	if (cint[3] < 0)
		cint[1] = -1;
	else
		cint[1] = cint[3] - 1;
	cint[5] = iw->guns.t_rect[iw->guns.t].w;
	cint[6] = iw->guns.t_rect[iw->guns.t].h;
	cint[2] = cint[5] + cint[3];
	if (cint[2] > WINDOW_W)
		cint[2] = WINDOW_W;
	cint[7] = iw->t_weap[iw->guns.t]->w;
	cint[8] = iw->t_weap[iw->guns.t]->h;
	cint[9] = iw->t_weap[iw->guns.t]->bpp;
	cint[10] = iw->t_weap[iw->guns.t]->pitch;
	cint[12] = WINDOW_W;
	*to_i = cint[4] + cint[6];
}

void	draw_gun_kernel(t_sdl *iw)
{
	int		cint[13];
	int		to_i;
	size_t	global_item_size;
	size_t	local_item_size;

	draw_gun_kernel2(iw, cint, &to_i);
	if (iw->sectors[iw->d.cs].light == 0 ||
		iw->sectors[iw->d.cs].light->t != 18)
		cint[11] = 1;
	else
		cint[11] = 0;
	if (to_i > WINDOW_H)
		to_i = WINDOW_H;
	if (to_i <= cint[0])
		return ;
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE,
		0, 13 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clSetKernelArg(iw->k.kernel8, 1, sizeof(cl_mem),
		(void *)&iw->k.m_t_weap[iw->guns.t]);
	global_item_size = to_i - cint[0];
	local_item_size = 1;
	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel8, 1,
	NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
}
