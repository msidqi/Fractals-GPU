/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:29:04 by msidqi            #+#    #+#             */
/*   Updated: 2018/10/17 23:51:48 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void	const *content, size_t content_size)
{
	t_list	*node1;

	if (!(node1 = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	if (content == NULL)
	{
		node1->content = NULL;
		node1->content_size = 0;
		node1->next = NULL;
	}
	else
	{
		if (!(node1->content = (void *)ft_strdup((char *)content)))
		{
			free(node1);
			return (NULL);
		}
		node1->content_size = content_size;
		node1->next = NULL;
	}
	return (node1);
}
