/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 17:11:46 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/25 17:45:08 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

char	**ft_getallpaths(char **envp)
{
	char	**paths;
	char	*line;

	line = 0;
	while (*envp)
	{
		if (ft_strncmp("PATH", *envp, 4) == 0)
		{
			line = *envp + 5;
			break ;
		}
		envp++;
	}
	paths = ft_split(line, ':');
	if (!paths)
		return (0);
	return (paths);
}

char	*ft_getcmdpath(char **paths, char *cmd)
{
	int		i;
	char	*cmd_path;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin("/", cmd);
		cmd_path = ft_strjoin(paths[i], temp);
		free(temp);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (0);
}

char	**ft_pathname(char *arg, t_pipex *pipex, char **envp)
{
	char	*cmd;

	pipex->tab = ft_split(arg, ' ');
	if (!pipex->tab)
		return (0);
	cmd = pipex->tab[0];
	if (!cmd)
	{
		ft_free_pipex(pipex);
		show_err(ERR_CMD);
	}
	pipex->tab[0] = ft_getcmdpath(pipex->paths, cmd);
	if (!pipex->tab[0])
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
		free(cmd);
		ft_free_pipex(pipex);
		show_err(ERR_CMD);
	}
	free(cmd);
	return (pipex->tab);
}

void	child_process(t_pipex pipex, char **av, char **envp)
{
	dup2(pipex.infile, 0);
	close(pipex.infile);
	dup2(pipex.pipeline[1], 1);
	close_pipes(&pipex);
	pipex.tab = ft_pathname(av[2 + pipex.here_doc], &pipex, envp);
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

void	pipe_process(t_pipex pipex, char **av, char **envp)
{
	dup2(pipex.pipeline[pipex.i * 2 - 2], 0);
	dup2(pipex.pipeline[pipex.i * 2 + 1], 1);
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
