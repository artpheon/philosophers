#include "philosophers.h"

void	print_stat(t_ph_prop * const p, t_phil *phil, const int stat)
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
	phil = p->phil;
	while (get_time() - p->start_t - phil->ate_stmp < p->t_die)
		usleep(10);
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
	phil = p->phil;
	i = 0;
	if (pthread_create(&watcher, NULL, &pcycle_check_death, p))
		return (perr_exit("inner watcher was not created."));
	pthread_detach(watcher);
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

bool	exited_all(const int stat[], unsigned int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (stat[i])
			++i;
		else
			return (false);
	}
	return (true);
}

int wait_all(pid_t pids[], int status[], const unsigned int size)
{
	int	i;

	i = 0;
	while (status[i] != 255 << 8)
	{
		waitpid(pids[i], &status[i], WNOHANG);
		++i;
		if (i == size)
			i = 0;
		if (exited_all(status, size))
		{
			return (STFIN);
		}
	}
	return (STDEA);
}

void	*watch_stat(void *args)
{
	t_ph_prop	*p;

	p = args;
	p->i = wait_all(p->pid, p->stat, p->total);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_ph_prop	p;
	pthread_t	watcher;
	pid_t		child;
	size_t		i;

	setbuf(stdout, NULL);
	if (ph_fill_prop(&p, argv, argc) == 1)
		return (1);
	i = 0;
	p.start_t = get_time();
	while (i < p.total)
	{
		child = fork();
		if (child == 0)
		{
			pcycle(&p);
		}
		else
		{
			p.pid[i] = child;
			p.phil = p.phil->right;
		}
		++i;
	}
	i = 0;
	if (pthread_create(&watcher, NULL, watch_stat, &p))
		return (perr_exit("watcher was not created."));
	if (pthread_join(watcher, NULL))
		perr_exit("watcher could not join");
	if (p.i == STDEA)
	{
		while (i < p.total)
			kill(p.pid[i++], 9);
	}
	return (0);
}
