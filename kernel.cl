//int
//0 - wall_width
//1 - wall_height
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - floorA
//12 - floorB
//13 - floorC
//14 - floorD
//15 - ceilA
//16 - ceilB
//17 - ceilC
//18 - ceilD
//19 - left_zu
//20 - left_zd

//21 - screen_left
//22 - screen_right

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py
//13 - left_olen
//14 - wall_tsz
//15 - zudiff
//16 - zddiff

// top && bottom + left->x

__kernel void draw_inclined_wall_floor_ceil_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *wallpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	int	frcoef;
	int	clcoef;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;
	float	tx;
	float	ty;
	float	dty;
	float	zu;
	float	zd;

	//printf("1");
	j = get_global_id(0);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[21] ||
		cint[8] + j >= cint[22])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	frcoef = (cint[15] * (int)rx + cint[16] * (int)ry + cint[18]) / cint[17] * -1 -
		(cint[11] * (int)rx + cint[12] * (int)ry + cint[14]) / cint[13] * -1;
	clcoef = frcoef;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]);
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist * frcoef / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			frcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
				(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + frcoef) / 2.0f;
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}

	if (wallTop[j] > top[j])
	{
		if (wallTop[j] < bottom[j])
			i = wallTop[j] + 1;
		else
			i = bottom[j] + 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[10] * (float)(wallTop[j] - i + 1);
		while (--i >= top[j])
		{
			weight = wall_dist * clcoef / k;
			k += cfloat[10];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			clcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
				(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + clcoef) / 2.0f;
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
			wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
		}
		top[j] = wallTop[j];
	}

	tx = (cfloat[13] + left_len) * (float)cint[0] * cfloat[14] / 1000.0f;
	zu = (float)cint[19] + left_len * cfloat[15];
	zd = (float)cint[20] + left_len * cfloat[16];
	if (wallTop[j] < top[j])
		ty = zu + cfloat[14] * (zu - zd) * (float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
	else
		ty = zu;
	ty = ty * (float)cint[1] / 1000.0f;

	dty = ((zu - zd) * (float)cint[1] / 1000.0f) /
		(float)(wallBot[j] - wallTop[j]) * cfloat[14];
	i = top[j] - 1;
	while (++i < bottom[j])
	{
		tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
		wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
		ty += dty;
	}
	top[j] = cint[7] + 1;
}

//int
//0 - wall_width
//1 - wall_height
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - frcoef

//12 - screen_left
//13 - screen_right

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py
//13 - left_olen
//14 - wall_tsz
//15 - zu
//16 - zd

// top && bottom + left->x

__kernel void draw_wall_floor_ceil_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *wallpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;
	float	tx;
	float	ty;
	float	dty;

	//printf("2");
	j = get_global_id(0);
	//printf("j = %d\n", j);
	// printf("wallBot[%d] %d\n", j, wallBot[999]);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[12] ||
		cint[8] + j >= cint[13])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]) * (float)cint[11];
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}
	
	if (wallTop[j] > top[j])
	{
		if (wallTop[j] < bottom[j])
			i = wallTop[j];
		else
			i = bottom[j];
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[10] * (float)(wallTop[j] - i + 1);
		while (--i >= top[j])
		{
			weight = wall_dist / k;
			k += cfloat[10];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
			wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
		}
		top[j] = wallTop[j];
	}

	tx = (cfloat[13] + left_len) * (float)cint[0] * cfloat[14] / 1000.0f;
	if (wallTop[j] < top[j])
		ty = (float)cfloat[15] + cfloat[14] * (float)(cfloat[15] - cfloat[16]) * (float)(top[j] - wallTop[j]) /
		(float)(wallBot[j] - wallTop[j]);
	else
		ty = (float)cfloat[15];
	ty = ty * (float)cint[1] / 1000.0f;

	dty = ((float)(cfloat[15] - cfloat[16]) * (float)cint[1] / 1000.0f) /
		(float)(wallBot[j] - wallTop[j]) * cfloat[14];
	i = top[j] - 1;
	while (++i < bottom[j])
	{
		tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
		wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
		ty += dty;
	}
	top[j] = cint[7] + 1;
}

//int
//0 - wall_width - !NULL
//1 - wall_height - !NULL
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - floorA
//12 - floorB
//13 - floorC
//14 - floorD
//15 - ceilA
//16 - ceilB
//17 - ceilC
//18 - ceilD
//19 - left_zu
//20 - left_zd

//21 - screen_left
//22 - screen_right

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py
//13 - left_olen
//14 - wall_tsz
//15 - zudiff
//16 - zddiff

// top && bottom + left->x

__kernel void draw_inclined_floor_ceil_betw_walls_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *wallpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat,
	__global const int *top_betw, __global const int *bottom_betw
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	int		frcoef;
	int		clcoef;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;
	float	tx;
	float	ty;
	float	dty;
	float	zu;
	float	zd;

	//printf("3");
	j = get_global_id(0);
	//printf("wallBot[%d] %d\n", j, bottom_betw[j]);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[21] ||
		cint[8] + j >= cint[22])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	frcoef = (cint[15] * (int)rx + cint[16] * (int)ry + cint[18]) / cint[17] * -1 -
		(cint[11] * (int)rx + cint[12] * (int)ry + cint[14]) / cint[13] * -1;
	clcoef = frcoef;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]);
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist * frcoef / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			frcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
				(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + frcoef) / 2.0f;
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}

	if (wallTop[j] > top[j])
	{
		if (wallTop[j] < bottom[j])
			i = wallTop[j] + 1;
		else
			i = bottom[j] + 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[10] * (float)(wallTop[j] - i + 1);
		while (--i >= top[j])
		{
			weight = wall_dist * clcoef / k;
			k += cfloat[10];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			clcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
				(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + clcoef) / 2.0f;
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
			wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
		}
		top[j] = wallTop[j];
	}

	// tx = (cfloat[13] + left_len) * (float)cint[0] * cfloat[14] / 1000.0f;
	// zu = (float)cint[19] + left_len * cfloat[15];
	// zd = (float)cint[20] + left_len * cfloat[16];
	// if (wallTop[j] < top[j])
	// 	ty = zu + cfloat[14] * (zu - zd) * (float)(top[j] - wallTop[j]) /
	// 		(float)(wallBot[j] - wallTop[j]);
	// else
	// 	ty = zu;
	// ty = ty * (float)cint[1] / 1000.0f;

	// dty = ((zu - zd) * (float)cint[1] / 1000.0f) /
	// 	(float)(wallBot[j] - wallTop[j]) * cfloat[14];
	// i = top[j] - 1;
	// while (++i < bottom[j])
	// {
	// 	tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
	// 	wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
	// 	ty += dty;
	// }
	// top[j] = cint[7] + 1;

	if (bottom[j] > bottom_betw[j] || top[j] < top_betw[j])
	{
		zu = (float)cint[19] + left_len * cfloat[15];
		zd = (float)cint[20] + left_len * cfloat[16];
		tx = (cfloat[13] + left_len) *
			(float)cint[0] * cfloat[14] / 1000.0f;
		dty = ((float)(zu - zd) *
			(float)cint[1] / 1000.0f) /
			(float)(wallBot[j] - wallTop[j]) * cfloat[14];
	}
	
	if (bottom[j] > bottom_betw[j])
	{
		if (wallTop[j] < bottom_betw[j])
			ty = zu + cfloat[14] *
			(zu - zd) *
			(float)(bottom_betw[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = zu;
		ty = ty * (float)cint[1] / 1000.0f;
		if (bottom_betw[j] < top[j])
			i = top[j] - 1;
		else
			i = bottom_betw[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		bottom[j] = bottom_betw[j];
	}

	if (top[j] < top_betw[j])
	{
		if (wallTop[j] < top_betw[j])
			ty = zu + cfloat[14] *
			(zu - zd) *
			(float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = zu;
		ty = ty * (float)cint[1] / 1000.0f;
		i = top[j] - 1;
		while (++i < top_betw[j] && i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		top[j] = top_betw[j];
	}
	
}

//int
//0 - wall_width - !NULL
//1 - wall_height - !NULL
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - frcoef

//12 - screen_left
//13 - screen_right

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py
//13 - left_olen
//14 - wall_tsz
//15 - zu
//16 - zd

//17 - screen_left
//18 - screen_right

// top && bottom + left->x

__kernel void draw_floor_ceil_betw_walls_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *wallpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat,
	__global const int *top_betw, __global const int *bottom_betw
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;
	float	tx;
	float	ty;
	float	dty;

	//printf("4");
	j = get_global_id(0);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[12] ||
		cint[8] + j >= cint[13])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]) * (float)cint[11];
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}

	if (wallTop[j] > top[j])
	{
		if (wallTop[j] < bottom[j])
			i = wallTop[j] + 1;
		else
			i = bottom[j] + 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[10] * (float)(wallTop[j] - i + 1);
		while (--i >= top[j])
		{
			weight = wall_dist / k;
			k += cfloat[10];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
			wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
		}
		top[j] = wallTop[j];
	}

	if (bottom[j] > bottom_betw[j] || top[j] < top_betw[j])
	{
		tx = (cfloat[13] + left_len) *
			(float)cint[0] * cfloat[14] / 1000.0f;
		dty = ((float)(cfloat[15] - cfloat[16]) *
			(float)cint[1] / 1000.0f) /
			(float)(wallBot[j] - wallTop[j]) * cfloat[14];
	}

	if (bottom[j] > bottom_betw[j])
	{
		if (wallTop[j] < bottom_betw[j])
			ty = cfloat[15] + cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(bottom_betw[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = ty * (float)cint[1] / 1000.0f;
		if (bottom_betw[j] < top[j])
			i = top[j] - 1;
		else
			i = bottom_betw[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		bottom[j] = bottom_betw[j];
	}

	if (top[j] < top_betw[j])
	{
		if (wallTop[j] < top_betw[j])
			ty = cfloat[15] + cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = ty * (float)cint[1] / 1000.0f;
		i = top[j] - 1;
		while (++i < top_betw[j] && i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		top[j] = top_betw[j];
	}
}

//int
	//0 - wall_width - NULL
	//1 - wall_height - NULL
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - frcoef
//12 - screen_left
//13 - screen_right

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py
	//13 - left_olen
	//14 - wall_tsz
	//15 - zu
	//16 - zd


// top && bottom + left->x

__kernel void draw_floor_ceil_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;
	//float	tx;
	//float	ty;
	//float	dty;

	//printf("5");
	j = get_global_id(0);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[12] ||
		cint[8] + j >= cint[13])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]) * (float)cint[11];
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}

	if (wallTop[j] > top[j])
	{
		if (wallTop[j] < bottom[j])
			i = wallTop[j] + 1;
		else
			i = bottom[j] + 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[10] * (float)(wallTop[j] - i + 1);
		while (--i >= top[j])
		{
			weight = wall_dist / k;
			k += cfloat[10];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
			wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
		}
		top[j] = wallTop[j];
	}

	/*if (bottom[j] > bottom_betw[j] || top[j] < top_betw[j])
	{
		tx = (cfloat[13] + left_len) *
			(float)cint[0] * cfloat[14] / 1000.0f;
		dty = ((float)(cfloat[15] - cfloat[16]) *
			(float)cint[1] / 1000.0f) /
			(float)(wallBot[j] - wallTop[j]) * cfloat[14];
	}

	if (bottom[j] > bottom_betw[j])
	{
		if (wallTop[j] < bottom_betw[j])
			ty = cfloat[15] + cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(bottom_betw[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = ty * (float)cint[1] / 1000.0f;

		i = bottom_betw[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		bottom[j] = bottom_betw[j];
	}

	if (top[j] < top_betw[j])
	{
		if (wallTop[j] < top_betw[j])
			ty = cfloat[15] + cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = ty * (float)cint[1] / 1000.0f;
		i = top[j] - 1;
		while (++i < top_betw[j] && i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = (int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16);
			ty += dty;
		}
		top[j] = top_betw[j];
	}*/
}

//int
	//0 - wall_width - NULL
	//1 - wall_height - NULL
//2 - floor_width
//3 - floor_height
//4 - ceil_width
//5 - ceil_height
//6 - WINDOW_W
//7 - WINDOW_H
//8 - left_x
//9 - left_px
//10 - left_py
//11 - floorA
//12 - floorB
//13 - floorC
//14 - floorD
//15 - ceilA
//16 - ceilB
//17 - ceilC
//18 - ceilD
	//19 - left_zu
	//20 - left_zd
//21 - screen_left - 19
//22 - screen_right - 20

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - d.rv.x
//4 - d.rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_len
//9 - frpl
//10 - clpl
//11 - px
//12 - py
	//13 - left_olen
	//14 - wall_tsz
	//15 - zudiff
	//16 - zddiff

// top && bottom + left->x

__kernel void draw_inclined_floor_ceil_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels,
	__global const uchar *floorpixels, __global const uchar *ceilpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		i;
	int		j;
	int		tp;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	int		frcoef;
	int		clcoef;
	float	wall_dist;
	float	k;
	float	weight;
	float	floorx;
	float	floory;
	//float	tx;
	//float	ty;
	//float	dty;
	//float	zu;
	//float	zd;

	//printf("6");
	j = get_global_id(0);
	top += cint[8];
	bottom += cint[8];
	if (top[j] >= bottom[j] || cint[8] + j < cint[19] ||
		cint[8] + j >= cint[20])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[9] + cfloat[3] * left_len;
	ry = (float)cint[10] + cfloat[4] * left_len;
	frcoef = (cint[15] * (int)rx + cint[16] * (int)ry + cint[18]) / cint[17] * -1 -
		(cint[11] * (int)rx + cint[12] * (int)ry + cint[14]) / cint[13] * -1;
	clcoef = frcoef;
	wall_dist = (float)cint[7] * cfloat[8] /
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[7]);
	if (wall_dist < 0)
		wall_dist *= -1.0f;

	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < bottom[j])
	{
		if (wallBot[j] < top[j])
			i = top[j] - 1;
		else
			i = wallBot[j] - 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[9] * (float)(i + 1 - wallBot[j]);
		while (++i < bottom[j])
		{
			weight = wall_dist * frcoef / k;
			k += cfloat[9];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			frcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
				(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + frcoef) / 2.0f;
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[2]) % cint[2]) + cint[2] - 1) : ((int)(floorx * (float)cint[2]) % cint[2])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[3]) % cint[3]) + cint[3] - 1) : ((int)(floory * (float)cint[3]) % cint[3])) * 3 * cint[2];
			wpixels[cint[8] + j + i * cint[6]] = (int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16);
		}
		bottom[j] = wallBot[j];
	}

	if (wallTop[j] > top[j])
	{
		if (wallTop[j] < bottom[j])
			i = wallTop[j] + 1;
		else
			i = bottom[j] + 1;
		k = (float)(wallBot[j] - wallTop[j]) +
			cfloat[10] * (float)(wallTop[j] - i + 1);
		while (--i >= top[j])
		{
			weight = wall_dist * clcoef / k;
			k += cfloat[10];
			floorx = weight * rx + (1.0f - weight) * cfloat[11];
			floory = weight * ry + (1.0f - weight) * cfloat[12];
			clcoef = ((cint[15] * (int)(floorx * 1000.0f) + cint[16] * (int)(floory * 1000.0f) + cint[18]) / cint[17] * -1 -
				(cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 + clcoef) / 2.0f;
			tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
				+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
			wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
		}
		top[j] = wallTop[j];
	}
}