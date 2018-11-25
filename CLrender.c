#include "guardians.h"

void	load_kernel(t_kernel *k)
{
	int		fd;

	fd = open("kernel.cl", O_RDONLY);
	k->source_str = (char*)malloc(MAX_SOURCE_SIZE);
	k->source_size = read(fd, k->source_str, MAX_SOURCE_SIZE);
	close(fd);
	k->device_id = 0;
	k->platforms = 0;
	k->ret = clGetPlatformIDs(0, NULL, &k->ret_num_platforms);
	k->platforms = (cl_platform_id*)malloc(k->ret_num_platforms * sizeof(cl_platform_id));
	k->ret = clGetPlatformIDs(k->ret_num_platforms, k->platforms, NULL);
	k->ret = clGetDeviceIDs(k->platforms[0], CL_DEVICE_TYPE_ALL, 1,
		&k->device_id, &k->ret_num_devices);
	k->context = clCreateContext(NULL, 1, &k->device_id, NULL, NULL, &k->ret);
	k->command_queue = clCreateCommandQueue(k->context, k->device_id, 0, &k->ret);
	k->program = clCreateProgramWithSource(k->context, 1, (const char **)&k->source_str,
		(const size_t *)&k->source_size, &k->ret);
	k->ret = clBuildProgram(k->program, 1, &k->device_id, NULL, NULL, NULL);
	printf("Build ret %d\n", k->ret);
}

// 	clFlush(iw->k.command_queue);
// 	clFinish(iw->k.command_queue);
// 	clReleaseKernel(iw->k.kernel);
// 	/*clReleaseProgram(iw->k.program);*/
// 	clReleaseMemObject(m_ang);
// 	clReleaseMemObject(m_dang);
// 	clReleaseMemObject(m_top);
// 	clReleaseMemObject(m_bottom);
// 	clReleaseMemObject(m_twidth);
// 	clReleaseMemObject(m_left_x);
// 	clReleaseMemObject(m_lenpl);
// 	clReleaseMemObject(m_sing);
// 	clReleaseMemObject(m_left_olen);
// 	clReleaseMemObject(m_left_zu);
// 	clReleaseMemObject(m_left_zd);
// 	clReleaseMemObject(m_tsz);
// 	clReleaseMemObject(m_zudiff);
// 	clReleaseMemObject(m_zddiff);
// 	clReleaseMemObject(m_txs);
// 	clReleaseMemObject(m_zus);
// 	clReleaseMemObject(m_zds);
// 	/*clReleaseCommandQueue(iw->k.command_queue);
// 	clReleaseContext(iw->k.context);*/
// }

//void	draw_wall_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
//{
//	t_draw_wall_tex_kernel	d;
//	cl_mem	m_top;
//	cl_mem	m_bottom;
//	cl_mem	m_wpixels;
//	cl_mem	m_tpixels;
//	cl_mem	m_wallTop;
//	cl_mem	m_wallBot;
//	cl_mem	m_cint;
//	cl_mem	m_cfloat;
//	int		cint[6];
//	float	cfloat[8];
//
//	cint[0] = iw->t[left->wall->t]->w;
//	cint[1] = left->x;
//	cint[2] = left->zu;
//	cint[3] = left->zd;
//	cint[4] = WINDOW_W;
//	cint[5] = iw->t[left->wall->t]->h;
//	cfloat[4] = left->olen;
//	cfloat[5] = iw->tsz[left->wall->t];
//
//	d.lv.x = (float)(left->p.x - iw->p.x);
//	d.lv.y = (float)(left->p.y - iw->p.y);
//	d.rv.x = (float)(right->p.x - iw->p.x);
//	d.rv.y = (float)(right->p.y - iw->p.y);
//	cfloat[0] = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
//	cfloat[1] = cfloat[0] / (float)len;
//	d.rv.x = (float)(-right->p.x + left->p.x);
//	d.rv.y = (float)(-right->p.y + left->p.y);
//	cfloat[3] = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
//	cfloat[2] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
//	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
//	cfloat[6] = (right->zu - left->zu) / d.len_lr;
//	cfloat[7] = (right->zd - left->zd) / d.len_lr;
//
//	m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
//	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
//	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
//	m_tpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[left->wall->t]->w * iw->t[left->wall->t]->h
//		* sizeof(int), NULL, &iw->k.ret);
//	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
//	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
//	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 6 * sizeof(int), NULL, &iw->k.ret);
//	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 8 * sizeof(float), NULL, &iw->k.ret);
//
//	clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_tpixels, CL_TRUE, 0, iw->t[left->wall->t]->w * iw->t[left->wall->t]->h
//		* sizeof(int), iw->t[left->wall->t]->pixels, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 6 * sizeof(int), cint, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 8 * sizeof(float), cfloat, 0, NULL, NULL);
//
//	iw->k.kernel = clCreateKernel(iw->k.program, "draw_wall_tex", &iw->k.ret);
//	//printf("Create_kernel_ret %d\n", iw->k.ret);
//
//	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&m_top);
//	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&m_bottom);
//	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&m_wpixels);
//	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&m_tpixels);
//	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&m_wallTop);
//	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&m_wallBot);
//	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_cint);
//	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_cfloat);
//
//	size_t global_item_size = len;
//	size_t local_item_size = 1;
//
//	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
//		&global_item_size, &local_item_size, 0, NULL, NULL);
//	//printf("kernel run ret %d\n", iw->k.ret);
//
//	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
//		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
//	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
//		(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
//
//	clFlush(iw->k.command_queue);
//	clFinish(iw->k.command_queue);
//	clReleaseKernel(iw->k.kernel);
//
//	clReleaseMemObject(m_top);
//	clReleaseMemObject(m_bottom);
//	clReleaseMemObject(m_wpixels);
//	clReleaseMemObject(m_tpixels);
//	clReleaseMemObject(m_wallTop);
//	clReleaseMemObject(m_wallBot);
//	clReleaseMemObject(m_cint);
//	clReleaseMemObject(m_cfloat);
//}

//void	draw_inclined_floor_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
//{
//	t_draw_floor_tex_kernel	d;
//	int		cint[15];
//	float	cfloat[12];
//	cl_mem	m_top;
//	cl_mem	m_bottom;
//	cl_mem	m_wpixels;
//	cl_mem	m_tpixels;
//	cl_mem	m_wallTop;
//	cl_mem	m_wallBot;
//	cl_mem	m_cint;
//	cl_mem	m_cfloat;
//
//	cint[0] = left->x;
//	cint[1] = left->p.x;
//	cint[2] = left->p.y;
//	cint[3] = WINDOW_H;
//	cint[4] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
//	cint[5] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
//	cint[6] = WINDOW_W;
//
//	if (iw->sectors[iw->d.cs].fr.n == 0)
//	{
//		cint[7] = 0;
//		cint[8] = 0;
//		cint[9] = -1;
//		cint[10] = iw->sectors[iw->d.cs].fr.z;
//	}
//	else
//	{
//		cint[7] = iw->sectors[iw->d.cs].fr.n->a;
//		cint[8] = iw->sectors[iw->d.cs].fr.n->b;
//		cint[9] = iw->sectors[iw->d.cs].fr.n->c;
//		cint[10] = iw->sectors[iw->d.cs].fr.n->d;
//	}
//	if (iw->sectors[iw->d.cs].cl.n == 0)
//	{
//		cint[11] = 0;
//		cint[12] = 0;
//		cint[13] = -1;
//		cint[14] = iw->sectors[iw->d.cs].cl.z;
//	}
//	else
//	{
//		cint[11] = iw->sectors[iw->d.cs].cl.n->a;
//		cint[12] = iw->sectors[iw->d.cs].cl.n->b;
//		cint[13] = iw->sectors[iw->d.cs].cl.n->c;
//		cint[14] = iw->sectors[iw->d.cs].cl.n->d;
//	}
//	cfloat[5] = iw->d.screen.a;
//	cfloat[6] = iw->d.screen.b;
//	cfloat[7] = iw->d.screen.c;
//	cfloat[8] = sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
//
//	d.lv.x = (float)(left->p.x - iw->p.x);
//	d.lv.y = (float)(left->p.y - iw->p.y);
//	d.rv.x = (float)(right->p.x - iw->p.x);
//	d.rv.y = (float)(right->p.y - iw->p.y);
//	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
//	cfloat[0] = d.ang / (float)len;
//	d.ang = 0.0f;
//	d.rv.x = (float)(-right->p.x + left->p.x);
//	d.rv.y = (float)(-right->p.y + left->p.y);
//	cfloat[2] = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
//	cfloat[1] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
//	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
//	cfloat[3] = (float)(right->p.x - left->p.x) / d.len_lr;
//	cfloat[4] = (float)(right->p.y - left->p.y) / d.len_lr;
//	// d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
//	// d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
//	cfloat[9] = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
//	cfloat[10] = (float)iw->p.x / 1000.0f;
//	cfloat[11] = (float)iw->p.y / 1000.0f;
//
//	m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
//	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
//	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
//	m_tpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].fr.t]->w * iw->t[iw->sectors[iw->d.cs].fr.t]->h
//		* sizeof(int), NULL, &iw->k.ret);
//	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
//	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
//	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 15 * sizeof(int), NULL, &iw->k.ret);
//	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 12 * sizeof(float), NULL, &iw->k.ret);
//
//	clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_tpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].fr.t]->w * iw->t[iw->sectors[iw->d.cs].fr.t]->h
//		* sizeof(int), iw->t[iw->sectors[iw->d.cs].fr.t]->pixels, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 15 * sizeof(int), cint, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 12 * sizeof(float), cfloat, 0, NULL, NULL);
//
//	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_floor_tex", &iw->k.ret);
//	//printf("Create_kernel_floor_ret %d\n", iw->k.ret);
//
//	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&m_top);
//	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&m_bottom);
//	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&m_wpixels);
//	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&m_tpixels);
//	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&m_wallTop);
//	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&m_wallBot);
//	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_cint);
//	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_cfloat);
//
//	size_t global_item_size = len;
//	size_t local_item_size = 1;
//
//	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
//		&global_item_size, &local_item_size, 0, NULL, NULL);
//	//printf("kernel run ret %d\n", iw->k.ret);
//
//	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
//		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
//	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0,
//		(WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
//
//	clFlush(iw->k.command_queue);
//	clFinish(iw->k.command_queue);
//	clReleaseKernel(iw->k.kernel);
//
//	clReleaseMemObject(m_top);
//	clReleaseMemObject(m_bottom);
//	clReleaseMemObject(m_wpixels);
//	clReleaseMemObject(m_tpixels);
//	clReleaseMemObject(m_wallTop);
//	clReleaseMemObject(m_wallBot);
//	clReleaseMemObject(m_cint);
//	clReleaseMemObject(m_cfloat);
//}

void	draw_inclined_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	/*cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_wallpixels;
	cl_mem	m_floorpixels;
	cl_mem	m_ceilpixels;*/
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	int		cint[23];
	float	cfloat[17];

	cint[0] = iw->t[left->wall->t]->w;
	cint[1] = iw->t[left->wall->t]->h;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
	cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[21] = iw->d.screen_left;
	cint[22] = iw->d.screen_right;
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
	cint[19] = left->zu;
	cint[20] = left->zd;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = left->olen;
	cfloat[14] = iw->tsz[left->wall->t];

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
	cfloat[15] = (right->zu - left->zu) / d.len_lr;
	cfloat[16] = (right->zd - left->zd) / d.len_lr;
	cfloat[3] = (float)(right->p.x - left->p.x) / d.len_lr;
	cfloat[4] = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	cfloat[9] = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	cfloat[10] = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	/*m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
	m_wallpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[left->wall->t]->w *
		iw->t[left->wall->t]->h * sizeof(int), NULL, &iw->k.ret);*/
	/*m_floorpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
		iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), NULL, &iw->k.ret);
	m_ceilpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
		iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), NULL, &iw->k.ret);*/
	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 23 * sizeof(int), NULL, &iw->k.ret);
	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 17 * sizeof(float), NULL, &iw->k.ret);

	/*clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallpixels, CL_TRUE, 0, iw->t[left->wall->t]->w *
		iw->t[left->wall->t]->h * sizeof(int), iw->t[left->wall->t]->pixels, 0, NULL, NULL);
	if (left->wall->t == iw->sectors[iw->d.cs].fr.t)
		m_floorpixels = m_wallpixels;
	else
	{
		m_floorpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, m_floorpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].fr.t]->pixels, 0, NULL, NULL);
	}
	if (left->wall->t == iw->sectors[iw->d.cs].cl.t)
		m_ceilpixels = m_wallpixels;
	else if (iw->sectors[iw->d.cs].fr.t == iw->sectors[iw->d.cs].cl.t)
		m_ceilpixels = m_floorpixels;
	else
	{
		m_ceilpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, m_ceilpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].cl.t]->pixels, 0, NULL, NULL);
	}*/
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 23 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 17 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_wall_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_wfci_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);
	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_wallTop);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_wallBot);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_cint);
	clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("kernel run ret %d\n", iw->k.ret);

	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);*/

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);

	/*clReleaseMemObject(m_top);
	clReleaseMemObject(m_bottom);
	clReleaseMemObject(m_wpixels);
	clReleaseMemObject(m_wallpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].fr.t)
		clReleaseMemObject(m_floorpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].cl.t &&
		iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
		clReleaseMemObject(m_ceilpixels);*/
	clReleaseMemObject(m_wallTop);
	clReleaseMemObject(m_wallBot);
	clReleaseMemObject(m_cint);
	clReleaseMemObject(m_cfloat);
}

void	draw_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	/*cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_wallpixels;
	cl_mem	m_floorpixels;
	cl_mem	m_ceilpixels;*/
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	int		cint[14];
	float	cfloat[17];

	cint[0] = iw->t[left->wall->t]->w;
	cint[1] = iw->t[left->wall->t]->h;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
	cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[12] = iw->d.screen_left;
	cint[13] = iw->d.screen_right;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = left->olen;
	cfloat[14] = iw->tsz[left->wall->t];

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
	cfloat[15] = get_ceil_z(iw, iw->p.x, iw->p.y);
	cfloat[16] = get_floor_z(iw, iw->p.x, iw->p.y);
	cint[11] = (int)(cfloat[15] - cfloat[16]);
	cfloat[9] = (float)cint[11] / ((float)iw->p.z - cfloat[16]);
	cfloat[10] = (float)cint[11] / (cfloat[15] - (float)iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	/*m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
	m_wallpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[left->wall->t]->w *
		iw->t[left->wall->t]->h * sizeof(int), NULL, &iw->k.ret);*/
	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 14 * sizeof(int), NULL, &iw->k.ret);
	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 17 * sizeof(float), NULL, &iw->k.ret);

	/*clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallpixels, CL_TRUE, 0, iw->t[left->wall->t]->w *
		iw->t[left->wall->t]->h * sizeof(int), iw->t[left->wall->t]->pixels, 0, NULL, NULL);
	if (left->wall->t == iw->sectors[iw->d.cs].fr.t)
		m_floorpixels = m_wallpixels;
	else
	{
		m_floorpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, m_floorpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].fr.t]->pixels, 0, NULL, NULL);
	}
	if (left->wall->t == iw->sectors[iw->d.cs].cl.t)
		m_ceilpixels = m_wallpixels;
	else if (iw->sectors[iw->d.cs].fr.t == iw->sectors[iw->d.cs].cl.t)
		m_ceilpixels = m_floorpixels;
	else
	{
		m_ceilpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, m_ceilpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].cl.t]->pixels, 0, NULL, NULL);
	}*/
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 14 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 17 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_wall_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_wfc_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);
	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);

	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_wallTop);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_wallBot);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_cint);
	clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("kernel run ret %d\n", iw->k.ret);

	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);*/

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);

	/*clReleaseMemObject(m_top);
	clReleaseMemObject(m_bottom);
	clReleaseMemObject(m_wpixels);
	clReleaseMemObject(m_wallpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].fr.t)
		clReleaseMemObject(m_floorpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].cl.t &&
		iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
		clReleaseMemObject(m_ceilpixels);*/
	clReleaseMemObject(m_wallTop);
	clReleaseMemObject(m_wallBot);
	clReleaseMemObject(m_cint);
	clReleaseMemObject(m_cfloat);
}

void	draw_inclined_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	/*cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_floorpixels;
	cl_mem	m_ceilpixels;*/
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	cl_mem	m_top_betw;
	cl_mem	m_bot_betw;
	int		cint[23];
	float	cfloat[17];

	cint[0] = iw->t[left->wall->t]->w;
	cint[1] = iw->t[left->wall->t]->h;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
	cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[21] = iw->d.screen_left;
	cint[22] = iw->d.screen_right;

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
	cint[19] = left->zu;
	cint[20] = left->zd;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = left->olen;
	cfloat[14] = iw->tsz[left->wall->t];

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
	cfloat[15] = (right->zu - left->zu) / d.len_lr;
	cfloat[16] = (right->zd - left->zd) / d.len_lr;
	cfloat[3] = (float)(right->p.x - left->p.x) / d.len_lr;
	cfloat[4] = (float)(right->p.y - left->p.y) / d.len_lr;
	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
	cfloat[9] = (float)(d.zu - d.zd) / (float)(iw->p.z - d.zd);
	cfloat[10] = (float)(d.zu - d.zd) / (float)(d.zu - iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	/*m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
	m_floorpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
		iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), NULL, &iw->k.ret);*/
	/*m_ceilpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
		iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), NULL, &iw->k.ret);*/
	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 23 * sizeof(int), NULL, &iw->k.ret);
	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 17 * sizeof(float), NULL, &iw->k.ret);
	m_top_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_bot_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	/*clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_floorpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].fr.t]->pixels, 0, NULL, NULL);
	if (iw->sectors[iw->d.cs].fr.t == iw->sectors[iw->d.cs].cl.t)
		m_ceilpixels = m_floorpixels;
	else
	{
		m_ceilpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, m_ceilpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].cl.t]->pixels, 0, NULL, NULL);
	}*/
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 23 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 17 * sizeof(float), cfloat, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_top_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_top_betw, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bot_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_bot_betw, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_floor_ceil_betw_walls_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fci_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);
	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_wallTop);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_wallBot);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_cint);
	clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&m_cfloat);
	clSetKernelArg(iw->k.kernel, 10, sizeof(cl_mem), (void *)&m_top_betw);
	clSetKernelArg(iw->k.kernel, 11, sizeof(cl_mem), (void *)&m_bot_betw);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("kernel run ret %d\n", iw->k.ret);

	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0,
		len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);*/

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);

	/*clReleaseMemObject(m_top);
	clReleaseMemObject(m_bottom);
	clReleaseMemObject(m_wpixels);
	clReleaseMemObject(m_floorpixels);
	if (iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
		clReleaseMemObject(m_ceilpixels);*/
	clReleaseMemObject(m_wallTop);
	clReleaseMemObject(m_wallBot);
	clReleaseMemObject(m_cint);
	clReleaseMemObject(m_cfloat);
	clReleaseMemObject(m_top_betw);
	clReleaseMemObject(m_bot_betw);
}

//void	draw_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
//{
//	t_draw_wall_floor_ceil_tex_kernel	d;
//	cl_mem	m_top;
//	cl_mem	m_bottom;
//	cl_mem	m_wpixels;
//	cl_mem	m_floorpixels;
//	cl_mem	m_ceilpixels;
//	cl_mem	m_wallTop;
//	cl_mem	m_wallBot;
//	cl_mem	m_cint;
//	cl_mem	m_cfloat;
//	int		cint[12];
//	float	cfloat[13];
//
//	cint[0] = iw->t[left->wall->t]->w;
//	cint[1] = iw->t[left->wall->t]->h;
//	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
//	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
//	cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
//	cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
//	cint[6] = WINDOW_W;
//	cint[7] = WINDOW_H;
//	cint[8] = left->x;
//	cint[9] = left->p.x;
//	cint[10] = left->p.y;
//
//	cfloat[5] = iw->d.screen.a;
//	cfloat[6] = iw->d.screen.b;
//	cfloat[7] = iw->d.screen.c;
//	cfloat[8] = iw->d.screen_len;
//	// cfloat[13] = left->olen;
//	// cfloat[14] = iw->tsz[left->wall->t];
//
//	d.lv.x = (float)(left->p.x - iw->p.x);
//	d.lv.y = (float)(left->p.y - iw->p.y);
//	d.rv.x = (float)(right->p.x - iw->p.x);
//	d.rv.y = (float)(right->p.y - iw->p.y);
//	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
//	cfloat[0] = d.ang / (float)len;
//	d.ang = 0.0f;
//	d.rv.x = (float)(-right->p.x + left->p.x);
//	d.rv.y = (float)(-right->p.y + left->p.y);
//	cfloat[2] = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
//	cfloat[1] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
//	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
//	cfloat[3] = (float)(right->p.x - left->p.x) / d.len_lr;
//	cfloat[4] = (float)(right->p.y - left->p.y) / d.len_lr;
//	d.zu = get_ceil_z(iw, iw->p.x, iw->p.y);
//	d.zd = get_floor_z(iw, iw->p.x, iw->p.y);
//	cint[11] = (int)(d.zu - d.zd);
//	cfloat[9] = (float)cint[11] / ((float)iw->p.z - d.zd);
//	cfloat[10] = (float)cint[11] / (d.zu - (float)iw->p.z);
//	cfloat[11] = (float)iw->p.x / 1000.0f;
//	cfloat[12] = (float)iw->p.y / 1000.0f;
//
//	m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
//	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
//	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
//	m_floorpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
//			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), NULL, &iw->k.ret);
//	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
//	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
//	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 12 * sizeof(int), NULL, &iw->k.ret);
//	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 13 * sizeof(float), NULL, &iw->k.ret);
//
//	clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_floorpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
//			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].fr.t]->pixels, 0, NULL, NULL);
//	if (iw->sectors[iw->d.cs].fr.t == iw->sectors[iw->d.cs].cl.t)
//		m_ceilpixels = m_floorpixels;
//	else
//	{
//		m_ceilpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
//			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), NULL, &iw->k.ret);
//		clEnqueueWriteBuffer(iw->k.command_queue, m_ceilpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
//			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].cl.t]->pixels, 0, NULL, NULL);
//	}
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 12 * sizeof(int), cint, 0, NULL, NULL);
//	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 13 * sizeof(float), cfloat, 0, NULL, NULL);
//
//	iw->k.kernel = clCreateKernel(iw->k.program, "draw_floor_ceil_tex_kernel", &iw->k.ret);
//	//printf("Create_kernel_wfc_ret %d\n", iw->k.ret);
//
//	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&m_top);
//	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&m_bottom);
//	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&m_wpixels);
//	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&m_floorpixels);
//	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&m_ceilpixels);
//	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&m_wallTop);
//	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_wallBot);
//	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_cint);
//	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_cfloat);
//
//	size_t global_item_size = len;
//	size_t local_item_size = 1;
//
//	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
//		&global_item_size, &local_item_size, 0, NULL, NULL);
//	//printf("kernel run ret %d\n", iw->k.ret);
//
//	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
//		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
//	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
//		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
//		iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0,
//		len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);
//
//	clFlush(iw->k.command_queue);
//	clFinish(iw->k.command_queue);
//	clReleaseKernel(iw->k.kernel);
//
//	clReleaseMemObject(m_top);
//	clReleaseMemObject(m_bottom);
//	clReleaseMemObject(m_wpixels);
//	clReleaseMemObject(m_floorpixels);
//	if (iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
//		clReleaseMemObject(m_ceilpixels);
//	clReleaseMemObject(m_wallTop);
//	clReleaseMemObject(m_wallBot);
//	clReleaseMemObject(m_cint);
//	clReleaseMemObject(m_cfloat);
//}

void	draw_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	/*cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_wallpixels;
	cl_mem	m_floorpixels;
	cl_mem	m_ceilpixels;*/
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	cl_mem	m_top_betw;
	cl_mem	m_bot_betw;
	int		cint[14];
	float	cfloat[17];

	cint[0] = iw->t[left->wall->t]->w;
	cint[1] = iw->t[left->wall->t]->h;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
	cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[12] = iw->d.screen_left;
	cint[13] = iw->d.screen_right;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = left->olen;
	cfloat[14] = iw->tsz[left->wall->t];

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
	cfloat[15] = get_ceil_z(iw, iw->p.x, iw->p.y);
	cfloat[16] = get_floor_z(iw, iw->p.x, iw->p.y);
	cint[11] = (int)(cfloat[15] - cfloat[16]);
	cfloat[9] = (float)cint[11] / ((float)iw->p.z - cfloat[16]);
	cfloat[10] = (float)cint[11] / (cfloat[15] - (float)iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	/*m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
	m_wallpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[left->wall->t]->w *
		iw->t[left->wall->t]->h * sizeof(int), NULL, &iw->k.ret);*/
	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 14 * sizeof(int), NULL, &iw->k.ret);
	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 17 * sizeof(float), NULL, &iw->k.ret);
	m_top_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_bot_betw = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);

	/*clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallpixels, CL_TRUE, 0, iw->t[left->wall->t]->w *
		iw->t[left->wall->t]->h * sizeof(int), iw->t[left->wall->t]->pixels, 0, NULL, NULL);
	if (left->wall->t == iw->sectors[iw->d.cs].fr.t)
		m_floorpixels = m_wallpixels;
	else
	{
		m_floorpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, m_floorpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].fr.t]->w *
			iw->t[iw->sectors[iw->d.cs].fr.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].fr.t]->pixels, 0, NULL, NULL);
	}
	if (left->wall->t == iw->sectors[iw->d.cs].cl.t)
		m_ceilpixels = m_wallpixels;
	else if (iw->sectors[iw->d.cs].fr.t == iw->sectors[iw->d.cs].cl.t)
		m_ceilpixels = m_floorpixels;
	else
	{
		m_ceilpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), NULL, &iw->k.ret);
		clEnqueueWriteBuffer(iw->k.command_queue, m_ceilpixels, CL_TRUE, 0, iw->t[iw->sectors[iw->d.cs].cl.t]->w *
			iw->t[iw->sectors[iw->d.cs].cl.t]->h * sizeof(int), iw->t[iw->sectors[iw->d.cs].cl.t]->pixels, 0, NULL, NULL);
	}*/
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 14 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 17 * sizeof(float), cfloat, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_top_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_top_betw, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bot_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_bot_betw, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_floor_ceil_betw_walls_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_wfc_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);

	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_wallTop);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_wallBot);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_cint);
	clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&m_cfloat);
	clSetKernelArg(iw->k.kernel, 10, sizeof(cl_mem), (void *)&m_top_betw);
	clSetKernelArg(iw->k.kernel, 11, sizeof(cl_mem), (void *)&m_bot_betw);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("kernel run ret %d\n", iw->k.ret);

	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0,
		len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);*/

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);

	/*clReleaseMemObject(m_top);
	clReleaseMemObject(m_bottom);
	clReleaseMemObject(m_wpixels);
	clReleaseMemObject(m_wallpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].fr.t)
		clReleaseMemObject(m_floorpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].cl.t && 
		iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
		clReleaseMemObject(m_ceilpixels);*/
	clReleaseMemObject(m_wallTop);
	clReleaseMemObject(m_wallBot);
	clReleaseMemObject(m_cint);
	clReleaseMemObject(m_cfloat);
	clReleaseMemObject(m_top_betw);
	clReleaseMemObject(m_bot_betw);
}

void	draw_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	int		cint[14];
	float	cfloat[13];

	cint[0] = 0;
	cint[1] = 0;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
	cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[12] = iw->d.screen_left;
	cint[13] = iw->d.screen_right;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;

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
	cint[11] = (int)(cfloat[15] - cfloat[16]);
	cfloat[9] = (float)cint[11] / ((float)iw->p.z - cfloat[16]);
	cfloat[10] = (float)cint[11] / (cfloat[15] - (float)iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 14 * sizeof(int), NULL, &iw->k.ret);
	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 13 * sizeof(float), NULL, &iw->k.ret);

	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 14 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 13 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_wfc_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&m_wallTop);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_wallBot);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_cint);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("kernel run ret %d\n", iw->k.ret);


	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);

	clReleaseMemObject(m_wallTop);
	clReleaseMemObject(m_wallBot);
	clReleaseMemObject(m_cint);
	clReleaseMemObject(m_cfloat);
}

void	draw_inclined_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	int		cint[21];
	float	cfloat[13];

	cint[0] = 0;
	cint[1] = 0;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
	cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[19] = iw->d.screen_left;
	cint[20] = iw->d.screen_right;

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

	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 21 * sizeof(int), NULL, &iw->k.ret);
	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 13 * sizeof(float), NULL, &iw->k.ret);

	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 21 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 13 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fci_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&m_wallTop);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_wallBot);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_cint);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("kernel run ret %d\n", iw->k.ret);

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);

	clReleaseMemObject(m_wallTop);
	clReleaseMemObject(m_wallBot);
	clReleaseMemObject(m_cint);
	clReleaseMemObject(m_cfloat);
}