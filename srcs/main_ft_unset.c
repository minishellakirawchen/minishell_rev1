#include "minishell.h"

static int	init_info(t_info *info)
{
	info->envlist_head = get_envlist();
	if (!info->envlist_head)
	{
		info->envlist_head = free_1d_alloc(info->envlist_head);
		return (FAILURE);
	}
	info->tokenlist_head = NULL;
//	ft_dprintf(STDERR_FILENO, "## envlist_head ##\n");
//	ft_lstiter(info->envlist_head, print_key_value);
	return (SUCCESS);
}

int main (int argc, char **argv)
{
	int		exit_status;
	t_info	info;

	if (argc != 1 || !argv)
	{
		ft_dprintf(STDERR_FILENO,
				   "[Error]Too many argument."
				       "       Input following:$> ./minishell");
		return (EXIT_FAILURE);
	}

	size_t	test_cnt = 18;
		char *test_input[20][20] = {
			{"unset", NULL},
			{"unset", "a", NULL},
			{"unset", "a", "c", NULL},
			{"unset", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", NULL},
			{"unset", "x", "x", "x", "x", "x", "x", "x", "x", "x", "x", NULL},
			{"unset", "_", "", NULL},
			{"unset", "aa",NULL},
			{"unset", "a=b",NULL},
			{"unset", "a=", "a+", "+","a+=hoge",NULL},
			{"unset", "",NULL},
			{"unset", " ",NULL},
			{NULL},
	};
	size_t	i = 0;
	while (i < test_cnt)
	{
		// init info
		info = (t_info){ 0 };
		init_info(&info);

		// test
		printf("\n\n### TEST %zu ###\n", i);
		debug_print_2d_arr(test_input[i], "input");

		exit_status = execute_builtin(&info, test_input[i]);

		printf("exit_status: %d\n", exit_status);
		printf("vvvvvvvvvvvvvvvvvvv debug_env vvvvvvvvvvvvvvvvvvv\n");
		ft_env(&info);
		printf("^^^^^^^^^^^^^^^^^^^ debug_env ^^^^^^^^^^^^^^^^^^^\n");
		i++;

		// free info
		free_info(&info);
	}
	system("leaks -q minishell");
	return (exit_status);
}

