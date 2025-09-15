/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   johanes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:40:00 by devriez           #+#    #+#             */
/*   Updated: 2025/09/11 20:23:12 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// -----------------------------------------------------------------------------
// Helpers: free memory
// -----------------------------------------------------------------------------
static void	free_char_array(char **arr)
{
	int i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	free_redirect_list(t_redirect *head)
{
	t_redirect *tmp;

	while (head)
	{
		tmp = head;
		if (tmp->file)
			free(tmp->file);
		head = head->next;
		free(tmp);
	}
}

void	free_command_list(t_command *head)
{
	t_command *tmp;

	while (head)
	{
		tmp = head;
		if (tmp->name)
			free(tmp->name);
		if (tmp->args)
			free_char_array(tmp->args);
		free_redirect_list(tmp->redirections);
		head = head->next;
		free(tmp);
	}
}

// -----------------------------------------------------------------------------
// Split string by a single character
// -----------------------------------------------------------------------------
static char	**split_string(const char *s, char delimiter)
{
	char	**result;
	int		count;
	int		i;
	int		start;
	int		j;

	if (!s)
		return (NULL);
	count = 1;
	i = 0;
	while (s[i])
		if (s[i++] == delimiter)
			count++;
	result = ft_calloc(count + 1, sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == delimiter)
		{
			result[j++] = ft_strndup(s + start, i - start);
			start = i + 1;
		}
		i++;
	}
	result[j] = ft_strndup(s + start, i - start);
	return (result);
}

// -----------------------------------------------------------------------------
// Parse redirections
// -----------------------------------------------------------------------------
static t_redirect *parse_redirects(char **tokens)
{
    t_redirect *head = NULL;
    t_redirect *current = NULL;

    for (int i = 0; tokens[i]; i++)
    {
        t_redirect *new = NULL;

        if (ft_strcmp(tokens[i], "<") == 0
            || ft_strcmp(tokens[i], ">") == 0
            || ft_strcmp(tokens[i], "<<") == 0
            || ft_strcmp(tokens[i], ">>") == 0)
        {
            if (!tokens[i + 1])
            {
                // Ошибка: нет файла после редиректа, очищаем всё
                free_redirect_list(head);
                return NULL;
            }

            new = ft_calloc(1, sizeof(t_redirect));
            if (!new)
            {
                free_redirect_list(head);
                return NULL;
            }

            if (ft_strcmp(tokens[i], "<") == 0)
                new->type = INPUT;
            else if (ft_strcmp(tokens[i], ">") == 0)
                new->type = OUTPUT;
            else if (ft_strcmp(tokens[i], "<<") == 0)
                new->type = HEREDOC;
            else
                new->type = APPEND;

            new->file = ft_strdup(tokens[i + 1]);
            if (!new->file)
            {
                free(new);
                free_redirect_list(head);
                return NULL;
            }

            if (!head)
                head = new;
            else
                current->next = new;
            current = new;

            // Помечаем токены, чтобы их не включать в args
            tokens[i][0] = '\0';
            tokens[i + 1][0] = '\0';
            i++; // пропускаем имя файла
        }
    }
    return head;
}


// -----------------------------------------------------------------------------
// Parse single command
// -----------------------------------------------------------------------------
static t_command *parse_single_command(char *command_str)
{
    t_command *cmd = ft_calloc(1, sizeof(t_command));
    if (!cmd)
        return NULL;

    // Разбиваем команду на токены (через пробел)
    char **tokens = split_string(command_str, ' ');
    if (!tokens)
    {
        free(cmd);
        return NULL;
    }

    // Парсим редиректы, убирая их из токенов
    cmd->redirections = parse_redirects(tokens);

    // Считаем количество аргументов (без имени команды и пустых токенов)
    int count = 0;
    int i = 0;
    while (tokens[i])
    {
        if (ft_strlen(tokens[i]) > 0)
            count++;
        i++;
    }

    if (count == 0)
    {
        free(cmd);
        free_char_array(tokens);
        return NULL;
    }

    // Выделяем массив аргументов (без имени команды)
    cmd->args = ft_calloc(count, sizeof(char *));
    if (!cmd->args)
    {
        free(cmd);
        free_char_array(tokens);
        return NULL;
    }

    int j = 0;
    i = 0;
    while (tokens[i])
    {
        if (ft_strlen(tokens[i]) > 0)
        {
            if (!cmd->name)
            {
                cmd->name = ft_strdup(tokens[i]); // первый токен → имя команды
                if (!cmd->name)
                {
                    free_command_list(cmd);
                    free_char_array(tokens);
                    return NULL;
                }
            }
            else
            {
                cmd->args[j] = ft_strdup(tokens[i]); // остальные токены → args
                if (!cmd->args[j])
                {
                    free_command_list(cmd);
                    free_char_array(tokens);
                    return NULL;
                }
                j++;
            }
        }
        i++;
    }

    free_char_array(tokens);
    return cmd;
}


// -----------------------------------------------------------------------------
// Main parser: split by pipes
// -----------------------------------------------------------------------------
t_command	*johannes_func(char *line)
{
	char		**pipe_cmds;
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;
	int			i;

	if (!line || ft_strlen(line) == 0)
		return (NULL);
	pipe_cmds = split_string(line, '|');
	if (!pipe_cmds)
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (pipe_cmds[i])
	{
		new_cmd = parse_single_command(pipe_cmds[i]);
		if (!new_cmd)
		{
			free_command_list(head);
			free_char_array(pipe_cmds);
			return (NULL);
		}
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		i++;
	}
	free_char_array(pipe_cmds);
	return (head);
}
