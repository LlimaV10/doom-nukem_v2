/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkostrub <kkostrub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:48:06 by kkostrub          #+#    #+#             */
/*   Updated: 2019/03/05 17:32:25 by kkostrub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define ONE (WINDOW_W * 2 + 2)
#include "doom_nuken.h"

int		load_map_sprites(t_doom *den)
{
	t_sprite			*tmp;
	int					i;

	den->sprite = 0;
	if (!read_check(den->fd, &den->sprites, sizeof(int)))
		return (0);
	i = -1;
	while (++i < den->sprites)
	{
		tmp = (t_sprite *)malloc(sizeof(t_sprite));
		if (!read_check(den->fd, tmp, sizeof(t_sprite) - sizeof(int) * ONE))
		{
			free(tmp);
			return (0);
		}
		tmp->fall_time = 1;
		tmp->e.previous_picture_change = 1;
		tmp->e.prev_update_time = 1;
		tmp->next = den->sprite;
		den->sprite = tmp;
	}
	return (1);
}

int		load_map_walls_pictures_1(t_doom *den, t_wals *tmp)
{
	t_picture			*pic;
	int					count_pic;
	int					j;

	if (!read_check(den->fd, &count_pic, sizeof(int)))
		return (0);
	j = -1;
	while (++j < count_pic)
	{
		pic = (t_picture *)malloc(sizeof(t_picture));
		if (!read_check(den->fd, pic, sizeof(t_picture)))
		{
			free(pic);
			return (0);
		}
		pic->next = tmp->p;
		tmp->p = pic;
	}
	return (1);
}

int		load_map_walls_pictures(t_doom *den)
{
	t_wals				*tmp;
	int					i;

	den->tmp = 0;
	if (!read_check(den->fd, &den->walls, sizeof(int)))
		return (0);
	i = -1;
	while (++i < den->walls)
	{
		tmp = (t_wals *)malloc(sizeof(t_wals));
		if (!read_check(den->fd, tmp, sizeof(t_wals)))
		{
			free(tmp);
			return (0);
		}
		tmp->p = 0;
		if (!load_map_walls_pictures_1(den, tmp))
			return (0);
		tmp->next = den->tmp;
		den->tmp = tmp;
	}
	den->walls -= 1;
	return (1);
}

int		load_map_1(t_doom *den, int zero)
{
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_sector_animations(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_wall_animations(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_checkpoints(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_level(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	return (1);
}

int		load_map(t_doom *den)
{
	int					zero;
	char				author[79];

	zero = 0;
	author[78] = '\0';
	if (!read_check(den->fd, author, 78))
		return (0);
	if (ft_strcmp(author, "This is map created for doom-nukem by"
		" dbolilyi & ddehtyar & kkostrub & elopukh") != 0)
		return (0);
	if (!load_map_walls_pictures(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_sec(den))
		return (0);
	if (!connect_map_sectors(den))
		return (0);
	if (!read_check(den->fd, &zero, sizeof(int)) || zero != 0)
		return (0);
	if (!load_map_sprites(den))
		return (0);
	if (!load_map_1(den, zero))
		return (0);
	return (1);
}
