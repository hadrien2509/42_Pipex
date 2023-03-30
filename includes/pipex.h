/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:03:48 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/30 13:22:47 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	char	*path;
	char	**tab;
	char	**paths;
	int		child;
	int		pid;
	int		end[2];
	char	*cmd;
}					t_pipex;

/* ************************************************************************** */
/*                                ERROR MESSAGES                              */
/* ************************************************************************** */

# define ERR_INFILE "Infile error\n"
# define ERR_OUTFILE "Outfile error\n"
# define ERR_INPUT "Pipex executable must take 4 arguments\n"
# define ERR_PIPE "Pipe error\n"
# define ERR_FORK "Fork error\n"
# define ERR_CMD "command not found\n"

/* ************************************************************************** */
/*                                PIPEX_FCTS                                  */
/* ************************************************************************** */

void	show_err(char *err);
void	show_perr(char *err);
void	child_process(t_pipex pipex, char **av, char **envp);
char	**ft_pathname(char *arg, t_pipex *pipex, char **envp);
void	ft_free_pipex(t_pipex *pipex);
char	**ft_getallpaths(char **envp);
void	ft_exit(t_pipex *pipex, int e, char *str);

/* ************************************************************************** */
/*                                LIBFT_FCTS                                  */
/* ************************************************************************** */

int		ft_strlen(char *str);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_free_tab(char **tab);

#endif