/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-15 02:24:42 by mtarza13          #+#    #+#             */
/*   Updated: 2025-01-15 02:24:42 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0 ||
			ft_strcmp(cmd, "cd") == 0 ||
			ft_strcmp(cmd, "pwd") == 0 ||
			ft_strcmp(cmd, "export") == 0 ||
			ft_strcmp(cmd, "unset") == 0 ||
			ft_strcmp(cmd, "env") == 0 ||
			ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin(char **args, t_shell *shell)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, shell));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args));
	else if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, shell));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, shell));
	else if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(args, shell));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, shell));
	return (1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	
	// Handle -n flag properly (all test cases from Excel)
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		int j = 2;
		int valid_flag = 1;
		
		// Check if all characters after -n are 'n'
		while (args[i][j])
		{
			if (args[i][j] != 'n')
			{
				valid_flag = 0;
				break;
			}
			j++;
		}
		
		if (valid_flag && args[i][j] == '\0')
		{
			newline = 0;
			i++;
		}
		else
			break;
	}
	
	// Print arguments with spaces
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*home;
	char	*oldpwd;
	char	cwd[4096];

	// Handle too many arguments
	if (args[1] && args[2])
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	
	// Save current PWD as OLDPWD
	oldpwd = get_env_value(shell->env, "PWD");
	if (oldpwd)
		oldpwd = ft_strdup(oldpwd);
	
	// Determine target directory
	if (!args[1])
	{
		home = get_env_value(shell->env, "HOME");
		if (!home)
		{
			printf("cd: HOME not set\n");
			free(oldpwd);
			return (1);
		}
		path = home;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (!path)
		{
			printf("cd: OLDPWD not set\n");
			free(oldpwd);
			return (1);
		}
		printf("%s\n", path);
	}
	else
		path = args[1];
		
	// Change directory
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	
	// Update PWD and OLDPWD
	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env_value(shell->env, "PWD", cwd);
		free(shell->pwd);
		shell->pwd = ft_strdup(cwd);
	}
	
	if (oldpwd)
	{
		set_env_value(shell->env, "OLDPWD", oldpwd);
		free(shell->oldpwd);
		shell->oldpwd = oldpwd;
	}
	
	return (0);
}

int	builtin_pwd(char **args)
{
	char	cwd[4096];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}

int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	(void)args;
	i = 0;
	while (i < shell->env->count)
	{
		if (shell->env->envp[i] && ft_strchr(shell->env->envp[i], '='))
			printf("%s\n", shell->env->envp[i]);
		i++;
	}
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*name;
	char	*value;
	char	*equals;

	if (!args[1])
	{
		// Show all exported variables (sorted in real bash)
		i = 0;
		while (i < shell->env->count)
		{
			if (shell->env->envp[i])
			{
				printf("declare -x ");
				equals = ft_strchr(shell->env->envp[i], '=');
				if (equals)
				{
					*equals = '\0';
					printf("%s=\"%s\"\n", shell->env->envp[i], equals + 1);
					*equals = '=';
				}
				else
					printf("%s\n", shell->env->envp[i]);
			}
			i++;
		}
		return (0);
	}
	
	// Process each argument
	i = 1;
	while (args[i])
	{
		equals = ft_strchr(args[i], '=');
		if (equals)
		{
			*equals = '\0';
			name = args[i];
			value = equals + 1;
			
			if (!is_valid_identifier(name))
			{
				printf("export: `%s=%s': not a valid identifier\n", name, value);
				*equals = '=';
				return (1);
			}
			
			set_env_value(shell->env, name, value);
			*equals = '=';
		}
		else
		{
			if (!is_valid_identifier(args[i]))
			{
				printf("export: `%s': not a valid identifier\n", args[i]);
				return (1);
			}
			// Export without value
			set_env_value(shell->env, args[i], NULL);
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
		return (0);
		
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			printf("unset: `%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		else
			unset_env_value(shell->env, args[i]);
		i++;
	}
	return (ret);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int		exit_code;
	long	num;

	printf("exit\n");
	
	if (!args[1])
		exit(shell->env->exit_status);
		
	if (args[1] && args[2])
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	
	if (!is_numeric_string(args[1]))
	{
		printf("exit: %s: numeric argument required\n", args[1]);
		exit(2);
	}
	
	num = ft_atol(args[1]);
	exit_code = (int)(num % 256);
	if (exit_code < 0)
		exit_code += 256;
		
	exit(exit_code);

