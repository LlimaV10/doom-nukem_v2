/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_opencl2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:24:31 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 17:24:32 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_kernels2(t_sdl *iw)
{
	iw->k.kernel0 = clCreateKernel(iw->k.program,
		"draw_inclined_wall_floor_ceil_tex_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel0, 0,
		sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel0, 1,
		sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel0, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel0, 6,
		sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel0, 7,
		sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel0, 8,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel0, 9,
		sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	iw->k.kernel1 = clCreateKernel(iw->k.program,
		"draw_wall_floor_ceil_tex_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel1, 0,
		sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel1, 1,
		sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel1, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
}

void	get_kernels3(t_sdl *iw)
{
	iw->k.ret = clSetKernelArg(iw->k.kernel1, 6,
		sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel1, 7,
		sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel1, 8,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel1, 9,
		sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	iw->k.kernel2 = clCreateKernel(iw->k.program,
		"draw_inclined_floor_ceil_betw_walls_tex_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 0,
		sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 1,
		sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 6,
		sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 7,
		sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 8,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 9,
		sizeof(cl_mem), (void *)&iw->k.m_cfloat);
}

void	get_kernels4(t_sdl *iw)
{
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 10,
		sizeof(cl_mem), (void *)&iw->k.m_top_betw);
	iw->k.ret = clSetKernelArg(iw->k.kernel2, 11,
		sizeof(cl_mem), (void *)&iw->k.m_bot_betw);
	iw->k.kernel3 = clCreateKernel(iw->k.program,
		"draw_floor_ceil_betw_walls_tex_kernel", &iw->k.ret);
	clSetKernelArg(iw->k.kernel3, 0,
		sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel3, 1,
		sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel3, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel3, 6,
		sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	clSetKernelArg(iw->k.kernel3, 7,
		sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	clSetKernelArg(iw->k.kernel3, 8,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	clSetKernelArg(iw->k.kernel3, 9,
		sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	clSetKernelArg(iw->k.kernel3, 10,
		sizeof(cl_mem), (void *)&iw->k.m_top_betw);
	clSetKernelArg(iw->k.kernel3, 11,
		sizeof(cl_mem), (void *)&iw->k.m_bot_betw);
}
