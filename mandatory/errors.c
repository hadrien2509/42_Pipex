/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:57:54 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/25 16:32:03 by hgeissle         ###   ########.fr       */
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
