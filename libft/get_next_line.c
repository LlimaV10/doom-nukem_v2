/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 12:56:19 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/06/17 14:33:42 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>

static char		*add_prev(t_line *lst, const int fd, char **line)
{
	char	*tmp1;
	char	*tmp2;

	while (lst != 0)
	{
		if (lst->fd == fd)
			break ;
		lst = lst->next;
	}
	if ((lst == 0) || (lst->content == 0))
		return (0);
	tmp1 = ft_strchr(lst->content, '\n');
	if (tmp1 != 0)
		*tmp1 = '\0';
	tmp2 = *line;
	*line = ft_strjoin(*line, lst->content);
	free(tmp2);
	tmp2 = lst->content;
	if (tmp1 != 0)
		lst->content = ft_strdup(tmp1 + 1);
	else
		lst->content = 0;
	free(tmp2);
	return (tmp1);
}

static t_line	*ft_new_lst_add(t_line **lst, const int fd)
{
	t_line	*new;
	t_line	*tmp;

	new = (t_line *)malloc(sizeof(t_line));
	new->fd = fd;
	new->next = 0;
	if (*lst == 0)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next != 0)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (new);
}

static void		save_end(t_line **lst, const int fd, char *end)
{
	t_line	*ltmp;

	ltmp = *lst;
	while (ltmp != 0)
	{
		if (ltmp->fd == fd)
			break ;
		ltmp = ltmp->next;
	}
	if (ltmp == 0)
		ltmp = ft_new_lst_add(lst, fd);
	ltmp->content = (char *)malloc(BUFF_SIZE);
	ft_strcpy(ltmp->content, end);
}

static char		read_next(t_line **lst, const int fd, char **line)
{
	char	buf[BUFF_SIZE + 1];
	char	*tmp1;
	char	*tmp2;
	int		rd;

	rd = read(fd, buf, BUFF_SIZE);
	while (rd > 0)
	{
		buf[rd] = '\0';
		tmp1 = ft_strchr(buf, '\n');
		if (tmp1 != 0)
			*tmp1 = '\0';
		tmp2 = *line;
		*line = ft_strjoin(*line, buf);
		free(tmp2);
		if (tmp1 != 0)
		{
			save_end(lst, fd, tmp1 + 1);
			return (1);
		}
		rd = read(fd, buf, BUFF_SIZE);
	}
	if ((*line != 0) && (**line != '\0'))
		return (1);
	return (0);
}

int				get_next_line(const int fd, char **line)
{
	static t_line	*lst = 0;
	char			c;

	if ((line == 0) || (fd < 0) || (read(fd, &c, 0) < 0))
		return (-1);
	*line = 0;
	if (add_prev(lst, fd, line) != 0)
		return (1);
	return (read_next(&lst, fd, line));
}
