/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:30 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/21 13:31:29 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	show_err(char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}

void	ft_free_pipex(t_pipex *pipex)
{
	int		i;

	if (pipex->tab)
	{
		i = 0;
		while (pipex->tab[i])
		{
			free(pipex->tab[i]);
			i++;
		}
		free(pipex->tab);
	}
	if (pipex->paths)
	{
		i = 0;
		while (pipex->paths[i])
		{
			free(pipex->paths[i]);
			i++;
		}
		free(pipex->paths);
	}
}

void	parent_process(t_pipex pipex, char **av, char **envp)
{
	char	**tab;

	dup2(pipex.pipeline[pipex.i * 2 - 2], 0);
	close(pipex.pipeline[pipex.i * 2 - 1]);
	dup2(pipex.outfile, 1);
	close(pipex.outfile);
	ft_pathname(av[2 + pipex.i], &pipex, envp);
	if (!pipex.tab)
	{
		ft_free_pipex(&pipex);
		show_err(ERR_CMD);
		exit(1);
	}
	execve(pipex.tab[0], pipex.tab, envp);
}

void	select_process(t_pipex pipex, char **av, char **envp, int ac)
{
	if (pipex.i == 0)
		child_process(pipex, av, envp);
	if (pipex.i == ac - 4)
		parent_process(pipex, av, envp);
	else
		pipe_process(pipex, av, envp);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	if (ac < 5)
		return (show_err(ERR_INPUT));
	pipex.infile = open(av[1], O_RDONLY);
	if (pipex.infile == -1)
		return (show_err(ERR_INFILE));
	pipex.outfile = open(av[ac - 1], O_CREAT | O_WRONLY, 0644);
	if (pipex.outfile == -1)
		return (show_err(ERR_OUTFILE));
	pipex.pipeline = (int *)malloc(sizeof(int) * (2 * (ac - 3)));
	if (!pipex.pipeline)
		show_err(ERR_PIPE);
	pipex.i = 0;
	while (pipex.i <= ac - 4)
	{
		if (pipe(pipex.pipeline + pipex.i * 2) == -1)
			return (show_err(ERR_PIPE));
		pipex.child = fork();
		if (!pipex.child)
			select_process(pipex, av, envp, ac);
		wait(&pipex.child);
		pipex.i++;
	}
	//close_pipes(&pipex);
	return (0);
}
