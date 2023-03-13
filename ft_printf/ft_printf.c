/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 14:06:34 by hgeissle          #+#    #+#             */
/*   Updated: 2023/02/15 20:33:56 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_checkformat(const char c, va_list ap, int len)
{
	int		old_len;

	old_len = len;
	if (c == 'c')
		len += ft_printchar(va_arg(ap, int));
	if (c == 's')
		len += ft_printstr(va_arg(ap, char *));
	if (c == 'd' || c == 'i')
		len += ft_printnbr(va_arg(ap, int), "0123456789", 10);
	if (c == 'u')
		len += ft_printunbr(va_arg(ap, unsigned int), "0123456789", 10);
	if (c == 'x')
		len += ft_printunbr(va_arg(ap, unsigned int), "0123456789abcdef", 16);
	if (c == 'X')
		len += ft_printunbr(va_arg(ap, unsigned int), "0123456789ABCDEF", 16);
	if (c == 'p')
		len += ft_printptr(va_arg(ap, unsigned long int));
	if (c == '%')
		len += ft_printchar('%');
	if (len == old_len - 1)
		return (-1);
	return (len);
}

int	ft_printf(const char *str, ...)
{
	va_list		ap;
	size_t		i;
	int			len;
	int			old_len;

	i = 0;
	len = 0;
	va_start(ap, str);
	while (str[i])
	{
		old_len = len;
		if (str[i] == '%')
		{	
			i++;
			len = ft_checkformat(str[i], ap, len);
		}
		else
			len += write(1, &str[i], 1);
		i++;
		if (len == old_len - 1)
			return (-1);
	}
	va_end(ap);
	return (len);
}
