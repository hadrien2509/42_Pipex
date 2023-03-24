/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:30 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/24 18:52:00 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	show_perr(char *err)
{
	perror(err);
	exit (0);
}

void	show_err(char *err)
{
	write(2, err, ft_strlen(err));
	if (ft_strncmp(err, ERR_CMD, ft_strlen(ERR_CMD)) == 0)
		exit (127);
	exit (1);
}

void	ft_free_pipex(t_pipex *pipex)
{
	if (pipex->tab)
		ft_free_tab(pipex->tab);
	if (pipex->paths)
		ft_free_tab(pipex->paths);
}

void	parent_process(t_pipex pipex, char **av, char **envp)
{
	char	**tab;

	dup2(pipex.end[0], 0);
	dup2(pipex.outfile, 1);
	close(pipex.outfile);
	close(pipex.end[0]);
	close(pipex.end[1]);
	ft_pathname(av[3], &pipex, envp);
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

void	ft_setfd(t_pipex *pipex, int ac, char **av)
{
	if (ac != 5)
		show_err(ERR_INPUT);
	pipex->outfile = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		show_perr(av[4]);
	pipex->infile = open(av[1], O_RDONLY);
	if (pipex->infile == -1)
		show_perr(av[1]);
}

static void	ft_exit(t_pipex *pipex, int e, char *str)
{
	ft_free_pipex(pipex);
	if (e == 1)
		show_err(str);
	if (e == 2)
		show_perr(str);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	ft_setfd(&pipex, ac, av);
	pipex.paths = ft_getallpaths(envp);
	if (!pipex.paths)
		return (1);
	if (pipe(pipex.end) == -1)
		ft_exit(&pipex, 2, "Pipe");
	pipex.child = fork();
	if (pipex.child == -1)
	{
		close(pipex.end[0]);
		close(pipex.end[1]);
		ft_exit(&pipex, 1, ERR_FORK);
	}
	if (pipex.child == 0)
		child_process(pipex, av, envp);
	wait(&pipex.child);
	parent_process(pipex, av, envp);
	return (0);
}
