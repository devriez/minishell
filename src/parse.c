/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:19:35 by johartma          #+#    #+#             */
/*   Updated: 2025/09/01 15:46:02 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

// 1. Readline
// 2. Split into words (seperated by spaces, if not in "" or '')
// 3. Expand the $ in the prompt, ("Edge"-case: "", but not single qoutes)
// 4. seperate <>| that had no space to other words
// 5. detect redirection and write redirections to files


int	main(void)
{
	char	*prompt;
	char	**lex_result;

	prompt = readline(NULL);
	lex_result = lex(prompt);
	free(prompt);
	
	return (0);
}
