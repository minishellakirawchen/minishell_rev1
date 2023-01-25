#include "minishell.h"

int	test_echo(int no, char **input, char *expected)
{
	printf("\n## test:%d, ##\n", no);fflush(stdout);
	printf("vvvvvvvvvvvvvvvvvvvvvvvvvv\n");fflush(stdout);
	printf(" expected:[%s]\n", expected);fflush(stdout);
	printf(" ft_echo :[");fflush(stdout);
	ft_echo(input);fflush(stdout);
	printf("]");fflush(stdout);
	printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^\n");fflush(stdout);
	return (no++);
}

int	main(void)
{
	int no = 0;
	char *test_input[20][20] = {
			{"echo", NULL},
			{"echo", "hello", NULL},
			{"echo", "hello",  "world", NULL},
			{"echo", "hoge", "", "huga", NULL},
			{"echo", "hoge", " ", "huga", NULL},
			{"echo", "hoge", "", "", "", "", "", "huga", NULL},
			{"echo", "hoge", "            ",  "            ","huga", NULL},
			{"echo", "newline->\n<-newline", NULL},
			{"echo", "-unknown_op", NULL},
			{"echo", "-n", "no newline:-n", NULL},
			{"echo", "-n", "no newline:-n", "-n", NULL},
			{"echo", "-n", "-n", "no newline:-n -n", NULL},
			{"echo", "-n", "-n", "-n", "-n", "-n", "no newline: -n -n -n -n -n", NULL},
			{"echo", "-nnnnnnnnnnnn", "no newline:-nnnnnnnnnnnn", NULL},
			{"echo", "-n-n-n-n-n-n", "no newline:-n-n-n-n-n-n", NULL},
			{"echo", "-N", "optin -N", NULL},
			{"echo", "-n", NULL},
			{NULL},
	};
	char *expected[20] = {
			"\n",
			"hello\n",
			"hello world\n",
			"hoge  huga\n",
			"hoge   huga\n",
			"hoge      huga\n",
			"hoge                           huga\n",
			"newline->\\n<-newline\n",
			"-unknown_op\n",
			"no newline:-n",
			"no newline:-n -n",
			"no newline:-n -n",
			"no newline: -n -n -n -n -n",
			"no newline:-nnnnnnnnnnnn",
			"-n-n-n-n-n-n no newline:-n-n-n-n-n-n\n",
			"-N optin -N\n",
			"",
			NULL,
	};


	while (test_input[no][0])
	{
		test_echo(no, test_input[no], expected[no]);
		no++;
	}
}
