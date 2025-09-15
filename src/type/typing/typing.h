/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 13:51:01 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPING_H
# define TYPING_H

# include "../includes/minishell.h"

/* Function to classify tokens by type (pipe, redirect, word, etc.) */
t_tokens	*get_type(char **lexed, t_tokens **tokens);

#endif


