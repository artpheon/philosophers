#include "philosophers.h"

bool	exited_all(int stat[], int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (stat[i] == 0)
			++i;
		else
			return (false);
	}
	return (true);
}

int	wait_all(pid_t pids[], int status[], int size)
{
	int	i;

	i = 0;
	while (status[i] != 1 << 8)
	{
		waitpid(pids[i], &status[i], WNOHANG);
		++i;
		if (i == size)
			i = 0;
		if (exited_all(status, size))
			return (STFIN);
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

int	watcher_setup(t_ph_prop	*p)
{
	pthread_t	watcher;
	int			i;

	if (pthread_create(&watcher, NULL, watch_stat, p))
		return (perr_exit("Watcher was not created:"));
	if (pthread_join(watcher, NULL))
		return (perr_exit("Watcher could not join:"));
	i = 0;
	if (p->i == STDEA)
	{
		while (i < p->total)
			kill(p->pid[i++], 9);
	}
	else
		print_stat(p, p->phil, p->i);
	sem_close(p->sem);
	sem_close(p->common);
	return (0);
}
