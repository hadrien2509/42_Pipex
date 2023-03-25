/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:30:21 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/25 16:31:08 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_free_pipex(t_pipex *pipex)
{
	close(pipex->infile);
	close(pipex->outfile);
	if (pipex->tab)
		ft_free_tab(pipex->tab);
	if (pipex->paths)
		ft_free_tab(pipex->paths);
}

void	ft_exit(t_pipex *pipex, int e, char *str)
{
	ft_free_pipex(pipex);
	if (e == 1)
		show_err(str);
	if (e == 2)
		show_perr(str);
	else
		exit (1);
}
