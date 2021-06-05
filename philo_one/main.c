#include "philosophers.h"

int	ph_fill_prop(t_ph_prop *p, char *argv[], int argc)
{
	int	num[5];

	if (fill_prop_num(num, argc, argv) == 1)
		return (1);
	ph_fill_prop_basic(p, num, argc);
	//if (p->total == 1)
	//	return (err_exit("Cannot be less than 2 philosophers"));
	if (p->t_eat >= p->t_die || p->t_slp >= p->t_die)
		return (err_exit("Philosophers will die too quickly"));
	p->phil = prop_get_phil(p->total);
	if (p->phil == NULL)
		return (err_exit("Could not create philosophers"));
	if (pthread_mutex_init(&p->print_lock, NULL))
		return (perr_exit("Print lock mutex init:"));
	return (0);
}

void	*pcycle(void *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	int			i;

	p = (t_ph_prop *)args;
	i = 0;
	phil = &p->phil[p->i++];
	while (p->eat_num != p->total)
	{
		pcycle_eat_routine(p, phil);
		print_stat(p, phil, STSLP);
		fsleep(ms_to_micros(p->t_slp));
		print_stat(p, phil, STTHNK);
		++i;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_ph_prop	p;
	pthread_t	*threads;
	int			i;

	setbuf(stdout, NULL);
	if (ph_fill_prop(&p, argv, argc) == 1)
		return (1);
	threads = (pthread_t *)malloc(sizeof(pthread_t) * p.total);
	if (!threads)
		return (perr_exit("malloc failed."));
	i = 0;
	while (i < p.total)
	{
		if (pthread_create(&threads[i], NULL, pcycle, &p))
			return (perr_exit("philosopher thread creation failed."));
		fsleep(1);
		++i;
	}
	p.start_t = get_time();
	return (wait_phils(&p));
}
