/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites_kernel.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 14:38:26 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 14:38:27 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	draw_sprite_kernel1(t_sdl *iw, t_sprite *sprite, int *cint)
{
	sprite->spriteheight = 2 * sprite->spritewidth *
		sprite->t->h / sprite->t->w;
	if (abs(sprite->z - iw->p.z) > 1500)
		sprite->spriteheight = (float)sprite->spriteheight * 1.0f /
		((float)abs(sprite->z - iw->p.z) / 1500.0f);
	sprite->ey = WINDOW_H * (iw->p.z + (int)sprite->pplen / 2 - sprite->z)
		/ (int)sprite->pplen + iw->p.rotup;
	sprite->sy = sprite->ey - sprite->spriteheight;
	if (iw->sectors[sprite->num_sec].light == 0
		|| iw->sectors[sprite->num_sec].light->t != 18)
		cint[12] = 1;
	else
		cint[12] = 0;
	cint[0] = sprite->sx;
	cint[1] = sprite->spritewidth;
	cint[2] = sprite->t->w;
	cint[3] = sprite->sy;
	cint[4] = sprite->ey;
	cint[5] = WINDOW_H;
	cint[6] = sprite->spriteheight;
	cint[7] = sprite->t->h;
	cint[8] = WINDOW_W;
	cint[9] = sprite->t->bpp;
	cint[10] = sprite->t->pitch;
}

void	draw_sprite_kernel(t_sdl *iw, t_sprite *sprite)
{
	int		cint[13];
	size_t	global_item_size;
	size_t	local_item_size;

	draw_sprite_kernel1(iw, sprite, cint);
	if (sprite->sx < 0)
		cint[11] = -sprite->sx;
	else
		cint[11] = 0;
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint,
		CL_TRUE, 0, 13 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_save_top3,
		CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), sprite->top, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_save_bottom3,
	CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), sprite->bottom, 0, NULL, NULL);
	iw->k.ret = clSetKernelArg(iw->k.kernel7, 3,
		sizeof(cl_mem), (void *)sprite->t_kernel);
	global_item_size = ft_min(WINDOW_W, sprite->ex) - ft_max(0, sprite->sx);
	local_item_size = 1;
	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel7, 1,
	NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
}

void	draw_sprites_kernel(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (iw->sectors[tmp1->num_sec].visited &&
			tmp1->draweble && tmp1->taken == 0)
		{
			if (tmp1->top[WINDOW_W / 2] != -1 && tmp1->sx < WINDOW_W / 2
				&& tmp1->ex > WINDOW_W / 2 &&
				tmp1->top[WINDOW_W / 2] < WINDOW_H / 2 &&
				tmp1->bottom[WINDOW_W / 2] > WINDOW_H / 2
				&& tmp1->sy < WINDOW_H / 2 && tmp1->ey > WINDOW_H / 2)
				iw->v.look_sprite = tmp1;
			draw_sprite_kernel(iw, tmp1);
		}
		tmp1 = tmp1->next;
	}
}

void	draw_glass_sprites_kernel(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (tmp1->num_sec == iw->d.cs && tmp1->draweble)
		{
			draw_sprite_kernel(iw, tmp1);
			tmp1->draweble = 0;
		}
		tmp1 = tmp1->next;
	}
}
