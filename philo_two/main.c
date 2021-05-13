#include "philosophers.h"
//ms - milliseconds. 1 millisecond = 1000 microsenconds

t_phil *get_next_phil(t_ph_prop *p)
{
	t_phil *ph;

	++p->i;
	ph = p->phil;
	while (ph->id != p->i)
	{
		ph = ph->right;
	}
	return (ph);
}

bool watch_stat_finish(t_ph_prop *p)
{
	int i;
	t_phil *phil;

	i = 0;
	phil = p->phil;
	while (i < p->total)
	{
		if (phil->fin)
		{
			phil = phil->right;
			++i;
		}
		else
			return (false);
	}
	return (true);
}

void print_stat(t_ph_prop * const p, t_phil *phil, const int stat)
{
	pthread_mutex_lock(&p->common_lock);
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
		phil->st_dea = true;
		return ;
	}
	pthread_mutex_unlock(&p->common_lock);
}

void pcycle_eat_routine(t_ph_prop *p, t_phil *phil, int round)
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
	if (round == p->m_eat_num - 1)
		phil->fin = true;
}

void *pcycle(void *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	int i;

	p = (t_ph_prop *)args;
	phil = get_next_phil(p);
	i = 0;
	while (i < p->m_eat_num || p->m_eat_num == 0)
	{
		pcycle_eat_routine(p, phil, i);
		print_stat(p, phil, STSLP);
		fsleep(ms_to_micros(p->t_slp));
		print_stat(p, phil, STTHNK);
		++i;
	}
	return (NULL);
}

void *watch_stat(void *args)
{
	t_ph_prop	*p;
	t_phil *phil;
	uint64_t now;

	p = (t_ph_prop *)args;
	phil = p->phil;
	while (1)
	{
		usleep(10);
		now = get_time() - p->start_t;
		if (now - phil->ate_stmp >= p->t_die)
			break ;
		if (watch_stat_finish(p))
		{
			print_stat(p, phil, STFIN);
			return (NULL);
		}
		phil = phil->right;
	}
	print_stat(p, phil, STDEA);
	return (NULL);
}

int main(int argc, char *argv[])
{
	t_ph_prop p;
	pthread_t *threads;
	pthread_t watcher;

	size_t i;

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
		pthread_detach(threads[i]);
		++i;
	}
	if (pthread_create(&watcher, NULL, watch_stat, &p))
		return (perr_exit("watcher was not created."));
	if (pthread_join(watcher, NULL) == 0)
		return (0);
	return (0);
}
