#include "philosophers.h"

int	err_exit(char *str)
{
	printf("Error: %s. Programme exits with value of 1.\n", str);
	return (1);
}

int	perr_exit(char *str)
{
	perror(str);
	return (1);
}

uint64_t	ms_to_micros(unsigned int ms)
{
	return (ms * 1000);
}

uint64_t	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	fsleep(useconds_t mcrsec)
{
	uint64_t	start;
	uint64_t	now;

	start = get_time();
	now = start;
	while (ms_to_micros(now - start) < mcrsec)
	{
		usleep(10);
		now = get_time();
	}
}
