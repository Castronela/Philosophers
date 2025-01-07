
# include "philo.h"

void test_print_data(t_philo_data *data, int buffer)
{

    printf("\n\n%*s : %u", buffer, "philo_count",data->philo_count);
    printf("\n%*s : %u", buffer, "time_die",data->time_die);
    printf("\n%*s : %u", buffer, "time_eat",data->time_eat);
    printf("\n%*s : %u", buffer, "time_sleep",data->time_sleep);
    printf("\n%*s : %d", buffer, "eat_count",data->eat_count);

    printf("\n\n%*s : ", buffer, "table_w_forks");
    for (size_t i = 0; i < data->philo_count; i++)
        printf("%u, ", data->table_w_forks[i]);
    printf("\n\n");
}

void test_print_exit_status(t_philos *philo)
{
    if (philo->status == DEAD)
        printf("%d exited with status DEAD\n", philo->philo_id);
    else if (philo->status == STOPPED)
        printf("%d exited with status STOPPED\n", philo->philo_id);
    else if (philo->status == EXITED)
        printf("%d exited with status EXITED\n", philo->philo_id);
    else if (philo->status == INT_FAIL)
        printf("%d exited with status INT_FAIL\n", philo->philo_id);
}
