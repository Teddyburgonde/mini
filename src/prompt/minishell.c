/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:15:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/20 10:50:30 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int ac, char **av, char *envp[])
{
	t_env	*env;
	t_vars	*vars;

	env = NULL;
	vars = NULL;
	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("Error: Too much arguments\n", 2);
		return (1);
	}
	init_env(&env, envp);
	ft_signal();
	if (ft_readline(&env, vars) == 1)
	{
		ft_lstclear_env(&env);
		return (1);
	}
	ft_lstclear_env(&env);
	return (0);
}

// parsing 
// fork , ecouter exit status du dernier
// waitpid 

// cas particulier : une seul command et que c'est un builtind -> il ne faut pas fork !
// builtind dans le process parent si il y a une seul command
// sur le cd et le pwd est set avec env
// si la taille est de 1 , faire une autre route 

// parsing dans les fork 
// couper les espaces -> problematique -> si dans la variable tu as une
// quote

// flag caractere , associer une valeur pour dire si elle est special ou pas 
// struct le caractere et le bool 
// 


// "" '' $ 

// typedef struct s_char_list t_char_list;
// struct s_char_list
// {
	
// 	char	value;
// 	t_bool	was_in_a_variable;

// 	t_char_list *next;
// };

// typedef struct s_argument t_argument;
// struct s_argument
// {
// 	t_char_list *chars;

// 	t_argument* next;	
// };


// t_argument *ft_expand_vars_in_argument(const char* argument)
// {
// 	// pour chaque charactere de l'argument:
// 		// si je suis une quote simple ':
// 			// je vais chercher la quote simple suivante. (ELLE EXISTE NECESSAIREMENT)
// 			// et je remplace tous les characteres rencontrés sans les marquer.
// 		// sinon
// 			// si je suis un dollar $:
// 				// si je suis suivi de \0:
// 					// j'ecris un dollar
// 				// sinon:
// 					// je remplace en marquant les characteres (was_in_a_variable = TRUE)
// 			// sinon je remplace le charactere sans le marquer.
// }

// t_argument *ft_split_argument(const t_argument *argument_to_split)
// {
// 	t_argument *splitted_arguments;

// 	splitted_arguments = NULL;

// 	// iterer sur les character de argument_to_split

// 	// couper aux espaces.

// 	return (splitted_arguments);
// }

// t_argument* ft_expand_argument(const t_argument_to_expand argument)
// {
// 	t_argument *argument_with_expanded_vars;
// 	t_argument *splitted_arguments;
// 	t_argument *final_arguments;
	
// 	// deux étapes:
// 	//  1) remplacement des variables + flaguer les characteres -> t_char_list *
// 	argument_with_expanded_vars = ft_expand_vars_in_argument(argument.content);
// 	// 2) split aux espaces qui sont pas entre quotes.
// 	splitted_arguments = ft_split_argument(argument_with_expanded_vars);
// 	// 3) virer les quotes qui sont pas flaguees
// 	final_arguments = ft_remove_quotes_from_arguments(splitted_arguments);

// 	return (final_arguments);
	
// }