



__kernel void get_wall_tx_zu_zd(__global const float *ang, __global const float *dang,
	__global const int *top, __global const int *bottom, __global const int *twidth,
	__global const int *left_x, __global const float *lenpl, __global const float *sing,
	__global const float *left_olen, __global const int *left_zu,
	__global const int *left_zd, __global const float *tsz,
	__global const float *zudiff, __global const float *zddiff,
	__global int *txs, __global float *zus, __global float *zds)
{
	int j = get_global_id(0);

	if (top[*left_x + j] >= bottom[*left_x + j])
		return;
	float nang = *ang + *dang * (float)j;
	float left_len = sin(nang) * *lenpl / sin(*sing - nang);
	txs[j] = (*left_olen + left_len) * (float)*twidth * *tsz / 1000.0f;
	while (txs[j] > (float)*twidth)
		txs[j] -= (float)*twidth;
	zus[j] = (float)*left_zu + left_len * *zudiff;
	zds[j] = (float)*left_zd + left_len * *zddiff;
}

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

__kernel void get_wall_tx_zu_zd2(
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