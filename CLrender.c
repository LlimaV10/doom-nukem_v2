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


void	draw_inclined_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	/*cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_wallpixels;
	cl_mem	m_floorpixels;
	cl_mem	m_ceilpixels;*/
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	int		cint[25];
	float	cfloat[17];

	if (iw->sectors[iw->d.cs].light == 0 || iw->sectors[iw->d.cs].light->t != 18)
		cint[24] = 1;
	else
		cint[24] = 0;
	if (left->wall->t >= 0)
	{
		cint[0] = iw->t[left->wall->t]->w;
		cint[1] = iw->t[left->wall->t]->h;
		cfloat[14] = iw->tsz[left->wall->t];
	}
	else
		cint[0] = -1;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[21] = iw->d.screen_left;
	cint[22] = iw->d.screen_right;
	cint[23] = iw->sectors[iw->d.cs].cl.t;
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

	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 25 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 17 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_wall_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_wfci_ret %d\n", iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	if (left->wall->t >= 0)
		iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);
	else
		iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[0]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);

	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);*/

	iw->k.ret = clFlush(iw->k.command_queue);
	iw->k.ret = clFinish(iw->k.command_queue);
	iw->k.ret = clReleaseKernel(iw->k.kernel);

	//printf("kernel run ret %d\n", iw->k.ret);
	/*clReleaseMemObject(m_top);
	clReleaseMemObject(m_bottom);
	clReleaseMemObject(m_wpixels);
	clReleaseMemObject(m_wallpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].fr.t)
		clReleaseMemObject(m_floorpixels);
	if (left->wall->t != iw->sectors[iw->d.cs].cl.t &&
		iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
		clReleaseMemObject(m_ceilpixels);*/
		// iw->k.ret = clReleaseMemObject(m_wallTop);
		// iw->k.ret = clReleaseMemObject(m_wallBot);
		// iw->k.ret = clReleaseMemObject(m_cint);
		// iw->k.ret = clReleaseMemObject(m_cfloat);
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
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	int		cint[16];
	float	cfloat[17];

	if (iw->sectors[iw->d.cs].light == 0 || iw->sectors[iw->d.cs].light->t != 18)
		cint[15] = 1;
	else
		cint[15] = 0;
	if (left->wall->t >= 0)
	{
		cint[0] = iw->t[left->wall->t]->w;
		cint[1] = iw->t[left->wall->t]->h;
		cfloat[14] = iw->tsz[left->wall->t];
	}
	else
		cint[0] = -1;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[12] = iw->d.screen_left;
	cint[13] = iw->d.screen_right;
	cint[14] = iw->sectors[iw->d.cs].cl.t;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = left->olen;
	
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

	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 16 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 17 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_wall_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_wfc_ret %d len %d\n", iw->k.ret, len);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	if (left->wall->t >= 0)
		iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);
	else
		iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[0]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);


	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);*/

	iw->k.ret = clFlush(iw->k.command_queue);
	iw->k.ret = clFinish(iw->k.command_queue);
	iw->k.ret = clReleaseKernel(iw->k.kernel);
	//rintf("kernel run ret %d\n", iw->k.ret);
		/*clReleaseMemObject(m_top);
		clReleaseMemObject(m_bottom);
		clReleaseMemObject(m_wpixels);
		clReleaseMemObject(m_wallpixels);
		if (left->wall->t != iw->sectors[iw->d.cs].fr.t)
			clReleaseMemObject(m_floorpixels);
		if (left->wall->t != iw->sectors[iw->d.cs].cl.t &&
			iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
			clReleaseMemObject(m_ceilpixels);*/
			// clReleaseMemObject(m_wallTop);
			// clReleaseMemObject(m_wallBot);
			// clReleaseMemObject(m_cint);
			// clReleaseMemObject(m_cfloat);
}

void	draw_inclined_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	/*cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_floorpixels;
	cl_mem	m_ceilpixels;*/
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	// cl_mem	m_top_betw;
	// cl_mem	m_bot_betw;
	int		cint[26];
	float	cfloat[19];

	if (iw->sectors[iw->d.cs].light == 0 || iw->sectors[iw->d.cs].light->t != 18)
		cint[25] = 1;
	else
		cint[25] = 0;
	cint[0] = iw->t[left->wall->t]->w;
	cint[1] = iw->t[left->wall->t]->h;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	else
	{
		cint[4] = iw->t[iw->l.skybox]->w;
		cint[5] = iw->t[iw->l.skybox]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[21] = iw->d.screen_left;
	cint[22] = iw->d.screen_right;
	cint[23] = iw->sectors[iw->d.cs].cl.t;
	cint[24] = iw->p.rotup;

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
	cfloat[17] = iw->p.rot;
	cfloat[18] = iw->v.angle;

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


	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 26 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 19 * sizeof(float), cfloat, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_top_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_top_betw, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_bot_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_bot_betw, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_floor_ceil_betw_walls_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fci_betw_ret %d\n", iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->l.skybox]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 10, sizeof(cl_mem), (void *)&iw->k.m_top_betw);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 11, sizeof(cl_mem), (void *)&iw->k.m_bot_betw);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);


	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0,
		len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);*/

	iw->k.ret = clFlush(iw->k.command_queue);
	iw->k.ret = clFinish(iw->k.command_queue);
	iw->k.ret = clReleaseKernel(iw->k.kernel);
	//printf("kernel run ret %d\n", iw->k.ret);
		/*clReleaseMemObject(m_top);
		clReleaseMemObject(m_bottom);
		clReleaseMemObject(m_wpixels);
		clReleaseMemObject(m_floorpixels);
		if (iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
			clReleaseMemObject(m_ceilpixels);*/
			// iw->k.ret = clReleaseMemObject(m_wallTop);
			// iw->k.ret = clReleaseMemObject(m_wallBot);
			// iw->k.ret = clReleaseMemObject(m_cint);
			// iw->k.ret = clReleaseMemObject(m_cfloat);
			// iw->k.ret = clReleaseMemObject(m_top_betw);
			// iw->k.ret = clReleaseMemObject(m_bot_betw);
}

void	draw_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	/*cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_wallpixels;
	cl_mem	m_floorpixels;
	cl_mem	m_ceilpixels;*/
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	// cl_mem	m_top_betw;
	// cl_mem	m_bot_betw;
	int		cint[17];
	float	cfloat[19];

	if (iw->sectors[iw->d.cs].light == 0 || iw->sectors[iw->d.cs].light->t != 18)
		cint[16] = 1;
	else
		cint[16] = 0;
	cint[0] = iw->t[left->wall->t]->w;
	cint[1] = iw->t[left->wall->t]->h;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	else
	{
		cint[4] = iw->t[iw->l.skybox]->w;
		cint[5] = iw->t[iw->l.skybox]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[12] = iw->d.screen_left;
	cint[13] = iw->d.screen_right;
	cint[14] = iw->sectors[iw->d.cs].cl.t;
	cint[15] = iw->p.rotup;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = left->olen;
	cfloat[14] = iw->tsz[left->wall->t];
	cfloat[17] = iw->p.rot;
	cfloat[18] = iw->v.angle;

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

	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 17 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 19 * sizeof(float), cfloat, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_top_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_top_betw, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_bot_betw, CL_TRUE, 0, len * sizeof(int), iw->d.save_bot_betw, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_floor_ceil_betw_walls_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fc_betw_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->t]);

	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_t[iw->l.skybox]);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cint);
	clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&iw->k.m_cfloat);
	clSetKernelArg(iw->k.kernel, 10, sizeof(cl_mem), (void *)&iw->k.m_top_betw);
	clSetKernelArg(iw->k.kernel, 11, sizeof(cl_mem), (void *)&iw->k.m_bot_betw);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);


	/*iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		len * sizeof(int), &iw->d.top[left->x], 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0,
		len * sizeof(int), &iw->d.bottom[left->x], 0, NULL, NULL);*/

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
	//printf("kernel run ret %d\n", iw->k.ret);
		/*clReleaseMemObject(m_top);
		clReleaseMemObject(m_bottom);
		clReleaseMemObject(m_wpixels);
		clReleaseMemObject(m_wallpixels);
		if (left->wall->t != iw->sectors[iw->d.cs].fr.t)
			clReleaseMemObject(m_floorpixels);
		if (left->wall->t != iw->sectors[iw->d.cs].cl.t &&
			iw->sectors[iw->d.cs].fr.t != iw->sectors[iw->d.cs].cl.t)
			clReleaseMemObject(m_ceilpixels);*/
			// clReleaseMemObject(m_wallTop);
			// clReleaseMemObject(m_wallBot);
			// clReleaseMemObject(m_cint);
			// clReleaseMemObject(m_cfloat);
			// clReleaseMemObject(m_top_betw);
			// clReleaseMemObject(m_bot_betw);
}

void	draw_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	int		cint[16];
	float	cfloat[15];

	cint[0] = 0;
	cint[1] = 0;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	else
	{
		cint[4] = iw->t[iw->l.skybox]->w;
		cint[5] = iw->t[iw->l.skybox]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[12] = iw->d.screen_left;
	cint[13] = iw->d.screen_right;
	cint[14] = iw->sectors[iw->d.cs].cl.t;
	cint[15] = iw->p.rotup;

	cfloat[5] = iw->d.screen.a;
	cfloat[6] = iw->d.screen.b;
	cfloat[7] = iw->d.screen.c;
	cfloat[8] = iw->d.screen_len;
	cfloat[13] = iw->p.rot;
	cfloat[14] = iw->v.angle;

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

	cint[11] = d.zu - d.zd;
	cfloat[9] = (float)cint[11] / ((float)iw->p.z - (float)d.zd);
	cfloat[10] = (float)cint[11] / ((float)d.zu - (float)iw->p.z);
	cfloat[11] = (float)iw->p.x / 1000.0f;
	cfloat[12] = (float)iw->p.y / 1000.0f;

	// m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 14 * sizeof(int), NULL, &iw->k.ret);
	// m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 13 * sizeof(float), NULL, &iw->k.ret);

	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 16 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 15 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fc_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->l.skybox]);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_cint);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);



	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
	//printf("kernel run ret %d\n", iw->k.ret);
		// clReleaseMemObject(m_wallTop);
		// clReleaseMemObject(m_wallBot);
		// clReleaseMemObject(m_cint);
		// clReleaseMemObject(m_cfloat);
}

void	draw_inclined_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	// cl_mem	m_wallTop;
	// cl_mem	m_wallBot;
	// cl_mem	m_cint;
	// cl_mem	m_cfloat;
	int		cint[23];
	float	cfloat[15];

	cint[0] = 0;
	cint[1] = 0;
	cint[2] = iw->t[iw->sectors[iw->d.cs].fr.t]->w;
	cint[3] = iw->t[iw->sectors[iw->d.cs].fr.t]->h;
	if (iw->sectors[iw->d.cs].cl.t >= 0)
	{
		cint[4] = iw->t[iw->sectors[iw->d.cs].cl.t]->w;
		cint[5] = iw->t[iw->sectors[iw->d.cs].cl.t]->h;
	}
	else
	{
		cint[4] = iw->t[iw->l.skybox]->w;
		cint[5] = iw->t[iw->l.skybox]->h;
	}
	cint[6] = WINDOW_W;
	cint[7] = WINDOW_H;
	cint[8] = left->x;
	cint[9] = left->p.x;
	cint[10] = left->p.y;
	cint[19] = iw->d.screen_left;
	cint[20] = iw->d.screen_right;
	cint[21] = iw->sectors[iw->d.cs].cl.t;
	cint[22] = iw->p.rotup;

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
	cfloat[13] = iw->p.rot;
	cfloat[14] = iw->v.angle;

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

	// m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	// m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 21 * sizeof(int), NULL, &iw->k.ret);
	// m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 13 * sizeof(float), NULL, &iw->k.ret);

	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 23 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 15 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_inclined_floor_ceil_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fci_ret %d\n", iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].fr.t]);
	if (iw->sectors[iw->d.cs].cl.t >= 0)
		iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->sectors[iw->d.cs].cl.t]);
	else
		iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_t[iw->l.skybox]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);


	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
	//printf("kernel run ret %d\n", iw->k.ret);
		// clReleaseMemObject(m_wallTop);
		// clReleaseMemObject(m_wallBot);
		// clReleaseMemObject(m_cint);
		// clReleaseMemObject(m_cfloat);
}

void	draw_skybox_kernel(t_sdl *iw)
{
	int		cint[5];
	float	cfloat[4];

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

	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 5 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 4 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_skybox_kernel", &iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[iw->l.skybox]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = iw->d.screen_right - iw->d.screen_left;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);

	//printf("skybox run ret %d\n", iw->k.ret);

	iw->k.ret = clFlush(iw->k.command_queue);
	iw->k.ret = clFinish(iw->k.command_queue);
	iw->k.ret = clReleaseKernel(iw->k.kernel);
}

void	draw_glass_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_floor_ceil_tex_kernel	d;
	int		cint[6];
	float	cfloat[7];

	cint[1] = iw->t[left->wall->glass]->w;
	cint[2] = iw->t[left->wall->glass]->h;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[0] = d.ang / (float)len;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	cfloat[2] = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[1] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));

	cint[0] = iw->d.cs;
	iw->d.cs = left->wall->nextsector;
	cint[3] = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	cint[4] = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].next->x, iw->walls[left->wall->nextsector_wall].next->y);
	if (left->zu < cint[3])
	{
		cfloat[5] = (right->zu - left->zu) / d.len_lr;
		cint[3] = left->zu;
	}
	else
	{
		cint[5] = get_ceil_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
		cfloat[5] = (cint[5] - cint[3]) / d.len_lr;
	}
	if (left->zd > cint[4])
	{
		cfloat[6] = (right->zd - left->zd) / d.len_lr;
		cint[4] = left->zd;
	}
	else
	{
		cint[5] = get_floor_z(iw, iw->walls[left->wall->nextsector_wall].x, iw->walls[left->wall->nextsector_wall].y);
		cfloat[6] = (cint[5] - cint[4]) / d.len_lr;
	}
	iw->d.cs = cint[0];

	cint[0] = left->x;
	cint[5] = WINDOW_W;
	cfloat[3] = left->olen;
	cfloat[4] = iw->tsz[left->wall->glass];

	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.save_top_betw, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.save_bot_betw, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 6 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 7 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_glass_tex_kernel", &iw->k.ret);
	//printf("Create_kernel_fci_ret %d\n", iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_save_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_save_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[left->wall->glass]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_wallTop);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_wallBot);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("GLASSSS %d\n", iw->k.ret);

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
}

int		draw_picture_kernel(t_sdl *iw, t_picture *pic)
{
	t_draw_picture	d;
	int		cint[8];
	float	cfloat[6];

	cint[1] = iw->t[pic->t]->h;
	cint[4] = iw->t[pic->t]->w;
	cint[5] = WINDOW_W;
	cint[6] = iw->t[pic->t]->format->BytesPerPixel;
	cint[7] = iw->t[pic->t]->pitch;

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x0 - iw->p.x), (float)(pic->y0 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx0 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		return (0);

	d.lang = get_vectors_angle(iw->d.left_point.x - (float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	d.rang = get_vectors_angle(iw->d.right_point.x - (float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(pic->x1 - iw->p.x), (float)(pic->y1 - iw->p.y));
	if (d.rang < iw->v.angle * 2)
		d.rx1 = (int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	else
		d.rx1 = -(int)(d.lang * (float)WINDOW_W / (2.0f * iw->v.angle));

	if (d.rx1 >= WINDOW_W)
		return (0);
	d.plen = fabsf(iw->d.screen.a * (float)pic->x0 + iw->d.screen.b * (float)pic->y0 + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);
	d.ry0_up = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	d.ry0_down = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	d.plen = fabsf(iw->d.screen.a * (float)pic->x1 + iw->d.screen.b * (float)pic->y1 + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a + iw->d.screen.b * iw->d.screen.b);

	cint[3] = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zu) / (int)(d.plen + 1) + iw->p.rotup;
	cint[2] = WINDOW_H * (iw->p.z + (int)d.plen / 2 - pic->zd) / (int)(d.plen + 1) + iw->p.rotup;

	cfloat[0] = 0.0f;
	d.down = d.ry0_down - cint[2];
	d.up = d.ry0_up - cint[3];
	d.dx = d.rx0 - d.rx1;
	cint[0] = d.rx1;
	cfloat[2] = d.down / d.dx;
	cfloat[3] = d.down / d.dx;
	cfloat[4] = d.up / d.dx;
	cfloat[5] = d.up / d.dx;
	cfloat[1] = (float)iw->t[pic->t]->w / d.dx;
	if (cint[0] < 0)
	{
		cfloat[0] += cfloat[1] * (float)abs(cint[0]);
		//iw->t[pic->t]->w * abs(cint[0]) / d.dx;
		cfloat[2] += cfloat[3] * (float)abs(cint[0]);
		//(float)(d.down / d.dx * abs(cint[0]));
		cfloat[4] += cfloat[5] * (float)abs(cint[0]);
		//(float)(d.up / d.dx * abs(cint[0]));
		cint[0] = 0;
	}

	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 8 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cfloat, CL_TRUE, 0, 6 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_picture_kernel", &iw->k.ret);
	//printf("draw_picture_kernel_ret %d\n", iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_save_top2);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_save_bottom2);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&iw->k.m_t[pic->t]);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_cint);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&iw->k.m_cfloat);

	//while(i++ <= d.rx0 && i <= WINDOW_W)
	size_t global_item_size;
	if (d.rx0 < WINDOW_W)
		global_item_size = d.rx0 - cint[0];
	else
		global_item_size = WINDOW_W - cint[0];
	//printf("global %zu\n", global_item_size);
	size_t local_item_size = 1;

	if (global_item_size <= WINDOW_W)
		iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
			&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("GLASSSS %d\n", iw->k.ret);

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
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

void	draw_sprite_kernel(t_sdl *iw, t_sprite *sprite)
{
	int		cint[12];

	sprite->spriteheight = 2 * sprite->spritewidth * sprite->t->h / sprite->t->w;
	sprite->ey = WINDOW_H * (iw->p.z + (int)sprite->pplen / 2 - sprite->z) / (int)sprite->pplen + iw->p.rotup;
	sprite->sy = sprite->ey - sprite->spriteheight;

	cint[0] = sprite->sx;
	cint[1] = sprite->spritewidth;
	cint[2] = sprite->t->w;
	cint[3] = sprite->sy;
	cint[4] = sprite->ey;
	cint[5] = WINDOW_H;
	cint[6] = sprite->spriteheight;
	cint[7] = sprite->t->h;
	cint[8] = WINDOW_W;
	cint[9] = sprite->t->format->BytesPerPixel;
	cint[10] = sprite->t->pitch;
	if (sprite->sx < 0)
		cint[11] = -sprite->sx;
	else
		cint[11] = 0;
		
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_cint, CL_TRUE, 0, 12 * sizeof(int), cint, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_top, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), sprite->top, 0, NULL, NULL);
	iw->k.ret = clEnqueueWriteBuffer(iw->k.command_queue, iw->k.m_bottom, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), sprite->bottom, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "draw_sprite_kernel", &iw->k.ret);

	iw->k.ret = clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&iw->k.m_top);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&iw->k.m_bottom);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&iw->k.m_sur);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)sprite->t_kernel);
	iw->k.ret = clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&iw->k.m_cint);

	size_t global_item_size = get_min(WINDOW_W, sprite->ex) - get_max(0, sprite->sx);//sprite->ex - sprite->sx;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	
	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
}

void	draw_sprites_kernel(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (iw->sectors[tmp1->num_sec].visited && tmp1->draweble)
			draw_sprite_kernel(iw, tmp1);
		tmp1 = tmp1->next;
	}
}