/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 17:11:46 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/13 20:17:45 by hgeissle         ###   ########.fr       */
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
		if (access(cmd_path, F_OK) == 0)
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
	pipex->paths = ft_getallpaths(envp);
	if (!pipex->paths)
		return (0);
	pipex->tab[0] = ft_getcmdpath(pipex->paths, cmd);
	free(cmd);
	if (!pipex->tab[0])
		return (0);
	return (pipex->tab);
}

void	child_process(t_pipex pipex, char **av, char **envp)
{
	dup2(pipex.infile, 0);
	close(pipex.infile);
	dup2(pipex.end[1], 1);
	close(pipex.end[0]);
	pipex.tab = ft_pathname(av[2], &pipex, envp);
	if (!pipex.tab)
	{
		ft_free_pipex(&pipex);
		show_err(ERR_CMD);
		exit(1);
	}
	execve(pipex.tab[0], pipex.tab, envp);
}
