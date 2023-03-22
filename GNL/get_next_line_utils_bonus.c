/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 15:06:15 by hgeissle          #+#    #+#             */
/*   Updated: 2022/11/15 11:24:26 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_alloc_line(char *line, int length)
{
	char	*updated_line;

	updated_line = malloc(sizeof(char) * (length + 1));
	if (!updated_line)
	{
		if (line)
			free(line);
		return (0);
	}
	updated_line[length] = '\0';
	return (updated_line);
}

char	*ft_stradd(char *line, char *buffer, int length, int buffer_start)
{
	char	*updated_line;
	int		line_i;
	int		i;

	i = 0;
	line_i = 0;
	updated_line = ft_alloc_line(line, length);
	if (!updated_line)
		return (0);
	if (line)
	{
		while (line[line_i])
		{
			updated_line[line_i] = line[line_i];
			line_i++;
		}
		free(line);
	}
	while (buffer[buffer_start + i] != '\n' && buffer[buffer_start + i])
	{
		updated_line[line_i + i] = buffer[buffer_start + i];
		i++;
	}
	updated_line [line_i + i] = buffer[buffer_start + i];
	return (updated_line);
}

char	*ft_setzero(char *buffer)
{
	size_t	i;

	i = 0;
	while (!buffer[i] && i < BUFFER_SIZE)
		i++;
	while (buffer[i] != '\n' && i < BUFFER_SIZE)
	{
		buffer[i] = 0;
		i++;
	}
	buffer[i] = 0;
	return (buffer);
}

char	*ft_readstatus(char *buffer, int read_status)
{
	int	i;

	i = 0;
	if (read_status == -1)
		read_status = 0;
	while (i < BUFFER_SIZE - read_status)
	{
		buffer[BUFFER_SIZE - 1 - i] = 0;
		i++;
	}
	return (buffer);
}

char	*ft_old_buffer(char *buffer, char *line, int *length, int *start)
{
	int		buffer_i;

	while (*start < BUFFER_SIZE && !buffer[*start])
		(*start)++;
	buffer_i = *start;
	while (buffer[buffer_i])
	{
		(*length)++;
		if (buffer[buffer_i] == '\n')
		{
			line = ft_stradd(line, buffer, *length, *start);
			ft_setzero(buffer);
			return (line);
		}
		buffer_i++;
	}
	return (0);
}
