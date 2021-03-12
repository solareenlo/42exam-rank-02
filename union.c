#include <unistd.h>

int	main(int argc, char *argv[])
{
	int	flag[128] = {0};
	int	i;
	int	j;

	if (argc == 3)
	{
		j = 1;
		while (j < 3)
		{
			i = 0;
			while (argv[j][i])
			{
				if (flag[(unsigned char)argv[j][i]] == 0)
				{
					write(1, &argv[j][i], 1);
					flag[(unsigned char)argv[j][i]] = 1;
				}
				i++;
			}
			j++;
		}
	}
	write(1, "\n", 1);
	return 0;
}
