#include "philosophers.h"

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
			new[i].id = i + 1;
			new[i].ate_stmp = 0;
			new[i].fin = false;
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

int	ph_fill_prop_basic(t_ph_prop *p, const int num[], int argc)
{
	p->i = 0;
	p->eat_num = 0;
	p->total = num[0];
	p->t_die = num[1];
	p->t_eat = num[2];
	p->t_slp = num[3];
	p->start_t = get_time();
	if (argc == 6)
		p->m_eat_num = num[4];
	else
		p->m_eat_num = 0;
	if (pthread_mutex_init(&p->common_lock, NULL))
		return (perr_exit("Common lock init failed:"));
	if (p->total == 1)
		return (err_exit("Cannot be less than 2 philosophers"));
	if (p->t_eat >= p->t_die || p->t_slp >= p->t_die)
		return (err_exit("Philosophers will die too quickly"));
	return (0);
}

int	fill_prop_sem(t_ph_prop *p)
{
	p->sem = sem_open("sem", O_CREAT | O_EXCL, 0644, p->total);
	if (p->sem == SEM_FAILED)
		return (perr_exit("Semaphore failed:"));
	if (sem_unlink("sem") != 0)
		return (perr_exit("Sem_unlink failed:"));
	return (0);
}
