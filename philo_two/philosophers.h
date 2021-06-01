#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# define STFRK 1
# define STEAT 2
# define STSLP 3
# define STTHNK 4
# define STFIN 5
# define STDEA 0
# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_phil
{
	int				id;
	uint64_t		ate_stmp;
	pthread_mutex_t	eat_block;
}	t_phil;

typedef struct s_ph_prop
{
	int				total;
	int				t_die;
	int				t_eat;
	int				t_slp;
	int				m_eat_num;
	int				eat_num;
	int				i;
	sem_t			*sem;
	pthread_mutex_t	common_lock;
	uint64_t		start_t;
	t_phil			*phil;
}	t_ph_prop;

int			ft_atoi(const char *str);
int			ft_isdigit(int c);
int			is_strnum(const char *str);
int			err_exit(char *str);
int			perr_exit(char *str);
uint64_t	ms_to_micros(unsigned int ms);
void		fsleep(unsigned int mcrsec);
uint64_t	get_time(void);
t_phil		*prop_get_phil(int n);
int			fill_prop_num(int *num, int argc, char *argv[]);
int			ph_fill_prop_basic(t_ph_prop *p, const int num[], int argc);
int			fill_prop_sem(t_ph_prop *p);
void		print_stat(t_ph_prop *p, t_phil *phil, int stat);
void		pcycle_eat_routine(t_ph_prop *p, t_phil *phil, int round);
void		*watch_stat(void *args);
int			watcher_setup(t_ph_prop *p);

#endif
