



// __kernel void get_wall_tx_zu_zd(__global const float *ang, __global const float *dang,
// 	__global const int *top, __global const int *bottom, __global const int *twidth,
// 	__global const int *left_x, __global const float *lenpl, __global const float *sing,
// 	__global const float *left_olen, __global const int *left_zu,
// 	__global const int *left_zd, __global const float *tsz,
// 	__global const float *zudiff, __global const float *zddiff,
// 	__global int *txs, __global float *zus, __global float *zds)
// {
// 	int j = get_global_id(0);

// 	if (top[*left_x + j] >= bottom[*left_x + j])
// 		return;
// 	float nang = *ang + *dang * (float)j;
// 	float left_len = sin(nang) * *lenpl / sin(*sing - nang);
// 	txs[j] = (*left_olen + left_len) * (float)*twidth * *tsz / 1000.0f;
// 	while (txs[j] > (float)*twidth)
// 		txs[j] -= (float)*twidth;
// 	zus[j] = (float)*left_zu + left_len * *zudiff;
// 	zds[j] = (float)*left_zd + left_len * *zddiff;
// }

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

//float
//0 - dang
//1 - lenpl
//2 - sing
//3 - rv.x
//4 - rv.y
//5 - floorA
//6 - floorB
//7 - floorC
//8 - floorD
//9  - ceilA
//10 - ceilB
//11 - ceilC
//12 - ceilD
//13 - screenA
//14 - screenB
//15 - screenC
//16 - screen_equaton
//17 - pl
//18 - px
//19 - py

__kernel void draw_inclined_floor_tex(
	__global const int *top, __global int *bottom,
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
	coef = (cfloat[9] * rx + cfloat[10] * ry + cfloat[12]) / (-cfloat[11]) -
		(cfloat[5] * rx + cfloat[6] * ry + cfloat[8]) / (-cfloat[7]);
	wall_dist = (float)cint[3] / ((cfloat[13] * rx + cfloat[14] * ry + cfloat[15]) / cfloat[16]);
	if (wall_dist < 0)
		wall_dist = -wall_dist;
	rx /= 1000.0f;
	ry /= 1000.0f;
	if (wallBot[j] < top[cint[0] + j])
		i = top[cint[0] + j] - 1;
	else
		i = wallBot[j] - 1;
	k = (float)(wallBot[j] - wallTop[j]) + cfloat[17] * (float)(i + 1 - wallBot[j]);
	while (++i < bottom[cint[0] + j])
	{
		weight = wall_dist * coef / k;
		k += cfloat[17];
		floorx = weight * rx + (1.0f - weight) * cfloat[18];
		floory = weight * ry + (1.0f - weight) * cfloat[19];
		coef = (cfloat[9] * floorx * 1000.0f + cfloat[10] * floory * 1000.0f + cfloat[12]) / (-cfloat[11]) -
			(cfloat[5] * floorx * 1000.0f + cfloat[6] * floory * 1000.0f + cfloat[8]) / (-cfloat[7]);
		
		tp = ((floorx < 0) ? (((int)(floorx * (float)cint[4]) % cint[4]) + cint[4] - 1) : ((int)(floorx * (float)cint[4]) % cint[4])) * 3
			+ ((floory < 0) ? (((int)(floory * (float)cint[5]) % cint[5]) + cint[5] - 1) : ((int)(floory * (float)cint[5]) % cint[5])) * 3 * cint[4];
		wpixels[cint[0] + j + i * cint[6]] = (int)(tpixels[tp] | tpixels[tp + 1] << 8 | tpixels[tp + 2] << 16);
	}
	if (wallBot[j] < bottom[cint[0] + j])
		bottom[cint[0] + j] = wallBot[j];
}