/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_kernel_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:04:35 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:04:35 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_kernel_mem4(t_sdl *iw)
{
	iw->k.m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		19 * sizeof(float), NULL, &iw->k.ret);
	iw->k.m_top_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_bot_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top2 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom2 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_top3 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_save_bottom3 = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
}

void	get_kernel_mem3(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
	{
		iw->k.m_t_weap[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_weap[i]->pitch * iw->t_weap[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_weap[i], CL_TRUE, 0,
			iw->t_weap[i]->pitch * iw->t_weap[i]->h,
			iw->t_weap[i]->pixels, 0, NULL, NULL);
	}
	iw->k.m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_sur = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY,
		(WINDOW_W + 1) * (WINDOW_H + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		(WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	iw->k.m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
		26 * sizeof(int), NULL, &iw->k.ret);
	get_kernel_mem4(iw);
}

void	get_kernel_mem2(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
	{
		iw->k.m_t_enemies[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_enemies[i]->pitch * iw->t_enemies[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_enemies[i], CL_TRUE, 0,
			iw->t_enemies[i]->pitch * iw->t_enemies[i]->h,
			iw->t_enemies[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
	{
		iw->k.m_t_pickup[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_pickup[i]->pitch * iw->t_pickup[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_pickup[i], CL_TRUE, 0,
			iw->t_pickup[i]->pitch * iw->t_pickup[i]->h,
			iw->t_pickup[i]->pixels, 0, NULL, NULL);
	}
	get_kernel_mem3(iw);
}

void	get_kernel_mem(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < TEXTURES_COUNT)
	{
		iw->k.m_t[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t[i]->pitch * iw->t[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t[i], CL_TRUE, 0,
			iw->t[i]->pitch * iw->t[i]->h, iw->t[i]->pixels, 0, NULL, NULL);
	}
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
	{
		iw->k.m_t_decor[i] = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY,
			iw->t_decor[i]->pitch * iw->t_decor[i]->h, NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_t_decor[i], CL_TRUE, 0,
			iw->t_decor[i]->pitch * iw->t_decor[i]->h,
			iw->t_decor[i]->pixels, 0, NULL, NULL);
	}
	get_kernel_mem2(iw);
}
