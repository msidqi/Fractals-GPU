/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 21:22:45 by msidqi            #+#    #+#             */
/*   Updated: 2018/10/22 02:49:17 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_nlstr(char *holder, int sr)
{
	int i;

	i = 0;
	while (i < sr && holder[i])
	{
		if (holder[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static int		ft_filler(int fd, char **holder, char **line, int i)
{
	int sr;

	holder[fd] = ft_strnew(BUFF_SIZE + 1);
	while ((sr = read(fd, holder[fd], BUFF_SIZE)) > 0)
	{
		holder[fd][sr] = '\0';
		if ((i = ft_nlstr(holder[fd], sr)) >= 0)
		{
			*line = ft_strjoin(*line, ft_strsub(holder[fd], 0, i));
			holder[fd] = ft_strdup(holder[fd] + i + 1);
			return (1);
		}
		*line = ft_strjoin(*line, holder[fd]);
		free(holder[fd]);
		holder[fd] = ft_strnew(BUFF_SIZE + 1);
	}
	if (sr == -1)
		return (-1);
	if (sr == 0 && *line[0] == '\0')
		return (0);
	if (sr == 0 && *line[0] != '\0')
		return (1);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static char		*holder[4864];
	int				rv;
	int				i;

	if (fd < 0 || !line || fd > 4864 || BUFF_SIZE <= 0)
		return (-1);
	*line = ft_strdup("");
	if (holder[fd] == NULL)
	{
		if ((rv = ft_filler(fd, holder, line, 0)) == 1)
			return (1);
	}
	if ((i = ft_nlstr(holder[fd], BUFF_SIZE)) >= 0)
	{
		*line = ft_strjoin(*line, ft_strsub(holder[fd], 0, i));
		holder[fd] = ft_strdup(holder[fd] + i + 1);
		return (1);
	}
	*line = ft_strjoin(*line, holder[fd]);
	if ((rv = ft_filler(fd, holder, line, 0)) == 1)
		return (1);
	return (rv == -1 ? -1 : 0);
}
