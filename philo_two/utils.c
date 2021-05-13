#include "philosophers.h"

int err_exit(char *str)
{
	printf("Error: %s. Programme exits with value of 1.\n", str);
	return (1);
}

int perr_exit(char *str)
{
	perror(str);
	return (1);
}

unsigned long	ms_to_micros(unsigned int ms)
{
	return (ms * 1000);
}

unsigned long	micros_to_ms(unsigned long ms)
{
	return (ms / 1000);
}

uint64_t get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	fsleep(unsigned int mcrsec)
{
	uint64_t start;
	uint64_t now;

	start = get_time();
	now = start;
	while (ms_to_micros(now - start) < mcrsec)
	{
		usleep(100);
		now = get_time();
	}
}

int	ft_atoi(const char *str)
{
	int sign;
	int nbr;
	int i;

	sign = 1;
	nbr = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43)
		i++;
	else if (str[i] == 45)
	{
		sign *= -1;
		i++;
	}
	else if (str[i] < 48 || str[i] > 57)
		return (nbr);
	while (str[i] > 47 && str[i] < 58)
		nbr = nbr * 10 + (str[i++] - '0');
	return (nbr * sign);
}

int	ft_isdigit(int c)
{
	if (c > 47 && c < 58)
		return (1);
	else
		return (0);
}

int	is_strnum(const char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0 && str[i] != '+')
			return (1);
		++i;
	}
	return (0);
}
