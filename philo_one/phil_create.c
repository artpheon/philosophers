#include "philosophers.h"

int	phil_tonull(t_phil *p)
{
	p->fork_right = NULL;
	p->fin = false;
	p->ate_stmp = 0;
	p->ate_num = 0;
	p->id = 0;
	if (pthread_mutex_init(&p->fork_left, NULL))
		return (perr_exit("Fork left mutex init:"));
	if (pthread_mutex_init(&p->wait_fork, NULL))
		return (perr_exit("Wait fork mutex init:"));
	if (pthread_mutex_lock(&p->wait_fork))
		return (perr_exit("Wait fork mutex lock"));
	return (0);
}

void	phil_set(t_phil *ph, int index, int size)
{
	int	next;

	if (index == size - 1)
		next = 0;
	else
		next = index + 1;
	ph[index].fork_right = &ph[next].fork_left;
	ph[index].id = index + 1;
}

t_phil	*prop_get_phil(int n)
{
	t_phil	*new;
	int		i;

	new = (t_phil *)malloc(sizeof(t_phil) * n);
	if (new)
	{
		i = 0;
		while (i < n)
		{
			if (phil_tonull(&new[i]))
				return (NULL);
			phil_set(new, i, n);
			++i;
		}
		return (new);
	}
	return (NULL);
}

int	fill_prop_num(int *num, int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc > 6 || argc < 5)
		return (err_exit("Wrong arguments number"));
	while (i < argc && argv[i])
	{
		if (is_strnum(argv[i]))
			return (err_exit("Argument is not a number"));
		num[i - 1] = ft_atoi(argv[i]);
		if (num[i - 1] < 0)
			return (err_exit("Invalid argument - negative number"));
		++i;
	}
	return (0);
}

void	ph_fill_prop_basic(t_ph_prop *p, const int num[], int argc)
{
	p->i = 0;
	p->total = num[0];
	p->t_die = num[1];
	p->t_eat = num[2];
	p->t_slp = num[3];
	p->start_t = get_time();
	p->eat_num = 0;
	if (argc == 6)
		p->m_eat_num = num[4];
	else
		p->m_eat_num = 0;
}
