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
	/*iw->k.program = clCreateProgramWithSource(iw->k.context, 1, (const char **)&iw->k.source_str,
		(const size_t *)&iw->k.source_size, &iw->k.ret);*/
}

void	draw_wall_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_tex	d;
	cl_mem	m_ang;
	cl_mem	m_dang;
	cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_twidth;
	cl_mem	m_left_x;
	cl_mem	m_lenpl;
	cl_mem	m_sing;
	cl_mem	m_left_olen;
	cl_mem	m_left_zu;
	cl_mem	m_left_zd;
	cl_mem	m_tsz;
	cl_mem	m_zudiff;
	cl_mem	m_zddiff;
	cl_mem	m_txs;
	cl_mem	m_zus;
	cl_mem	m_zds;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;

	m_ang = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_dang = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	m_twidth = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(int), NULL, &iw->k.ret);
	m_left_x = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(int), NULL, &iw->k.ret);
	m_lenpl = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_sing = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_left_olen = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_left_zu = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(int), NULL, &iw->k.ret);
	m_left_zd = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(int), NULL, &iw->k.ret);
	m_tsz = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_zudiff = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_zddiff = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, sizeof(float), NULL, &iw->k.ret);
	m_txs = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, len * sizeof(int), NULL, &iw->k.ret);
	m_zus = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, len * sizeof(float), NULL, &iw->k.ret);
	m_zds = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, len * sizeof(float), NULL, &iw->k.ret);

	/*ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
		LIST_SIZE * sizeof(int), A, 0, NULL, NULL);*/
	clEnqueueWriteBuffer(iw->k.command_queue, m_ang, CL_TRUE, 0, sizeof(float), &d.ang, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_dang, CL_TRUE, 0, sizeof(float), &d.dang, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_twidth, CL_TRUE, 0, sizeof(int), &iw->t[left->wall->t]->w, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_left_x, CL_TRUE, 0, sizeof(int), &left->x, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_lenpl, CL_TRUE, 0, sizeof(float), &d.lenpl, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_sing, CL_TRUE, 0, sizeof(float), &d.sing, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_left_olen, CL_TRUE, 0, sizeof(float), &left->olen, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_left_zu, CL_TRUE, 0, sizeof(int), &left->zu, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_left_zd, CL_TRUE, 0, sizeof(int), &left->zd, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_tsz, CL_TRUE, 0, sizeof(float), &iw->tsz[left->wall->t], 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_zudiff, CL_TRUE, 0, sizeof(float), &d.zudiff, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_zddiff, CL_TRUE, 0, sizeof(float), &d.zddiff, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "get_wall_tx_zu_zd", &iw->k.ret);
	printf("Create_kernel_ret %d\n", iw->k.ret);
	//ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&m_ang);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&m_dang);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&m_top);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&m_bottom);
	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&m_twidth);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&m_left_x);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_lenpl);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_sing);
	clSetKernelArg(iw->k.kernel, 8, sizeof(cl_mem), (void *)&m_left_olen);
	clSetKernelArg(iw->k.kernel, 9, sizeof(cl_mem), (void *)&m_left_zu);
	clSetKernelArg(iw->k.kernel, 10, sizeof(cl_mem), (void *)&m_left_zd);
	clSetKernelArg(iw->k.kernel, 11, sizeof(cl_mem), (void *)&m_tsz);
	clSetKernelArg(iw->k.kernel, 12, sizeof(cl_mem), (void *)&m_zudiff);
	clSetKernelArg(iw->k.kernel, 13, sizeof(cl_mem), (void *)&m_zddiff);
	clSetKernelArg(iw->k.kernel, 14, sizeof(cl_mem), (void *)&m_txs);
	clSetKernelArg(iw->k.kernel, 15, sizeof(cl_mem), (void *)&m_zus);
	clSetKernelArg(iw->k.kernel, 16, sizeof(cl_mem), (void *)&m_zds);

	size_t global_item_size = len;
	size_t local_item_size = 1;
	/*ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);*/
	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	printf("kernel run ret %d\n", iw->k.ret);

	int *txs = (int *)malloc(len * sizeof(int));
	/*float *left_lens = (float *)malloc(len * sizeof(float));*/
	float *zus = (float *)malloc(len * sizeof(float));
	float *zds = (float *)malloc(len * sizeof(float));
	/*ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
		LIST_SIZE * sizeof(int), C, 0, NULL, NULL);*/
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_txs, CL_TRUE, 0,
		len * sizeof(int), txs, 0, NULL, NULL);
	printf("txs read ret %d\n", iw->k.ret);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_zus, CL_TRUE, 0,
		len * sizeof(float), zus, 0, NULL, NULL);
	printf("zus read ret %d\n", iw->k.ret);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_zds, CL_TRUE, 0,
		len * sizeof(float), zds, 0, NULL, NULL);
	printf("zds read ret %d\n", iw->k.ret);

	int j = -1;
	while (++j < len)
		printf("tx %d\n", txs[j]);

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);
	/*clReleaseProgram(iw->k.program);*/

	clReleaseMemObject(m_ang);
	clReleaseMemObject(m_dang);
	clReleaseMemObject(m_top);
	clReleaseMemObject(m_bottom);
	clReleaseMemObject(m_twidth);
	clReleaseMemObject(m_left_x);
	clReleaseMemObject(m_lenpl);
	clReleaseMemObject(m_sing);
	clReleaseMemObject(m_left_olen);
	clReleaseMemObject(m_left_zu);
	clReleaseMemObject(m_left_zd);
	clReleaseMemObject(m_tsz);
	clReleaseMemObject(m_zudiff);
	clReleaseMemObject(m_zddiff);
	clReleaseMemObject(m_txs);
	clReleaseMemObject(m_zus);
	clReleaseMemObject(m_zds);

	/*clReleaseCommandQueue(iw->k.command_queue);
	clReleaseContext(iw->k.context);*/
}

void	draw_wall_tex_kernel2(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	t_draw_wall_tex_kernel	d;
	cl_mem	m_top;
	cl_mem	m_bottom;
	cl_mem	m_wpixels;
	cl_mem	m_tpixels;
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	int		cint[6];
	float	cfloat[8];

	cint[0] = iw->t[left->wall->t]->w;
	cint[1] = left->x;
	cint[2] = left->zu;
	cint[3] = left->zd;
	cint[4] = WINDOW_W;
	cint[5] = iw->t[left->wall->t]->h;
	cfloat[4] = left->olen;
	cfloat[5] = iw->tsz[left->wall->t];

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	cfloat[0] = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[1] = cfloat[0] / (float)len;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	cfloat[3] = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	cfloat[2] = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	cfloat[6] = (right->zu - left->zu) / d.len_lr;
	cfloat[7] = (right->zd - left->zd) / d.len_lr;

	m_top = clCreateBuffer(iw->k.context, CL_MEM_READ_WRITE, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	m_bottom = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, (WINDOW_W + 1) * sizeof(int), NULL, &iw->k.ret);
	m_wpixels = clCreateBuffer(iw->k.context, CL_MEM_WRITE_ONLY, WINDOW_W * WINDOW_H * sizeof(int), NULL, &iw->k.ret);
	m_tpixels = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, iw->t[left->wall->t]->w * iw->t[left->wall->t]->h
		* sizeof(int), NULL, &iw->k.ret);
	m_wallTop = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_wallBot = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, len * sizeof(int), NULL, &iw->k.ret);
	m_cint = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 6 * sizeof(int), NULL, &iw->k.ret);
	m_cfloat = clCreateBuffer(iw->k.context, CL_MEM_READ_ONLY, 8 * sizeof(float), NULL, &iw->k.ret);

	clEnqueueWriteBuffer(iw->k.command_queue, m_top, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_bottom, CL_TRUE, 0, (WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0, WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_tpixels, CL_TRUE, 0, iw->t[left->wall->t]->w * iw->t[left->wall->t]->h
		* sizeof(int), iw->t[left->wall->t]->pixels, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallTop, CL_TRUE, 0, len * sizeof(int), iw->d.wallTop, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_wallBot, CL_TRUE, 0, len * sizeof(int), iw->d.wallBot, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cint, CL_TRUE, 0, 6 * sizeof(int), cint, 0, NULL, NULL);
	clEnqueueWriteBuffer(iw->k.command_queue, m_cfloat, CL_TRUE, 0, 8 * sizeof(float), cfloat, 0, NULL, NULL);

	iw->k.kernel = clCreateKernel(iw->k.program, "get_wall_tx_zu_zd2", &iw->k.ret);
	printf("Create_kernel_ret %d\n", iw->k.ret);

	clSetKernelArg(iw->k.kernel, 0, sizeof(cl_mem), (void *)&m_top);
	clSetKernelArg(iw->k.kernel, 1, sizeof(cl_mem), (void *)&m_bottom);
	clSetKernelArg(iw->k.kernel, 2, sizeof(cl_mem), (void *)&m_wpixels);
	clSetKernelArg(iw->k.kernel, 3, sizeof(cl_mem), (void *)&m_tpixels);
	clSetKernelArg(iw->k.kernel, 4, sizeof(cl_mem), (void *)&m_wallTop);
	clSetKernelArg(iw->k.kernel, 5, sizeof(cl_mem), (void *)&m_wallBot);
	clSetKernelArg(iw->k.kernel, 6, sizeof(cl_mem), (void *)&m_cint);
	clSetKernelArg(iw->k.kernel, 7, sizeof(cl_mem), (void *)&m_cfloat);

	size_t global_item_size = len;
	size_t local_item_size = 1;

	iw->k.ret = clEnqueueNDRangeKernel(iw->k.command_queue, iw->k.kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	printf("kernel run ret %d\n", iw->k.ret);

	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_wpixels, CL_TRUE, 0,
		WINDOW_W * WINDOW_H * sizeof(int), iw->sur->pixels, 0, NULL, NULL);
	iw->k.ret = clEnqueueReadBuffer(iw->k.command_queue, m_top, CL_TRUE, 0,
		(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);

	clFlush(iw->k.command_queue);
	clFinish(iw->k.command_queue);
	clReleaseKernel(iw->k.kernel);

	clReleaseMemObject(m_top);
	clReleaseMemObject(m_bottom);
	clReleaseMemObject(m_wpixels);
	clReleaseMemObject(m_tpixels);
	clReleaseMemObject(m_wallTop);
	clReleaseMemObject(m_wallBot);
	clReleaseMemObject(m_cint);
	clReleaseMemObject(m_cfloat);
}

void	draw_wall_tex_kernel3(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len)
{
	int		i;
	int		j;
	t_draw_wall_tex	d;

	d.lv.x = (float)(left->p.x - iw->p.x);
	d.lv.y = (float)(left->p.y - iw->p.y);
	d.rv.x = (float)(right->p.x - iw->p.x);
	d.rv.y = (float)(right->p.y - iw->p.y);
	d.ang = acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.dang = d.ang / (float)len;
	d.ang = 0.0f;
	d.rv.x = (float)(-right->p.x + left->p.x);
	d.rv.y = (float)(-right->p.y + left->p.y);
	d.sing = G180 - acosf((d.lv.x * d.rv.x + d.lv.y * d.rv.y) / (sqrtf(d.lv.x * d.lv.x + d.lv.y * d.lv.y) * sqrtf(d.rv.x * d.rv.x + d.rv.y * d.rv.y)));
	d.lenpl = sqrtf(powf(iw->p.x - left->p.x, 2.0f) + powf(iw->p.y - left->p.y, 2.0f));
	d.len_lr = sqrtf(powf(left->p.x - right->p.x, 2.0f) + powf(left->p.y - right->p.y, 2.0f));
	d.zudiff = (right->zu - left->zu) / d.len_lr;
	d.zddiff = (right->zd - left->zd) / d.len_lr;

	d.left_len = 0.0f;
	d.tx = left->olen * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
	while (d.tx >= (float)iw->t[left->wall->t]->w)
		d.tx -= (float)iw->t[left->wall->t]->w;

	j = -1;
	while (++j < len)
	{
		if (iw->d.top[left->x + j] >= iw->d.bottom[left->x + j])
		{
			d.ang += d.dang;
			continue;
		}
		d.left_len = sinf(d.ang) * d.lenpl / sin(d.sing - d.ang);
		d.tx = (left->olen + d.left_len) * (float)iw->t[left->wall->t]->w * iw->tsz[left->wall->t] / 1000.0f;
		d.zu = (float)left->zu + d.left_len * d.zudiff;
		d.zd = (float)left->zd + d.left_len * d.zddiff;
		if (iw->d.wallTop[j] < iw->d.top[j + left->x])
			d.ty = d.zu + iw->tsz[left->wall->t] * (d.zu - d.zd) * (float)(iw->d.top[j + left->x] - iw->d.wallTop[j]) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]);
		else
			d.ty = d.zu;
		d.ty = d.ty * (float)iw->t[left->wall->t]->h / 1000.0f;
		d.dty = ((d.zu - d.zd) * (float)iw->t[left->wall->t]->h / 1000.0f) / (float)(iw->d.wallBot[j] - iw->d.wallTop[j]) * iw->tsz[left->wall->t];
		i = iw->d.top[left->x + j] - 1;
		while (++i < iw->d.bottom[left->x + j])
		{
			set_pixel(iw->sur, left->x + j, i, get_pixel(iw->t[left->wall->t], (int)d.tx % iw->t[left->wall->t]->w, (int)d.ty % iw->t[left->wall->t]->h));
			d.ty += d.dty;
		}
		iw->d.top[left->x + j] = iw->d.bottom[left->x + j];
		d.ang += d.dang;
	}
}
