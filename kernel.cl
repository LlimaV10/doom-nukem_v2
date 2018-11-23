//int
// 0 - twidth
// 1 - left_x
// 2 - left_zu
// 3 - left_zd
// 4 - WINDOW_W
// 5 - theight

//float
// 0 - ang
// 1 - dang
// 2 - lenpl
// 3 - sing
// 4 - left_olen
// 5 - tsz
// 6 - zudiff
// 7 - zddiff

__kernel void draw_wall_tex(
	__global int *top, __global int const *bottom,
	__global int *wpixels, __global const uchar *tpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat)
{
	int		j;
	int		i;
	float	left_len;
	float	nang;
	float	tx;
	float	ty;
	float	dty;
	float	zu;
	float	zd;
	int		tp;

	j = get_global_id(0);
	if (top[cint[1] + j] >= bottom[cint[1] + j])
		return;
	nang = cfloat[1] * (float)j;
	left_len = sin(nang) * cfloat[2] / sin(cfloat[3] - nang);
	tx = (cfloat[4] + left_len) * (float)cint[0] * cfloat[5] / 1000.0f;
	zu = (float)cint[2] + left_len * cfloat[6];
	zd = (float)cint[3] + left_len * cfloat[7];
	if (wallTop[j] < top[j + cint[1]])
		ty = zu + cfloat[5] * (zu - zd) * (float)(top[j + cint[1]] - wallTop[j]) /
		(float)(wallBot[j] - wallTop[j]);
	else
		ty = zu;
	ty = ty * (float)cint[5] / 1000.0f;
	dty = ((zu - zd) * (float)cint[5] / 1000.0f) /
		(float)(wallBot[j] - wallTop[j]) * cfloat[5];
	i = top[cint[1] + j] - 1;
	while (++i < bottom[cint[1] + j])
	{
		tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[5]) * 3 * cint[0];
		wpixels[cint[1] + j + i * cint[4]] = (int)(tpixels[tp] | tpixels[tp + 1] << 8 | tpixels[tp + 2] << 16);
		ty += dty;
	}
	top[cint[1] + j] = bottom[cint[1] + j];
}

//int
//0 - left_x
//1 - left_px
//2 - left_py
//3 - WINDOW_H
//4 - twidth
//5 - theight
//6 - WINDOW_W

//5 - floorA 7
//6 - floorB 8
//7 - floorC 9
//8 - floorD 10
//9  - ceilA 11
//10 - ceilB 12
//11 - ceilC 13
//12 - ceilD 14

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - rv.x
//4 - rv.y
//5 - screenA
//6 - screenB
//7 - screenC
//8 - screen_equaton
//9 - pl
//10 - px
//11 - py

__kernel void draw_inclined_floor_tex(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *tpixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		j;
	int		i;
	float	left_len;
	float	nang;
	float	rx;
	float	ry;
	float	coef;
	float	wall_dist;
	float	weight;
	float	k;
	float	floorx;
	float	floory;
	int		tp;

	j = get_global_id(0);
	if (wallBot[j] >= bottom[cint[0] + j] ||
		top[cint[0] + j] >= bottom[cint[0] + j])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	rx = (float)cint[1] + cfloat[3] * left_len;
	ry = (float)cint[2] + cfloat[4] * left_len;
	coef = (cint[11] * (int)rx + cint[12] * (int)ry + cint[14]) / cint[13] * -1 -
		(cint[7] * (int)rx + cint[8] * (int)ry + cint[10]) / cint[9] * -1;
	wall_dist = (float)cint[3] / ((cfloat[5] * rx + cfloat[6] * ry + cfloat[7]) / cfloat[8]);
	if (wall_dist < 0)
		wall_dist = -wall_dist;
	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < top[cint[0] + j])
		i = top[cint[0] + j] - 1;
	else
		i = wallBot[j] - 1;
	k = (float)(wallBot[j] - wallTop[j]) + cfloat[9] * (float)(i + 1 - wallBot[j]);
	while (++i < bottom[cint[0] + j])
	{
		weight = wall_dist * coef / k;
		k += cfloat[9];
		floorx = weight * rx + (1.0f - weight) * cfloat[10];
		floory = weight * ry + (1.0f - weight) * cfloat[11];
		coef = ((cint[11] * (int)(floorx * 1000.0f) + cint[12] * (int)(floory * 1000.0f) + cint[14]) / cint[13] * -1 -
			(cint[7] * (int)(floorx * 1000.0f) + cint[8] * (int)(floory * 1000.0f) + cint[10]) / cint[9] * -1 + coef) / 2.0f;
		tp = ((floorx < 0.0f) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
			+ ((floory < 0.0f) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
		wpixels[cint[0] + j + i * cint[6]] = (int)(tpixels[tp] | tpixels[tp + 1] << 8 | tpixels[tp + 2] << 16);
	}
	if (wallBot[j] < bottom[cint[0] + j])
		bottom[cint[0] + j] = wallBot[j];
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

	j = get_global_id(0);
	if (top[j] >= bottom[j])
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

////11 - floorA
////12 - floorB
////13 - floorC
////14 - floorD
////15 - ceilA
////16 - ceilB
////17 - ceilC
////18 - ceilD
////19 - left_zu - 11
////20 - left_zd - 12

//11 - frcoef

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

////15 - zudiff
////16 - zddiff

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

	j = get_global_id(0);
	if (top[j] >= bottom[j])
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
	// float	tx;
	// float	ty;
	// float	dty;
	// float	zu;
	// float	zd;

	j = get_global_id(0);
	if (top[j] >= bottom[j])
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
}