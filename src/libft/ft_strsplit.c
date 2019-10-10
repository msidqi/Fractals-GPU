/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/13 20:14:26 by msidqi            #+#    #+#             */
/*   Updated: 2018/10/19 02:17:53 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_wc(char *s, char c)
{
	unsigned int w;
	unsigned int i;

	w = 0;
	i = 0;
	while (s[i])
	{
		if ((s[i] == c && s[i + 1] != c && s[i + 1] != '\0'))
			w++;
		i++;
	}
	if (s[0] != c)
		w++;
	return (w);
}

char			**ft_strsplit(char const *s, char c)
{
	char			**t;
	size_t			sublen;
	unsigned int	i;
	int				j;

	if (!s || !(t = (char **)malloc(sizeof(char*) * (ft_wc((char *)s, c) + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] != '\0' && j < ft_wc((char *)s, c))
	{
		sublen = 0;
		while (s[i] == c && s[i])
			i++;
		while (s[i] != c && s[i])
		{
			i++;
			sublen++;
		}
		t[j++] = ft_strsub(s, i - sublen, sublen);
		while (s[i] != c && s[i])
			i++;
	}
	t[ft_wc((char *)s, c)] = NULL;
	return (t);
}
