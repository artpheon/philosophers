#include "philosophers.h"

int	ph_fill_prop(t_ph_prop *p, char *argv[], int argc)
{
	int	num[5];

	if (fill_prop_num(num, argc, argv))
		return (1);
	if (ph_fill_prop_basic(p, num, argc))
		return (1);
	if (fill_prop_sem(p))
		return (1);
	p->phil = prop_get_phil(p->total);
	if (p->phil == NULL)
		return (perr_exit("A philosopher was not created"));
	return (0);
}

void	*pcycle(void *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	int			i;

	p = (t_ph_prop *)args;
	phil = &p->phil[p->i++];
	i = 0;
	while (p->eat_num != p->total)
	{
		pcycle_eat_routine(p, phil, i);
		print_stat(p, phil, STSLP);
		fsleep(ms_to_micros(p->t_slp));
		print_stat(p, phil, STTHNK);
		++i;
	}
	return (NULL);
}

int	philo_setup(pthread_t *threads, t_ph_prop *p)
{
	int	i;

	i = 0;
	p->start_t = get_time();
	while (i < p->total)
	{
		if (pthread_create(&threads[i], NULL, pcycle, p))
			return (perr_exit("Pthread_create for philosophers failed:"));
		if (pthread_detach(threads[i]))
			return (perr_exit("Pthread_detach for philosophers failed:"));
		++i;
	}
	return (0);
}

int	philo_exit(t_ph_prop *p)
{
	int	i;

	sem_close(p->sem);
	sem_close(p->common_lock);
	i = 0;
	while (i < p->total)
		sem_close(p->phil[i++].eat_block);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_ph_prop	p;
	pthread_t	*threads;

	setbuf(stdout, NULL);
	if (ph_fill_prop(&p, argv, argc) == 1)
		return (1);
	threads = (pthread_t *)malloc(sizeof(pthread_t) * p.total);
	if (!threads)
		return (perr_exit("Malloc for threads failed"));
	if (philo_setup(threads, &p))
		return (1);
	if (watcher_setup(&p))
		return (1);
	return (philo_exit(&p));
}
