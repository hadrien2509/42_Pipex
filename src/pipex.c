/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:30 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/13 20:19:20 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

	dup2(pipex.end[0], 0);
	close(pipex.end[1]);
	dup2(pipex.outfile, 1);
	close(pipex.outfile);
	ft_pathname(av[3], &pipex, envp);
	if (!pipex.tab)
	{
		ft_free_pipex(&pipex);
		show_err(ERR_CMD);
		exit(1);
	}
	execve(pipex.tab[0], pipex.tab, envp);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	if (ac != 5)
		return (show_err(ERR_INPUT));
	pipex.infile = open(av[1], O_RDONLY);
	if (pipex.infile == -1)
		return (show_err(ERR_INFILE));
	pipex.outfile = open(av[4], O_WRONLY);
	if (pipex.outfile == -1)
		return (show_err(ERR_OUTFILE));
	if (pipe(pipex.end) == -1)
		return (show_err(ERR_PIPE));
	pipex.child = fork();
	if (pipex.child == 0)
		child_process(pipex, av, envp);
	else
	{
		wait(&pipex.child);
		printf("child process over\n");
		parent_process(pipex, av, envp);
	}
	return (0);
}
