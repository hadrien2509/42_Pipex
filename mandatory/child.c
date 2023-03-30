/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 17:11:46 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/30 14:14:18 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

char	*ft_getcmdpath(char **paths, t_pipex *pipex)
{
	int		i;
	char	*cmd_path;
	char	*temp;

	i = 0;
	if (pipex->cmd[0] == '/')
	{
		if (access(pipex->cmd, X_OK) == 0)
			return (cmd_path);
		else
			return (0);
	}
	while (paths[i])
	{
		temp = ft_strjoin("/", pipex->cmd);
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
	pipex->tab = ft_split(arg, ' ');
	if (!pipex->tab)
		return (0);
	pipex->cmd = pipex->tab[0];
	if (!pipex->cmd)
	{
		ft_free_pipex(pipex);
		show_err(ERR_CMD);
	}
	pipex->path = ft_getcmdpath(pipex->paths, pipex);
	if (!pipex->path)
	{
		write(2, pipex->cmd, ft_strlen(pipex->cmd));
		write(2, ": ", 2);
		ft_free_pipex(pipex);
		show_err(ERR_CMD);
	}
	return (pipex->tab);
}

void	child_process(t_pipex pipex, char **av, char **envp)
{
	dup2(pipex.infile, 0);
	close(pipex.infile);
	dup2(pipex.end[1], 1);
	close(pipex.end[0]);
	close(pipex.end[1]);
	pipex.tab = ft_pathname(av[2], &pipex, envp);
	if (!pipex.tab)
	{
		ft_free_pipex(&pipex);
		exit (1);
	}
	if (execve(pipex.path, pipex.tab, envp) == -1)
	{
		ft_free_pipex(&pipex);
		exit (1);
	}
}
