/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chech_new_load.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 15:18:47 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/05 15:18:48 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	get_font_file(t_doom *den)
{
	void		*tmp;
	int			fd;
	SDL_RWops	*rw;

	den->iw.arial_font = 0;
	fd = open("fonts/ARIAL.TTF", READ_MAP);
	if (fd < 0)
		return ;
	tmp = malloc(70000);
	den->iw.font_pack_size = read(fd, tmp, 70000);
	close(fd);
	den->iw.font_pack = malloc(den->iw.font_pack_size);
	ft_memcpy(den->iw.font_pack, tmp, den->iw.font_pack_size);
	free(tmp);
	rw = SDL_RWFromConstMem(den->iw.font_pack, den->iw.font_pack_size);
	den->iw.arial_font = TTF_OpenFontRW(rw, 0, 24);
	den->font = den->iw.arial_font;
}

int		usage(void)
{
	write(1, "usage: doom-nukem new|edit|game map_name\n", 41);
	return (0);
}

int		check_new(t_doom *den, char *fname)
{
	den->fd = open(fname, O_WRONLY);
	if (den->fd >= 0)
	{
		close(den->fd);
		return (0);
	}
	den->fd = open(fname, O_DIRECTORY);
	if (den->fd >= 0)
	{
		close(den->fd);
		return (0);
	}
	den->fd = open(fname, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);
	if (den->fd < 0)
		return (0);
	close(den->fd);
	den->fname = ft_strdup(fname);
	return (1);
}

int		check_load(t_doom *den, char *fname)
{
	den->fd = open(fname, O_DIRECTORY);
	if (den->fd >= 0)
	{
		close(den->fd);
		return (0);
	}
	den->fd = open(fname, O_RDWR);
	if (den->fd < 0)
		return (0);
	close(den->fd);
	den->fname = ft_strdup(fname);
	return (1);
}
