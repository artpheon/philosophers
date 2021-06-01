#include "philosophers.h"

int	ph_fill_prop(t_ph_prop *p, char *argv[], int argc)
{
	int	num[5];

	if (fill_prop_num(num, argc, argv) == 1)
		return (1);
	if (p->total == 1)
		return (err_exit("Cannot be less than 2 philosophers"));
	ph_fill_prop_basic(p, num, argc);
	if (p->t_eat >= p->t_die || p->t_slp >= p->t_die)
		return (err_exit("Philosophers will die too quickly"));
	p->phil = prop_get_phil(p->total);
	if (p->phil == NULL)
		return (perr_exit("Malloc failed for phil creation:"));
	if (fill_prop_sem(p))
		return (1);
	return (0);
}

int	fork_philosophers(t_ph_prop	*p)
{
	int	i;
	int	child;

	i = 0;
	while (i < p->total)
	{
		child = fork();
		if (child == -1)
			return (perr_exit("Fork failed:"));
		if (child == 0)
			pcycle(p);
		else
		{
			usleep(1);
			++p->i;
			p->pid[i] = child;
		}
		++i;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_ph_prop	p;
	int			stat;

	setbuf(stdout, NULL);
	if (ph_fill_prop(&p, argv, argc) == 1)
		return (1);
	fork_philosophers(&p);
	return (watcher_setup(&p));
}
