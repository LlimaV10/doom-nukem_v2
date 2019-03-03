#include "../guardians.h"

void	draw_sprite(t_sdl *iw, t_sprite *sprite)
{
	int		i;
	int		j;
	int		stripe;
	int		y;
	float	koef;
	int		texX;
	int		texY;
	int		colour;

	sprite->spriteheight = 2 * sprite->spritewidth * sprite->t->h / sprite->t->w;
	if (abs(sprite->z - iw->p.z) > 1500)
		sprite->spriteheight = (float)sprite->spriteheight * 1.0f /
		(float)(abs(sprite->z - iw->p.z) / 1500.0f);
	sprite->ey = WINDOW_H * (iw->p.z + (int)sprite->pplen / 2 - sprite->z) / (int)sprite->pplen + iw->p.rotup;
	sprite->sy = sprite->ey - sprite->spriteheight;
	i = 0;
	stripe = sprite->sx - 1;
	if (stripe < -1)
	{
		i -= stripe + 1;
		stripe = -1;
	}
	while (++stripe < sprite->ex && stripe < WINDOW_W)
	{
		j = 0;
		koef = (float)sprite->spritewidth * 2.0f / (float)sprite->t->w;
		texX = (int)fabsf((float)i / koef);
		if ((sprite->top[stripe] < sprite->bottom[stripe])
			&& sprite->top[stripe] != -1)
		{
			y = sprite->sy - 1;
			if (y < -1)
			{
				j -= y + 1;
				y = -1;
			}
			while (++y < sprite->ey && y < WINDOW_H)
			{
				if (sprite->sy < WINDOW_H && sprite->bottom[stripe] >y && sprite->top[stripe] < y)
				{
					koef = (float)sprite->spriteheight / sprite->t->h;
					texY = (int)(j / koef);
					colour = get_pixel(sprite->t, texX, texY);
					if (colour != 0x010000)
						set_pixel2(iw->sur, stripe, y,
							get_light_color(colour, iw->sectors[sprite->num_sec].light));
				}
				j++;
			}
		}
		i++;
	}
}

void	draw_sprites(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (iw->sectors[tmp1->num_sec].visited && tmp1->draweble && tmp1->taken == 0)
		{
			if (tmp1->top[WINDOW_W / 2] != -1 && tmp1->sx < WINDOW_W / 2 && tmp1->ex > WINDOW_W / 2 &&
				tmp1->top[WINDOW_W / 2] < WINDOW_H / 2 && tmp1->bottom[WINDOW_W / 2] > WINDOW_H / 2
				&& tmp1->sy < WINDOW_H / 2 && tmp1->ey > WINDOW_H / 2)
				iw->v.look_sprite = tmp1;
			draw_sprite(iw, tmp1);
		}
		tmp1 = tmp1->next;
	}
}