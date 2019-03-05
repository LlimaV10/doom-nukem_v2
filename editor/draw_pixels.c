/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 14:20:46 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/01/29 14:20:48 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

Uint32	get_pixel2(SDL_Surface *sur, const int x, const int y)
{
	uint8_t *v;
	int		bpp;

	if (x < 0 || x >= sur->w || y < 0 || y >= sur->h)
		return (0);
	bpp = sur->format->BytesPerPixel;
	v = (uint8_t *)sur->pixels + y * sur->pitch + x * bpp;
	return (v[0] | v[1] << 8 | v[2] << 16);
}

void	coordinate_network(int x, int y, t_doom *den, t_col *vec)
{
	int		all_skape;
	float	findx;
	float	findy;

	all_skape = START + den->skape;
	findx = ((float)x / all_skape + 0.5);
	vec->xmod = (int)findx * all_skape;
	findy = ((float)y / all_skape + 0.5);
	vec->ymod = (int)findy * all_skape;
}

void	set_pixel(SDL_Surface *bmp, int x, int y, Uint32 pixel)
{
	Uint32 *target_pixel;

	target_pixel = NULL;
	if (x < WIDTH && x >= 0 && y < HEIGHT && y >= 0)
	{
		target_pixel = bmp->pixels + y * bmp->pitch + x
			* sizeof(*target_pixel);
		*target_pixel = pixel;
	}
}

void	load_image(t_doom *den)
{
	den->icon[0] = SDL_LoadBMP("icon/delete.bmp");
	den->icon[1] = SDL_LoadBMP("icon/next_wall.bmp");
	den->icon[2] = SDL_LoadBMP("icon/all_walls.bmp");
	den->icon[3] = SDL_LoadBMP("icon/start.bmp");
	den->icon[4] = SDL_LoadBMP("icon/5.bmp");
	den->icon[5] = SDL_LoadBMP("icon/save.bmp");
	den->icon[6] = SDL_LoadBMP("icon/bakc.bmp");
	den->icon[7] = SDL_LoadBMP("icon/player.bmp");
	den->icon[8] = SDL_LoadBMP("icon/all_sector.bmp");
	den->icon[9] = SDL_LoadBMP("icon/way.bmp");
	den->icon[10] = SDL_LoadBMP("icon/sprite.bmp");
	den->icon[11] = SDL_LoadBMP("icon/finish.bmp");
	den->icon[12] = SDL_LoadBMP("icon/sprite_find.bmp");
	den->i = -1;
	while (++den->i < 13)
	{
		if (den->icon[den->i] == 0)
		{
			write(1, "Error, check your files which need to be exist\n", 47);
			exit(0);
		}
	}
}

void	free_surface(t_doom *den)
{
	int i;

	i = -1;
	while (++i < 12)
		SDL_FreeSurface(den->icon[i]);
}
