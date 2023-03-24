/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:38:03 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/24 14:31:15 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static int	ft_strslen(const char *str, char c)
{
	int	i;
	int	len;
	int	counted;

	i = 0;
	len = 0;
	counted = 0;
	while (str[i])
	{
		if (str[i] != c && !counted)
		{
			len++;
			counted = 1;
		}
		else if (str[i] == c)
			counted = 0;
		i++;
	}
	return (len);
}

static char	*ft_range(const char *str, int min, int max)
{
	int		i;
	char	*range;

	range = malloc(sizeof(char) * (max - min + 1));
	if (!range)
		return (0);
	i = 0;
	while (str[min + i] && (min + i) < max)
	{
		range[i] = str[min + i];
		i++;
	}
	range[i] = '\0';
	return (range);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static void	set_tab(const char *str, char c, char **tab)
{
	int	i;
	int	len;
	int	split;

	i = 0;
	split = 0;
	while (str[i])
	{
		len = 0;
		while (str[i + len] && str[i + len] != c)
			len++;
		if (len != 0)
		{
			tab[split] = ft_range(str, i, i + len);
			if (!tab[split])
			{
				ft_free_tab(tab);
				return ;
			}
			split++;
			i = i + len;
		}
		else
			i++;
	}
}

char	**ft_split(char const *s, char c)
{
	char	**tab;

	if (!s)
		return (NULL);
	tab = malloc(sizeof(char *) * (ft_strslen(s, c) + 1));
	if (!tab)
		return (NULL);
	tab[ft_strslen(s, c)] = 0;
	set_tab(s, c, tab);
	return (tab);
}
