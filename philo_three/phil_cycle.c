#include "philosophers.h"

void	print_stat(t_ph_prop *p, t_phil *phil, const int stat)
{
	sem_wait(p->common);
	if (stat == 1)
		printf("%llu\t%d took fork\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 2)
		printf("%llu\t%d is eating\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 3)
		printf("%llu\t%d is sleeping\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 4)
		printf("%llu\t%d is thinking\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 5)
	{
		printf("%llu\tsimulation finished!\n", get_time() - p->start_t);
		return ;
	}
	if (stat == 0)
	{
		printf("%llu\t%d is dead\n", get_time() - p->start_t, (int) phil->id);
		return ;
	}
	sem_post(p->common);
}

void	pcycle_eat_routine(t_ph_prop *p, t_phil *phil)
{
	sem_wait(p->sem);
	print_stat(p, phil, STFRK);
	sem_wait(p->sem);
	print_stat(p, phil, STFRK);
	print_stat(p, phil, STEAT);
	phil->ate_stmp = get_time() - p->start_t;
	fsleep(ms_to_micros(p->t_eat));
	sem_post(p->sem);
	sem_post(p->sem);
}

void	*pcycle_check_death(void *arg)
{
	t_ph_prop	*p;
	t_phil		*phil;

	p = arg;
	phil = &p->phil[p->i];
	while ((int)(get_time() - p->start_t - phil->ate_stmp) < p->t_die)
		usleep(100);
	print_stat(p, phil, STDEA);
	exit(-1);
}

int	pcycle(t_ph_prop *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	pthread_t	watcher;
	int			i;

	p = (t_ph_prop *)args;
	phil = &p->phil[p->i];
	i = 0;
	if (pthread_create(&watcher, NULL, &pcycle_check_death, p))
		return (perr_exit("Inner watcher was not created:"));
	if (pthread_detach(watcher))
		return (perr_exit("Inner watcher was not detached:"));
	while (1)
	{
		pcycle_eat_routine(p, phil);
		if (i == p->m_eat_num - 1)
			break ;
		print_stat(p, phil, STSLP);
		fsleep(ms_to_micros(p->t_slp));
		print_stat(p, phil, STTHNK);
		++i;
	}
	exit(1);
}
