/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 11:36:17 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/04 11:36:18 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	movie_button_five(t_doom *den, t_col *vec)
{
	if (den->button_change == 9 && (den->change == 1 || den->change == 2))
	{
		all_sector(den, den->tmp);
		den->button_change = 0;
		den->color = 0xFFFFFF;
		den->change = 3;
	}
	else if (den->button_change == 10)
	{
		sprite(den);
		den->change = 0;
		den->button_change = 0;
	}
	else if (den->button_change == 11 && den->change == 3)
	{
		den->finish = den->find_tmp->sec;
		den->change = 0;
		den->button_change = 0;
		retry_write(den, vec);
	}
}

void	movie_button_four(t_doom *den, t_col *vec)
{
	if (den->button_change == 7)
	{
		if (den->blouk != 1 && den->walls > -1 && ((vec->x1 != -1000000 &&
		vec->y1 != -1000000) &&
		(vec->x2 != -1000000 && vec->y2 != -1000000)))
			back_wall(den, vec);
		else if (vec->x1 == -1000000 && vec->y1 == -1000000 && vec->x2
			!= -1000000 && vec->y2 != -1000000)
			back_startwall(den, vec);
		den->button_change = 0;
	}
	else if (den->button_change == 8)
	{
		player(den);
		den->button_change = 0;
		den->change = 0;
	}
}

void	movie_button_three(t_doom *den)
{
	if (den->button_change == 4)
	{
		if (den->sprite)
			check_sprite(den);
		den->xy.x = den->player.x;
		den->xy.y = den->player.y;
		in_all_sect(den);
		if (den->xy.bool_cor == -1)
		{
			den->player.x = -10000;
			den->player.y = -10000;
		}
		if (den->player.x == -10000 && den->player.y == -10000)
			blouk_player(den);
		else
			main3d_edit(den);
		den->button_change = 0;
	}
	else if (den->button_change == 5 && (den->change == 1 || den->change == 2))
	{
		if (den->find_tmp_one && den->find_tmp_two)
			drow_find(den);
		den->button_change = 0;
	}
}

void	movie_button_two(t_doom *den, t_col *vec)
{
	if (den->button_change == 3 && ((den->change == 1 || den->change == 2)
		|| den->change == 3))
	{
		if (den->find_tmp_one != 0 && den->find_tmp_two != 0 &&
		den->find_tmp->inside == 0 && den->find_tmp_one->inside == 0
		&& den->find_tmp_two->inside == 0)
		{
			portal_sector(den);
			clear_find(den);
			den->change = 0;
			den->button_change = 0;
		}
		else if (den->change == 3 && den->find_tmp->inside != 0)
		{
			make_portal_sector(den, vec);
			den->change = 0;
			den->button_change = 0;
		}
	}
	else if (den->button_change == 6)
	{
		save_map_file(den);
		den->button_change = 0;
	}
}

void	movie_button(t_doom *den, t_col *vec)
{
	if (den->button_change == 1 && (den->change != 0))
	{
		if (check_picture(den) == 0 || den->change == 4)
		{
			delete_wall(den, vec);
			den->button_change = 0;
		}
		else
			blouk_delete(den);
	}
	else if (den->button_change == 2 && (den->change == 1 || den->change == 2))
	{
		next_wall(den);
		den->button_change = 0;
		clean_display(den);
		info_display(den);
		SDL_UpdateWindowSurface(den->window);
	}
	else
	{
		movie_button_two(den, vec);
		movie_button_three(den);
		movie_button_four(den, vec);
		movie_button_five(den, vec);
	}
}
