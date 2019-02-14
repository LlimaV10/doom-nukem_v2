int		get_light_color(int color, int light)
{
	if (light == 1)
		return (color);
	else
	return ((((int)((float)(color >> 16) * 0.2f)) << 16) +
		(((int)((float)((color >> 8) - (color >> 16 << 8))
		* 0.2f)) << 8) + (int)((float)(color - (color
		>> 8 << 8)) * 0.2f));
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

//21 - screen_left
//22 - screen_right

//23 - ceil->t
//24 - light

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
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16), cint[24]);
		}
		bottom[j] = wallBot[j];
	}

	if (wallTop[j] > top[j] && cint[23] >= 0)
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
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16), cint[24]);
		}
		top[j] = wallTop[j];
	}

	if (cint[0] > 0)
	{
		tx = (cfloat[13] + left_len) * (float)cint[0] * cfloat[14] / 1000.0f;
		zu = (float)cint[19] + left_len * cfloat[15];
		zd = (float)cint[20] + left_len * cfloat[16];
		dty = ((zu - zd) * (float)cint[1] / 1000.0f) /
			(float)(wallBot[j] - wallTop[j]) * cfloat[14];
		//invzu = (float)((int)invzu - ((int)invzu % 1000) + 1000) - invzu;
		//invzd = (float)((int)invzd - ((int)invzd % 1000) + 1000) - invzd;
		if (wallTop[j] < top[j])
			ty = zu - (zu - zd) * (float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = zu;
		ty = (float)((int)ty - ((int)ty % (int)(1000.0f / cfloat[14])) +
			(int)(1000.0f / cfloat[14])) - ty;
		ty = ty * (float)cint[1] / (1000.0f / cfloat[14]);
		//i = top[j] - 1;
		if (wallTop[j] < top[j])
			i = top[j] - 1;
		else
			i = wallTop[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16), cint[24]);
			ty += dty;
		}
		//top[j] = cint[7] + 1;
		if (wallTop[j] < top[j] && top[j] < bottom[j])
			bottom[j] = top[j] - 1;
		else if (wallTop[j] >= top[j] && wallTop[j] < bottom[j])
			bottom[j] = wallTop[j] - 1;
	}
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
//14 - ceil->t

//15 - light

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
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16), cint[15]);
		}
		bottom[j] = wallBot[j];
	}
	
	if (wallTop[j] > top[j] && cint[14] >= 0)
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
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16), cint[15]);
		}
		top[j] = wallTop[j];
	}

	if (cint[0] > 0)
	{
		tx = (cfloat[13] + left_len) * (float)cint[0] * cfloat[14] / 1000.0f;
		if (wallTop[j] < top[j])
			ty = (float)cfloat[15] - cfloat[14] * (float)(cfloat[15] - cfloat[16]) * (float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = (float)cfloat[15];
		ty = (float)((int)ty - ((int)ty % 1000) + 1000) - ty;
		ty = ty * (float)cint[1] / 1000.0f;
		dty = ((float)(cfloat[15] - cfloat[16]) * (float)cint[1] / 1000.0f) /
			(float)(wallBot[j] - wallTop[j]) * cfloat[14];
		//i = top[j] - 1;
		if (wallTop[j] < top[j])
			i = top[j] - 1;
		else
			i = wallTop[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16), cint[15]);
			ty += dty;
		}
		//top[j] = cint[7] + 1;
		if (wallTop[j] < top[j] && top[j] < bottom[j])
			bottom[j] = top[j] - 1;
		else if (wallTop[j] >= top[j] && wallTop[j] < bottom[j])
			bottom[j] = wallTop[j] - 1;
	}
}

//int
//0 - wall_width - !NULL
//1 - wall_height - !NULL
//2 - floor_width
//3 - floor_height
//4 - ceil_width || skybox
//5 - ceil_height || skybox
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

//23 - ceil->t
//24 - rotup

//25 - light

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

//17 - p.rot
//18 - v.angle

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

	float	rot;
	float	sky_x;
	float	sky_y;
	float	dy;

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
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16), cint[25]);
		}
		bottom[j] = wallBot[j];
	}

	if (cint[23] < 0)
	{
		rot = (cfloat[17] - cfloat[18]) + (float)(cint[8] + j) /
			(float)cint[6] * cfloat[18] * 2.0f;
		if (rot < 0.0f)
			rot += 6.2831852f;
		else if (rot > 6.2831852f)
			rot -= 6.2831852f;
		sky_x = rot * (float)cint[4] / 6.2831852f;
		dy = (float)cint[5] / (float)(4 * cint[7]);
	}

	if (wallTop[j] > top[j])
	{
		if (cint[23] >= 0)
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
				wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16), cint[25]);
			}
			
		}
		else
		{
			sky_y = ((-cint[24] + 2 * cint[7]) * cint[5]) / (4 * cint[7]) +
				dy * top[j];
			i = top[j] - 1;
			while (++i < wallTop[j] && i < bottom[j])
			{
				if ((int)sky_y >= cint[5])
					sky_y -= (float)cint[5];
				tp = (int)sky_x * 3 + (int)sky_y * 3 * cint[4];
				wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);

				sky_y += dy;
			}
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
			ty = zu - (zu - zd) *
			(float)(bottom_betw[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = zu;
		ty = (float)((int)ty - ((int)ty % (int)(1000.0f / cfloat[14])) +
			(int)(1000.0f / cfloat[14])) - ty;
		ty = ty * (float)cint[1] / 1000.0f * cfloat[14];
		if (bottom_betw[j] < top[j])
		{
			i = top[j] - 1;
			ty += dty * (top[j] - bottom_betw[j]);
		}
		else
			i = bottom_betw[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16), cint[25]);
			ty += dty;
		}
		bottom[j] = bottom_betw[j];
	}

	if (top[j] < top_betw[j])
	{
		if (wallTop[j] < top_betw[j])
			ty = zu - (zu - zd) *
			(float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = zu;
		ty = (float)((int)ty - ((int)ty % (int)(1000.0f / cfloat[14])) +
			(int)(1000.0f / cfloat[14])) - ty;
		ty = ty * (float)cint[1] / 1000.0f * cfloat[14];
		i = top[j] - 1;
		while (++i < top_betw[j] && i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16), cint[25]);
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

//14 - ceil->t
//15 - rotup
//16 - light

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

//17 - p.rot
//18 - v.angle

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

	float	rot;
	float	sky_x;
	float	sky_y;
	float	dy;

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
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(floorpixels[tp] | floorpixels[tp + 1] << 8 | floorpixels[tp + 2] << 16), cint[16]);
		}
		bottom[j] = wallBot[j];
	}

	if (cint[14] < 0)
	{
		rot = (cfloat[17] - cfloat[18]) + (float)(cint[8] + j) /
			(float)cint[6] * cfloat[18] * 2.0f;
		if (rot < 0.0f)
			rot += 6.2831852f;
		else if (rot > 6.2831852f)
			rot -= 6.2831852f;
		sky_x = rot * (float)cint[4] / 6.2831852f;
		dy = (float)cint[5] / (float)(4 * cint[7]);
	}

	if (wallTop[j] > top[j])
	{
		if (cint[14] >= 0)
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
				wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16), cint[16]);
			}
		}
		else
		{
			sky_y = ((-cint[15] + 2 * cint[7]) * cint[5]) / (4 * cint[7]) +
				dy * top[j];
			i = top[j] - 1;
			while (++i < wallTop[j] && i < bottom[j])
			{
				if ((int)sky_y >= cint[5])
					sky_y -= (float)cint[5];
				tp = (int)sky_x * 3 + (int)sky_y * 3 * cint[4];
				wpixels[cint[8] + j + i * cint[6]] = (int)(ceilpixels[tp] | ceilpixels[tp + 1] << 8 | ceilpixels[tp + 2] << 16);
				sky_y += dy;
			}
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
			ty = cfloat[15] - cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(bottom_betw[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = (float)((int)ty - ((int)ty % 1000) + 1000) - ty;
		ty = ty * (float)cint[1] / 1000.0f;
		if (bottom_betw[j] < top[j])
		{
			i = top[j] - 1;
			ty += dty * (top[j] - bottom_betw[j]);
		}
		else
			i = bottom_betw[j] - 1;
		while (++i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16), cint[16]);
			ty += dty;
		}
		bottom[j] = bottom_betw[j];
	}

	if (top[j] < top_betw[j])
	{
		if (wallTop[j] < top_betw[j])
			ty = cfloat[15] - cfloat[14] *
			(cfloat[15] - cfloat[16]) *
			(float)(top[j] - wallTop[j]) /
			(float)(wallBot[j] - wallTop[j]);
		else
			ty = cfloat[15];
		ty = (float)((int)ty - ((int)ty % 1000) + 1000) - ty;
		ty = ty * (float)cint[1] / 1000.0f;
		i = top[j] - 1;
		while (++i < top_betw[j] && i < bottom[j])
		{
			tp = ((int)tx % cint[0]) * 3 + ((int)ty % cint[1]) * 3 * cint[0];
			wpixels[cint[8] + j + i * cint[6]] = get_light_color((int)(wallpixels[tp] | wallpixels[tp + 1] << 8 | wallpixels[tp + 2] << 16), cint[16]);
			ty += dty;
		}
		top[j] = top_betw[j];
	}
}

//int
//0 - WINDOW_H
//1 - WINDOW_W
//2 - sky_width
//3 - sky_height
//4 - screen_left

//float
//0 - sky_y
//1 - dy
//2 - prot
//3 - angle

__kernel void draw_skybox_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *skypixels,
	__global const int *cint, __global const float *cfloat
)
{
	float	sky_x;
	float	rot;
	float	sky_y;
	int		i;
	int		j;
	int		tp;

	j = get_global_id(0) + cint[4];
	rot = cfloat[2] - cfloat[3] + cfloat[3] * 2.0f * (float)j / (float)cint[1];
	if (rot < 0.0f)
		rot += 6.2831852f;
	else if (rot > 6.2831852f)
		rot -= 6.2831852f;
	sky_x = rot * ((float)cint[2]) / 6.2831852f;
	sky_y = cfloat[0] + cfloat[1] * (float)top[j];
	i = top[j] - 1;
	while (++i <= bottom[j] && i < cint[0])
	{
		if ((int)sky_y >= cint[3])
			sky_y -= (float)cint[3];
		tp = (int)sky_x * 3 + (int)sky_y * 3 * cint[2];
		wpixels[j + i * cint[1]] = (int)(skypixels[tp] | skypixels[tp + 1] << 8 | skypixels[tp + 2] << 16);
		sky_y += cfloat[1];
	}
	bottom[j] = top[j];
}


//int
//0 - left->x
//1 - glass_width
//2 - glass_height
//3 - nleft_zu
//4 - nleft_zd
//5 - WINDOW_W
//6 - light

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - olen
//4 - tsz[glass]
//5 - zudiff
//6 - zddiff

__kernel void draw_glass_tex_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *glass_pixels,
	__global const int *wallTop, __global const int *wallBot,
	__global const int *cint, __global const float *cfloat
)
{
	int		i;
	int		j;
	float	nang;
	float	left_len;
	float	tx;
	float	zu;
	float	zd;
	float	dty;
	float	ty;
	int		tp;
	int		pixel;

	j = get_global_id(0);
	top += cint[0];
	bottom += cint[0];
	if (top[j] >= bottom[j])
		return;
	nang = cfloat[0] * (float)j;
	left_len = sin(nang) * cfloat[1] / sin(cfloat[2] - nang);
	tx = (cfloat[3] + left_len) * (float)cint[1] * cfloat[4] / 1000.0f;
	zu = (float)cint[3] + left_len * cfloat[5];
	zd = (float)cint[4] + left_len * cfloat[6];
	dty = ((zu - zd) * (float)cint[2] / 1000.0f) /
		(float)(wallBot[j] - wallTop[j]) * cfloat[4];
	/*ty = (float)((int)zu % (int)(1000.0f / cfloat[4]) +
		(int)(1000.0f / cfloat[4])) - zu;
	ty = ty * (float)cint[1] / (1000.0f / cfloat[4]);
	while (ty < 0)
		ty += (float)cint[2];*/

	ty = 0.0f;
	if (wallTop[j] < top[j])
	{
		ty += (float)(top[j] - wallTop[j]) * dty;
		i = top[j] - 1;
	}
	else
		i = wallTop[j] - 1;

	while (++i < wallBot[j] && i < bottom[j])
	{
		tp = ((int)tx % cint[1]) * 3 + ((int)ty % cint[2]) * cint[1] * 3;
		pixel = (int)(glass_pixels[tp] | glass_pixels[tp + 1] << 8 | glass_pixels[tp + 2] << 16);
		if (pixel != 0x010000)
			wpixels[cint[0] + j + i * cint[5]] = get_light_color(pixel, cint[6]);
		ty += dty;
	}
}

//int
//0 - start_i
//1 - th
//2 - ry1_down
//3 - ry1_up
//4 - tw
//5 - WINDOW_W
//6 - bpp
//7 - pitch
//8 - light

//float
//0 - start_pic_x
//1 - dpic_x
//2 - start_dy_down
//3 - ddy_down
//4 - start_dy_up
//5 - ddy_up


__kernel void draw_picture_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *picture_pixels,
	__global const int *cint, __global const float *cfloat
)
{
	int		tp;
	int		i;
	int		j;
	float	pic_x;
	float	dy_down;
	float	dy_up;
	float	dy_plus;
	float	pic_y;

	i = get_global_id(0) + cint[0];
	if (top[i] >= bottom[i])
		return;
	pic_x = cfloat[0] + cfloat[1] * (float)(i - cint[0]);
	if ((int)pic_x >= cint[4])
		return;
	dy_down = cfloat[2] + cfloat[3] * (float)(i - cint[0]);
	dy_up = cfloat[4] + cfloat[5] * (float)(i - cint[0]);

	dy_plus = (float)cint[1] / (((float)cint[2] + dy_down) -
		((float)cint[3] + dy_up));
	j = cint[3] + (int)dy_up;
	if (j >= top[i])
		pic_y = 0.0f;
	else
	{
		pic_y = dy_plus * (float)(top[i] - j);
		j = top[i];
	}
	while (j++ < cint[2] + (int)dy_down && j < bottom[i] && (int)pic_y < cint[1])
	{
		tp = (int)pic_x * cint[6] + (int)pic_y * cint[7];//* 3 * cint[4];
		tp = (int)(picture_pixels[tp] | picture_pixels[tp + 1] << 8 | picture_pixels[tp + 2] << 16);
		if (tp != 0x010000)
			wpixels[i + j * cint[5]] = get_light_color(tp, cint[8]);
		pic_y += dy_plus;
	}
}

//cint
//0 - sprite->sx
//1 - sprite->spritewidth
//2 - ->t->w
//3 - ->sy
//4 - ->ey
//5 - WINDOW_H
//6 - ->spriteheight
//7 - ->t->h
//8 - WINDOW_W
//9 - bpp
//10 - pitch
//11 - left_plus
//12 - light

__kernel void draw_sprite_kernel(
	__global int *top, __global int *bottom,
	__global int *wpixels, __global const uchar *sprite_pixels,
	__global const int *cint
)
{
	int		i;
	int		j;
	int		stripe;
	int		y;
	float	koef;
	int		texX;
	int		texY;
	int		colour;
	int		tp;

	i = get_global_id(0) + cint[11];
	stripe = i + cint[0];

	j = 0;
	koef = (float)cint[1] * 2.0f / (float)cint[2];
	texX = (int)fabs((float)i / koef);

	if (top[stripe] < bottom[stripe] && top[stripe] != -1)
	{
		y = cint[3] - 1;
		if (y < -1)
		{
			j -= y + 1;
			y = -1;
		}
		while (++y < cint[4] && y < cint[5])
		{
			if (cint[3] < cint[5] && bottom[stripe] > y && top[stripe] < y)
			{
				koef = (float)cint[6] / cint[7];
				texY = (int)(j / koef);
				tp = texX * cint[9] + texY * cint[10];
				colour = (int)(sprite_pixels[tp] | sprite_pixels[tp + 1] << 8 | sprite_pixels[tp + 2] << 16);
				if (colour != 0x010000)
					wpixels[stripe + y * cint[8]] = get_light_color(colour, cint[12]);
			}
			j++;
		}
	}
}

//cint
//0 - start_i
//1 - start_j
//2 - to_j
//3 - changed_rect.x
//4 - changed_rect.y
//5 - rect_w
//6 - rect_h
//7 - tw
//8 - th
//9 - bpp
//10 - pitch
//11 - light
//12 - WINDOW_W

__kernel void draw_gun_kernel(
	__global int *wpixels, __global const uchar *gun,
	__global const int *cint
)
{
	int		i;
	int		wi;
	int		j;
	int		pixel;
	int		tp;

	i = get_global_id(0) + cint[0];
	wi = i * cint[12];
	j = cint[1];
	while (++j < cint[2])
	{
		tp = (j - cint[3]) * cint[7] / cint[5] * cint[9] +
			(i - cint[4]) * cint[8] / cint[6] * cint[10];
		pixel = (int)(gun[tp] | gun[tp + 1] << 8 | gun[tp + 2] << 16);
		if (pixel != 0x010000)
			wpixels[j + wi] = get_light_color(pixel, cint[11]);
	}
}