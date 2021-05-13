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
	struct s_phil	*left;
	struct s_phil	*right;
	unsigned int	id;
	unsigned long long ate_stmp;
	bool	fin;
	bool	st_eat;
	bool	st_slp;
	bool	st_thk;
	bool	st_dea;
}	t_phil;

typedef struct s_ph_prop
{
	unsigned int	total;
	unsigned int	t_die;
	unsigned int	t_eat;
	unsigned int	t_slp;
	unsigned int	m_eat_num;
	int	i;
	struct timeval	t;
	sem_t *sem;
	pthread_mutex_t	common_lock;
	unsigned long long start_t;
	t_phil	*phil;
}	t_ph_prop;


int	err_exit(char *str);
int perr_exit(char *str);
void	fsleep(unsigned int mcrsec);
unsigned long	ms_to_micros(unsigned int ms);
unsigned long	micros_to_ms(unsigned long ms);
bool watch_stat_finish(t_ph_prop *p);
uint64_t get_time(void);
void	try_die(t_ph_prop *p, t_phil *phil);
void ft_putnbr_fd(uint64_t n, int fd);
int	ft_atoi(const char *str);
int	ft_isdigit(int c);
int	is_strnum(const char *str);
t_phil  *prop_get_phil(unsigned int n);
int	ph_fill_prop(t_ph_prop *p, char *argv[], int argc);

#endif