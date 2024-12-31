
# include "philo.h"

void test_print_data(t_philo_data *data, int buffer)
{
    printf("\n%*s : %lu", buffer, "time_start_ms",data->time_start_ms);

    printf("\n\n%*s : %u", buffer, "philo_count",data->philo_count);
    printf("\n%*s : %u", buffer, "time_die",data->time_die);
    printf("\n%*s : %u", buffer, "time_eat",data->time_eat);
    printf("\n%*s : %u", buffer, "time_sleep",data->time_sleep);
    printf("\n%*s : %d", buffer, "eat_count",data->eat_count);

    printf("\n\n%*s : ", buffer, "forks");
    for (size_t i = 0; i < data->philo_count; i++)
        printf("%u, ", data->forks[i]);
    printf("\n\n");
}
