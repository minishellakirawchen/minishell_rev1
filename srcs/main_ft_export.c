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

int	main(int argc, char **argv)
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
			{"export", NULL},
			{"export", "a=b", "c=d", NULL},
			{"export", "a+=b", "c+=d", NULL},
			{"export", "__+=hoge", "_=piyo", NULL},
			{"export", "a=b", "a=c", "a=d", NULL},
			{"export", "SHELL=minishell", "_+=:add", "SHELL+=:add", "SHELVL+=:hoge huga", NULL},
			{"export", "SHELL=minishell", "SHELL+=:add1", "SHELL+=:add2", "SHELL+=:add3", NULL},
			{"export", "a=b", "a+=c", "a+=d", "a+=e", "a+=f", "a+=g", NULL},
			{"export", "a=bbb", "a=b", NULL},
			{"export", "a=bbb", "a=", NULL},
			{"export", "a=b", "a+=c", "a+=d", "a=e", NULL},
			{"export", "a", "b", "c", "d", NULL},
			{"export", "=", "a=", "a==", "a == = = == = == =", NULL},
			{"export", " a=", " a=b", " a b c=d", "=a=b", NULL},
			{"export", " = a =", "\ta=b", "a=\tb", "a=\nb", NULL},
			{"export", "a=a", "aa=aa", "aaa=aaa", "PWDD=test", NULL},
			{"export", "key1=val1", "key2_=val2", "_key3=val3", "__key4=val4", "___key5__=val5", "k_e_y_6=val6", NULL},
			{"export", "key1=val1", "key1++value1", "key1++=value2", "key1++=+value3", NULL},

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
