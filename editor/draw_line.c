/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 13:31:53 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/01/29 13:31:55 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	ft_line(t_doom *den, t_col *vec)
{
	t_brz	tip;
	int		x2;
	int		x1;
	int		y2;
	int		y1;

	x1 = vec->x1;
	x2 = vec->x2;
	y1 = vec->y1;
	y2 = vec->y2;
	tip.color = den->color;
	tip.dx = (x2 - x1 >= 0 ? 1 : -1);
	tip.dy = (y2 - y1 >= 0 ? 1 : -1);
	tip.lengthx = abs(x2 - x1);
	tip.lengthy = abs(y2 - y1);
	tip.length = (tip.lengthx > tip.lengthy ? tip.lengthx : tip.lengthy);
	if (tip.length == 0 && x1 < den->border && y2 < HEIGHT && x1 >= 0
		&& y2 >= 0 && x2 <= den->border)
		set_pixel(den->bmp, x1, y2, tip.color);
	if (tip.lengthy <= tip.lengthx)
		ft_line_x(den, &tip, x1, y1);
	else
		ft_line_y(den, &tip, x1, y1);
}

void	ft_line_x(t_doom *den, t_brz *tip, int x1, int y1)
{
	tip->x = x1;
	tip->y = y1;
	tip->d = -tip->lengthx;
	tip->length++;
	while (tip->length--)
	{
		if (tip->x < den->border && tip->y < HEIGHT
			&& tip->x >= 0 && tip->y >= 0)
			set_pixel(den->bmp, tip->x, tip->y, tip->color);
		tip->x += tip->dx;
		tip->d += 2 * tip->lengthy;
		if (tip->d > 0)
		{
			tip->d -= 2 * tip->lengthx;
			tip->y += tip->dy;
		}
	}
}

void	ft_line_y(t_doom *den, t_brz *tip, int x1, int y1)
{
	tip->x = x1;
	tip->y = y1;
	tip->d = -tip->lengthy;
	tip->length++;
	while (tip->length--)
	{
		if (tip->x < den->border && tip->y < HEIGHT
		&& tip->x >= 0 && tip->y >= 0)
			set_pixel(den->bmp, tip->x, tip->y, tip->color);
		tip->y += tip->dy;
		tip->d += 2 * tip->lengthx;
		if (tip->d > 0)
		{
			tip->d -= 2 * tip->lengthy;
			tip->x += tip->dx;
		}
	}
}

void	pixel_width(t_doom *den, int x, int y, int colort)
{
	set_pixel(den->bmp, x, y, colort);
	set_pixel(den->bmp, x + 1, y, colort);
	set_pixel(den->bmp, x, y + 1, colort);
}

void	pixel_bigwidth(t_doom *den, int x, int y, int colort)
{
	pixel_width(den, x, y, colort);
	pixel_width(den, x - 1, y, colort);
	pixel_width(den, x + 1, y, colort);
	pixel_width(den, x, y + 1, colort);
	pixel_width(den, x, y - 1, colort);
}
