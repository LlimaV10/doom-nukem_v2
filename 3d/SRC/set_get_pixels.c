#include "../guardians.h"

void	set_pixel2(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
	{
		Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
		*(Uint32 *)target_pixel = pixel;
	}
}

int		get_light_color(int color, t_picture *light)
{
	if (light == 0 || light->t != 18)
		return (color);
	return ((((int)((float)(color >> 16) * 0.2f)) << 16) +
		(((int)((float)((color >> 8) - (color >> 16 << 8))
		* 0.2f)) << 8) + (int)((float)(color - (color
		>> 8 << 8)) * 0.2f));
}

Uint32	get_pixel(t_packaging_texture *sur, const int x, const int y)
{
	uint8_t *v;
	int		bpp;

	if (x < 0 || x >= sur->w || y < 0 || y >= sur->h)
		return (0);
	bpp = sur->bpp;
	v = (uint8_t *)sur->pixels + y * sur->pitch + x * bpp;
	return (v[0] | v[1] << 8 | v[2] << 16);
}

Uint32	get_pixel_surface(SDL_Surface *sur, const int x, const int y)
{
	uint8_t *v;
	int		bpp;

	if (x < 0 || x >= sur->w || y < 0 || y >= sur->h)
		return (0);
	bpp = sur->format->BytesPerPixel;
	v = (uint8_t *)sur->pixels + y * sur->pitch + x * bpp;
	return (v[0] | v[1] << 8 | v[2] << 16);
}