/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:19:35 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 13:53:32 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

// 1. Readline
// 2. Split into words (seperated by spaces, if not in "" or '')
// 3. Classify the prompts
// 4. Expand the $ in the prompt, ("Edge"-case: "", but not single qoutes)
// 5. seperate <>| that had no space to other words
// 6. detect redirection and write redirections to files


int	main(void)
{
	char		*prompt;
	char		**lex_result;
	t_tokens	*tokens;

	prompt = readline(NULL);
	lex_result = lex(prompt);
	free(prompt);
	tokens = get_type(lex_result, &tokens);
	
	return (0);
}
