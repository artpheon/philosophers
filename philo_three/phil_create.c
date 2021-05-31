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

int	ph_fill_prop_basic(t_ph_prop *p, int num[], int argc)
{
	p->i = 0;
	p->total = num[0];
	p->t_die = num[1];
	p->t_eat = num[2];
	p->t_slp = num[3];
	p->start_t = get_time();
	if (argc == 6)
		p->m_eat_num = num[4];
	else
		p->m_eat_num = 0;
	p->stat = (int *)malloc(sizeof(int) * p->total);
	p->pid = (pid_t *)malloc(sizeof(pid_t) * p->total);
	if (p->stat && p->pid)
	{
		p->stat[0] = 0;
		p->pid[0] = 0;
	}
	else
		return (perr_exit("Malloc failed for stat/pid:"));
	return (0);
}

int	fill_prop_sem(t_ph_prop *p)
{
	p->sem = sem_open("sem", O_CREAT | O_EXCL, 0644, p->total);
	if (p->sem == SEM_FAILED)
		return (perr_exit("Semaphore failed:"));
	if (sem_unlink("sem") != 0)
		return (perr_exit("Sem_unlink failed:"));
	p->common = sem_open("sem_common", O_CREAT | O_EXCL, 0644, 1);
	if (p->common == SEM_FAILED)
		return (perr_exit("Semaphore common failed:"));
	if (sem_unlink("sem_common") != 0)
		return (perr_exit("Sem_unlink for sem_common failed:"));
	return (0);
}
