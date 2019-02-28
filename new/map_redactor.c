/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_redactor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 15:26:50 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/01/29 15:26:52 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	clean_vec(t_col *vec)
{
	vec->x1 = LIM;
	vec->y1 = LIM;
	vec->x2 = LIM;
	vec->y2 = LIM;
}

void	map_redactor_mane(t_doom *den, t_col *vec)
{
	den->skape = 0;
	den->retskape = den->skape;
	den->startx = 0;
	den->starty = 0;
	
	den->blouk = 0;
	den->change = 0;
	
	
	den->button_change = 0;
	den->find_tmp_one = 0;
	den->select = 0;
	den->blockwall = 0;
	
	den->incede = 0;
	den->angle = 0;
	den->noblouk = 0;
	
	den->sort = 0;
	den->find_tmp = 0;
	den->find_tmp_one = 0;
	den->find_tmp_two = 0;
	den->find_sprite = 0;

	clean_vec(vec);
	clean_find_vec(den);
	info_display(den);
}

void	map_redactor_mane2(t_doom *den)
{
	den->sec = 0;
	den->secbak = -1; // den->sec - 1
	den->walls = -1;
	den->sprites = 0;
	den->player.x = LIM;
	den->player.y = LIM;
	den->player.introt = 1;
	den->finish = -1;
	den->sprite = 0;
	den->tmp = 0;
	den->sectors = 0;
	den->secbaktmp = 0; // �� ������
}

void	clean_find_vec(t_doom *den)
{
	den->change = 0;
	den->rec.x1 = 0;
	den->rec.y1 = 0;
	den->rec.x2 = 0;
	den->rec.y2 = 0;
	den->rec.sect = 0;
	den->rec_two.x1 = 0;
	den->rec_two.y1 = 0;
	den->rec_two.x2 = 0;
	den->rec_two.y2 = 0;
	den->rec_two.sect = 0;
}

void	map_network(t_doom *den)
{
	int		y;
	int		x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < (WIDTH - 220))
		{
			pixel_width(den, x, y, 0xB3B3B3);
			x += (START + den->skape);
		}
		y += (START + den->skape);
	}
	den->border = x - (START + den->skape);
}

void	findstart(t_doom *den)
{
	den->startx = den->startx / (den->retskape + START) * (den->skape + START);
	den->starty = den->starty / (den->retskape + START) * (den->skape + START);
	den->retskape = den->skape;
}
