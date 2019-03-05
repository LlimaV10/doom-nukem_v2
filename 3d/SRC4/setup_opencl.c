/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_opencl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 14:38:40 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 14:38:41 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_kernels5(t_sdl *iw)
{
	iw->k.kernel4 = clCreateKernel(iw->k.program,
		"draw_skybox_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel4, 0,
		sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel4, 1,
		sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel4, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel4, 4,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel4, 5,
		sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	iw->k.kernel5 = clCreateKernel(iw->k.program,
		"draw_glass_tex_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 0,
		sizeof(cl_mem), (void *)&iw->k.m_save_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 1,
		sizeof(cl_mem), (void *)&iw->k.m_save_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 4,
		sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 5,
		sizeof(cl_mem), (void *)&iw->k.m_wallBot);
}

void	get_kernels6(t_sdl *iw)
{
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 6,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel5, 7,
		sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	iw->k.kernel6 = clCreateKernel(iw->k.program,
		"draw_picture_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel6, 0,
		sizeof(cl_mem), (void *)&iw->k.m_save_top2);
	iw->k.ret = clSetKernelArg(iw->k.kernel6, 1,
		sizeof(cl_mem), (void *)&iw->k.m_save_bottom2);
	iw->k.ret = clSetKernelArg(iw->k.kernel6, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel6, 4,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel6, 5,
		sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	iw->k.kernel7 = clCreateKernel(iw->k.program,
		"draw_sprite_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel7, 0,
		sizeof(cl_mem), (void *)&iw->k.m_save_top3);
	iw->k.ret = clSetKernelArg(iw->k.kernel7, 1,
		sizeof(cl_mem), (void *)&iw->k.m_save_bottom3);
	iw->k.ret = clSetKernelArg(iw->k.kernel7, 2,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
}

void	get_kernels(t_sdl *iw)
{
	get_kernels2(iw);
	get_kernels3(iw);
	get_kernels4(iw);
	get_kernels5(iw);
	get_kernels6(iw);
	iw->k.ret = clSetKernelArg(iw->k.kernel7, 4,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.kernel8 = clCreateKernel(iw->k.program,
		"draw_gun_kernel", &iw->k.ret);
	iw->k.ret = clSetKernelArg(iw->k.kernel8, 0,
		sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel8, 2,
		sizeof(cl_mem), (void *)&iw->k.m_cint);
}

void	load_kernel2(t_kernel *k)
{
	k->platforms = (cl_platform_id*)malloc(k->ret_num_platforms
		* sizeof(cl_platform_id));
	k->ret = clGetPlatformIDs(k->ret_num_platforms, k->platforms, NULL);
	k->ret = clGetDeviceIDs(k->platforms[0], CL_DEVICE_TYPE_ALL, 1,
		&k->device_id, &k->ret_num_devices);
	k->context = clCreateContext(NULL, 1, &k->device_id, NULL, NULL, &k->ret);
	k->command_queue = clCreateCommandQueue(k->context, k->device_id,
		0, &k->ret);
	k->program = clCreateProgramWithSource(k->context, 1,
		(const char **)&k->source_str,
		(const size_t *)&k->source_size, &k->ret);
	k->ret = clBuildProgram(k->program, 1, &k->device_id, NULL, NULL, NULL);
}

void	load_kernel(t_kernel *k, t_sdl *iw)
{
	int		fd;

	if (iw->v.game_mode == 0)
	{
		k->ret = 1;
		fd = open("3d/kernel.cl", O_RDONLY);
		if (fd < 0)
			return ;
		k->source_str = (char *)malloc(MAX_SOURCE_SIZE);
		k->source_size = read(fd, k->source_str, MAX_SOURCE_SIZE);
		close(fd);
	}
	k->device_id = 0;
	k->platforms = 0;
	k->ret = clGetPlatformIDs(0, NULL, &k->ret_num_platforms);
	if (k->ret_num_platforms <= 0)
	{
		k->ret = 1;
		return ;
	}
	load_kernel2(k);
}
