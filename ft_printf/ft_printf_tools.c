/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 15:22:02 by hgeissle          #+#    #+#             */
/*   Updated: 2022/11/03 13:13:32 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_printstr(char *str)
{
	int		i;

	i = 0;
	if (!str)
	{	
		if (write(1, "(null)", 6) == -1)
			return (-1);
		return (6);
	}
	while (str[i])
	{
		if (write(1, &str[i], 1) == -1)
			return (-1);
		i++;
	}
	return (i);
}

int	ft_printnbr(int nbr, char *base, unsigned int baselen)
{
	int				nbrlen;
	unsigned int	unbr;
	int				len;

	nbrlen = 0;
	if (nbr < 0)
	{	
		unbr = -nbr;
		if (write(1, "-", 1) == -1)
			return (-1);
		nbrlen += 1;
	}
	else
		unbr = nbr;
	if (unbr > (baselen - 1))
	{	
		len = ft_printnbr(unbr / baselen, base, baselen);
		if (len == -1)
			return (-1);
		nbrlen += len;
	}
	if (write(1, &base[unbr % baselen], 1) == -1)
		return (-1);
	nbrlen += 1;
	return (nbrlen);
}

int	ft_printunbr(unsigned int unbr, char *base, unsigned int baselen)
{
	int				nbrlen;
	int				len;

	nbrlen = 0;
	if (unbr > (baselen - 1))
	{	
		len = ft_printunbr(unbr / baselen, base, baselen);
		if (len == -1)
			return (-1);
		nbrlen += len;
	}
	len = write(1, &base[unbr % baselen], 1);
	if (len == -1)
		return (-1);
	nbrlen += len;
	return (nbrlen);
}

int	ft_printptr(unsigned long int nbr)
{
	int				nbrlen;
	int				len;

	nbrlen = 0;
	if (nbr > 15)
	{	
		len = ft_printptr(nbr / 16);
		if (len == -1)
			return (-1);
		nbrlen += len;
	}
	else
	{
		if (write(1, "0x", 2) == -1)
			return (-1);
		nbrlen += 2;
	}
	len = write(1, &"0123456789abcdef"[nbr % 16], 1);
	if (len == -1)
		return (-1);
	nbrlen += len;
	return (nbrlen);
}
