/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:30 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/30 12:49:05 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	parent_process(t_pipex pipex, char **av, char **envp)
{
	char	**tab;

	dup2(pipex.pipeline[pipex.i * 2 - 2], 0);
	dup2(pipex.outfile, 1);
	close(pipex.outfile);
	close_pipes(&pipex);
	pipex.tab = ft_pathname(av[2 + pipex.i + pipex.here_doc], &pipex, envp);
	if (!pipex.tab)
	{
		ft_free_pipex(&pipex);
		exit (1);
	}
	if (execve(pipex.tab[0], pipex.tab, envp) == -1)
	{
		ft_free_pipex(&pipex);
		exit (1);
	}
}

void	select_process(t_pipex pipex, char **av, char **envp, int ac)
{
	if (pipex.i == 0)
		child_process(pipex, av, envp);
	if (pipex.i == ac - 4 - pipex.here_doc)
		parent_process(pipex, av, envp);
	else
		pipe_process(pipex, av, envp);
}

void	ft_setfd(t_pipex *pipex, int ac, char **av)
{
	pipex->here_doc = 0;
	pipex->outfile = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		show_perr(av[ac - 1]);
	if (ft_strncmp("here_doc", av[1], 9) == 0)
	{
		pipex->here_doc = 1;
		if (ac < 6)
			show_err(ERR_INPUT);
		here_doc(pipex, av);
		pipex->infile = open(".heredoc_tmp", O_RDONLY);
		if (pipex->infile == -1)
		{
			unlink(".heredoc_tmp");
			show_perr(".heredoc_tmp");
		}
	}
	else
	{
		pipex->infile = open(av[1], O_RDONLY);
		if (pipex->infile == -1)
			show_perr(av[1]);
	}
}

void	ft_setstruct(t_pipex *pipex, int ac, char **envp)
{
	pipex->tab = 0;
	pipex->pipelen = ac - 4 - pipex->here_doc;
	pipex->pipeline = (int *)malloc(sizeof(int) * (2 * pipex->pipelen + 1));
	if (!pipex->pipeline)
		show_err(ERR_PIPE);
	pipex->pipeline[2 * pipex->pipelen] = 0;
	ft_getallpaths(pipex, envp);
	if (!pipex->paths)
	{
		close(pipex->infile);
		close(pipex->outfile);
		ft_free_pipex(pipex);
		exit (1);
	}
	pipex->i = 0;
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	if (ac < 5)
		show_err(ERR_INPUT);
	ft_setfd(&pipex, ac, av);
	ft_setstruct(&pipex, ac, envp);
	creat_pipes(&pipex);
	while (pipex.i <= pipex.pipelen)
	{
		pipex.child = fork();
		if (pipex.child == -1)
			ft_exit(&pipex, 1, ERR_FORK);
		if (!pipex.child)
			select_process(pipex, av, envp, ac);
		pipex.i++;
	}
	ft_exit(&pipex, 0, NULL);
	while (pipex.i >= 0)
	{
		waitpid(-1, NULL, 0);
		pipex.i--;
	}
	return (0);
}
