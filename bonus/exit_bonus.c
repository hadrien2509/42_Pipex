/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:52:09 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/25 17:52:12 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < 2 * pipex->pipelen)
	{
		close(pipex->pipeline[i]);
		i++;
	}
}

void	ft_free_pipex(t_pipex *pipex)
{
	close(pipex->infile);
	close(pipex->outfile);
	if (pipex->tab)
		ft_free_tab(pipex->tab);
	if (pipex->paths)
		ft_free_tab(pipex->paths);
	if (pipex->pipeline)
		free(pipex->pipeline);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
}

void	ft_exit(t_pipex *pipex, int e, char *str)
{
	close_pipes(pipex);
	ft_free_pipex(pipex);
	if (e == 1)
		show_err(str);
	if (e == 2)
		show_perr(str);
	else
		exit(1);
}
