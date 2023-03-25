/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:37:37 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/25 16:37:55 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	here_doc(t_pipex *pipex, char **av)
{
	int		len;
	char	*line;
	char	*strs[2];

	pipex->infile = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->infile == -1)
		show_err(ERR_HEREDOC);
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (!line)
			exit (1);
		len = ft_strlen(line) - 1;
		if (len == ft_strlen(av[2]) && ft_strncmp(line, av[2], len) == 0)
			break ;
		write(pipex->infile, line, len + 1);
		free(line);
	}
	free(line);
	close(pipex->infile);
}
