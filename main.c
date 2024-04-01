
#include <stdbool.h>
#include <readline/readline.h>

// une command c'est quoi ? arg1 arg2 < redirection1 arg3 >redirection2 arg4 arg5 > redirection3  |   arg6                                

typedef struct s_argument_to_expand t_argument_to_expand;
typedef struct s_argument_to_expand
{
	char *content;
	t_argument_to_expand* next;

} t_argument_to_expand;

typedef struct s_redirection_to_expand t_redirection_to_expand;
typedef struct s_redirection_to_expand
{

	char *arg;

	enum
	{
		REDIRECTION_OUTFILE,
		REDIRECTION_INFILE,
		REDIRECTION_APPEND,
		// REDIRECTION_HEREDOC
	} type;

	t_redirection_to_expand *next;

} t_redirection_to_expand;


typedef struct s_command t_command;
typedef struct s_command{

	t_argument_to_expand *arguments;
	t_redirection_to_expand *redirections;

	t_command *next;
} t_command_to_expand;

typedef struct
{
	bool did_succeed;

	t_command_to_expand *commands;

} t_command_line_parsing_result;

typedef struct
{
	bool did_succeed;

	t_command_to_expand command;

	const char *remaining_line;

} t_command_parsing_result;

typedef struct
{
	bool did_succeed;

	t_redirection_to_expand redirection;

	const char *remaining_line;

} t_redirection_parsing_result;

typedef struct
{
	bool did_succeed;

	t_argument_to_expand argument;

	const char *remaining_line;

} t_argument_parsing_result;

t_argument_parsing_result parse_argument(const char* command_line)
{
	const char *remaining_line;
	t_argument_parsing_result result;

	char *arg = NULL;

	remaining_line = skip_spaces(command_line);
	while (remaining_line[0] != '|' et != '<' et != '>' et != ' ' et != '\0')
	{
		if (remaining_line[0] == '\'')
		{
			// rejoindre la quote suivante sinon erreur
			// puis copier 
		}
		else if (remaining_line[0] == '\"')
		{
			// rejoindre la quote suivante sinon erreur
			// puis copier
		}
		else
		{
			//charactere normal
			// considerer strjoin ou memcpy
		}
	}
	return result 
	;
}

// manque parse redirection qui depend de parse command



t_command_parsing_result parse_command(const char* command_line) {
	t_command_parsing_result result;
	const char *remaining_line;

	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0) {
		result.did_succeed = false;
		return result;
	}

	while (ft_strlen(remaining_line) > 0 && remaining_line[0] != '|')
	{


		if (remaining_line[0] == '>' || remaining_line[0] == '<') {
			t_redirection_parsing_result redirection_result =	parse_redirection(remaining_line);
			if (!redirection_result.did_succeed)
			{
				result.did_succeed = false;
				return result;
			}
			ft_redirection_to_expand_addback(&result.command.redirections, redirection_result.redirection);
		}
		else
		{
			t_argument_parsing_result argument_result =	parse_argument(remaining_line);
			if (!argument_result.did_succeed) {
				result.did_succeed = false;
				return result;
			}
			// addback to list of arguments
		}
		remaining_line = skip_spaces(remaining_line);
	}
	result.did_succeed = true;
	return result;
}


t_command_line_parsing_result ft_parse_command_line(const char *command_line) {

	const char						*remaining_line;
	t_command_line_parsing_result	result;

	result.commands = NULL;
	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0) {
		result.did_succeed = true;
		return result;
	}

	while (ft_strlen(remaining_line) > 0) {
		t_command_parsing_result command_parsing_result = parse_command(remaining_line);
		if (!command_parsing_result.did_succeed) {
			result.did_succeed = false;
			return result;
		}
		ft_command_to_expand_add_back(&result.commands, &command_parsing_result.command);

		remaining_line = command_parsing_result.remaining_line;
		remaining_line = skip_spaces(remaining_line);

		if (remaining_line[0] == '|') {
			remaining_line = skip_one_character(remaining_line);
			remaining_line = skip_spaces(remaining_line);

			if (ft_strlen(remaining_line) == 0)
			{
					result.did_succeed = false;
					return result;
			}
		}
	}
	result.did_succeed = true;
	return result;
}

int run_read_eval_print_loop(void) {
	while (true) {
		char *command_line = readline(NULL);
		if (!command_line) {
			....
		}

		t_command_line_parsing_result parsing_result = ft_parse_command_line(command_line);
		if (!parsing_result.did_succeed) {
			// handle error
		}
	}
}