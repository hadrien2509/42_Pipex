/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:03:48 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/13 19:42:13 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../ft_printf/ft_printf.h"
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

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
}					t_pipex;

/* ************************************************************************** */
/*                                ERROR MESSAGES                              */
/* ************************************************************************** */

# define ERR_INFILE "Infile error\n"
# define ERR_OUTFILE "Outfile error\n"
# define ERR_INPUT "Pipex executable must take 4 arguments\n"
# define ERR_PIPE "Pipe"
# define ERR_CMD "Command not found\n"

/* ************************************************************************** */
/*                                PIPEX_FCTS                                  */
/* ************************************************************************** */

int		show_err(char *err);
void	child_process(t_pipex pipex, char **av, char **envp);
char	**ft_pathname(char *arg, t_pipex *pipex, char **envp);
void	ft_free_pipex(t_pipex *pipex);

/* ************************************************************************** */
/*                                LIBFT_FCTS                                  */
/* ************************************************************************** */

int		ft_strlen(char *str);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif