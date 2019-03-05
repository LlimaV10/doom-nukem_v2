/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/26 15:54:12 by ddehtyar          #+#    #+#             */
/*   Updated: 2018/08/26 15:54:14 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"
#include <stdio.h>

void	go_delete_one(t_wals *rmp, t_wals *nmp)
{
	nmp = rmp->nextsector_wall;
	nmp->next_sector = 0;
	nmp->nextsector_wall = 0;
	rmp->next_sector = 0;
	rmp->nextsector_wall = 0;
}

int		main_help(t_doom *den, char **argv)
{
	if (!check_load(den, argv[2]))
	{
		write(1, "Error loading file\n", 19);
		return (0);
	}
	den->fd = open(den->fname, READ_MAP);
	if (!load_map(den))
	{
		write(1, "Error loading file\n", 20);
		close(den->fd);
		return (0);
	}
	close(den->fd);
	main_edit(den);
	return (1);
}

int		main_help_two(t_doom *den, char **argv)
{
	if (!check_load(den, argv[2]))
	{
		write(1, "Error loading file\n", 19);
		return (0);
	}
	den->fd = open(den->fname, READ_MAP);
	if (!load_map(den) || !load4_game(den))
	{
		write(1, "Error loading file\n", 20);
		close(den->fd);
		return (0);
	}
	close(den->fd);
	main_game(den);
	return (1);
}

int		main_help_one(t_doom *den, char **argv)
{
	if (!check_new(den, argv[2]))
	{
		write(1, "Error creating file\n", 20);
		return (0);
	}
	map_redactor_mane2(den);
	main_new(den);
	return (1);
}

int		main(int argc, char **argv)
{
	t_doom	den;

	if (argc != 3)
		return (usage());
	if (ft_strcmp(argv[1], "new") == 0)
	{
		if (main_help_one(&den, argv) == 0)
			return (0);
	}
	else if (ft_strcmp(argv[1], "edit") == 0)
	{
		if (main_help(&den, argv) == 0)
			return (0);
	}
	else if (ft_strcmp(argv[1], "game") == 0)
	{
		if (main_help_two(&den, argv) == 0)
			return (0);
	}
	else
		return (usage());
	return (0);
}
