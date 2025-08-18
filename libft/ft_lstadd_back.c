/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:52:33 by devriez           #+#    #+#             */
/*   Updated: 2025/05/19 14:54:48 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp_lst;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp_lst = *lst;
	while (temp_lst->next)
		temp_lst = temp_lst->next;
	temp_lst->next = new;
}
