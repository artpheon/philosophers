#include "philosophers.h"

void	*watch_stat(void *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	int			i;

	p = (t_ph_prop *)args;
	phil = p->phil;
	i = 0;
	while (p->eat_num != p->total)
	{
		usleep(100);
		if (p->t_die < (int)(get_time() - p->start_t - phil[i].ate_stmp))
		{
			print_stat(p, &phil[i], STDEA);
			return (NULL);
		}
		++i;
		if (i == p->total)
			i = 0;
	}
	print_stat(p, phil, STFIN);
	return (NULL);
}

int	watcher_setup(t_ph_prop *p)
{
	pthread_t	watcher;
	int			ret;

	ret = pthread_create(&watcher, NULL, watch_stat, p);
	if (ret)
		perr_exit("watcher was not created.");
	ret = pthread_join(watcher, NULL);
	if (ret)
		perr_exit("Could not join watcher:");
	sem_close(p->sem);
	return (ret);
}
