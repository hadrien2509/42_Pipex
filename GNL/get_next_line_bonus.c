/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:08:22 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/22 18:11:34 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_finish_line(int fd, char *buffer, char *line, int length)
{	
	int		read_status;
	int		buffer_i;

	read_status = read(fd, buffer, BUFFER_SIZE);
	ft_readstatus(buffer, read_status);
	while (read_status)
	{
		buffer_i = 0;
		while (buffer[buffer_i])
		{
			length++;
			if (buffer[buffer_i] == '\n')
			{
				line = ft_stradd(line, buffer, length, 0);
				ft_setzero(buffer);
				return (line);
			}
			buffer_i++;
		}
		if (length > 0)
			line = ft_stradd(line, buffer, length, 0);
		read_status = read(fd, buffer, BUFFER_SIZE);
		ft_readstatus(buffer, read_status);
	}
	return (line);
}
#include <stdio.h>
char	*get_next_line(int fd)
{
	static char		buffer_array[OPEN_MAX][BUFFER_SIZE + 1];
	char			*line;
	int				length;
	int				buffer_start;

	buffer_start = 0;
	length = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		ft_readstatus(buffer_array[fd], read(fd, 0, 0));
		return (NULL);
	}
	line = NULL;
	line = ft_old_buffer(buffer_array[fd], line, &length, &buffer_start);
	if (line)
		return (line);
	if (length > 0)
	{
		line = ft_stradd(line, buffer_array[fd], length, buffer_start);
		ft_setzero(buffer_array[fd]);
	}
	line = ft_finish_line(fd, buffer_array[fd], line, length);
	return (line);
}
