#include "philosophers.h"

void	phil_tonull(t_phil *p)
{
	p->left = NULL;
	p->right = NULL;
	p->fork_right = NULL;
	p->fin = false;
	p->st_slp = false;
	p->st_thk = false;
	p->st_dea = false;
	p->lhanded = false;
	p->rhanded = false;
	p->ate_stmp = 0;
	p->id = 0;
	pthread_mutex_init(&p->fork_left, NULL);
}

t_phil	*get_phil(t_phil *first, unsigned int n)
{
	t_phil	*prev;
	t_phil	*new;
	int	i;

	i = 1;
	prev = first;
	first->rhanded = true;
	while (i < n)
	{
		new = (t_phil *)malloc(sizeof(t_phil));
		if (new == NULL)
			return (NULL);
		phil_tonull(new);
		if (i % 2 == 0)
			new->rhanded = true;
		else
			new->lhanded = true;
		new->id = (++i);
		prev->right = new;
		prev->fork_right = &new->fork_left;
		new->left = prev;
		prev = new;
	}
	return (new);
}

t_phil  *prop_get_phil(unsigned int n)
{
	t_phil	*first;
	t_phil	*last;

	first = NULL;
	last = NULL;
	if (n == 0)
		return (NULL);
	first = (t_phil *)malloc(sizeof(t_phil));
	if (first == NULL)
		return (NULL);
	phil_tonull(first);
	first->id = 1;
	last = get_phil(first, n);
	first->left = last;
	last->right = first;
	last->fork_right = &first->fork_left;
	return (first);
}

int fill_prop_num(long int *num, int argc, char *argv[])
{
	int i;

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

int	ph_fill_prop(t_ph_prop *p, char *argv[], int argc)
{
	long int num[5];

	if (fill_prop_num(num, argc, argv) == 1)
		return (1);
	p->i = 1;
	p->total = num[0];
	if (p->total == 1)
		return (err_exit("Cannot be less than 2 philosophers"));
	p->t_die = num[1];
	p->t_eat = num[2];
	p->t_slp = num[3];
	if (p->t_eat >= p->t_die || p->t_slp >= p->t_die)
		return (err_exit("Philosophers will die before finishing eating or sleeping"));
	gettimeofday(&p->t, NULL);
	p->start_t = get_time();
	if (argc == 6)
		p->m_eat_num = num[4];
	else
		p->m_eat_num = 0;
	pthread_mutex_init(&p->print_lock, NULL);
	pthread_mutex_init(&p->fork_lock, NULL);
	p->phil = prop_get_phil(p->total);
	if (p->phil == NULL)
		return (err_exit("Could not create a philosopher"));
	return (0);
}