/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 20:33:37 by david             #+#    #+#             */
/*   Updated: 2024/12/28 21:36:58 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

static void init_input_data(t_philo_data *data, size_t index, size_t input);
static t_invalid_input_type is_valid_number(char *num, int64_t nb);
static void print_input_error(t_invalid_input_type error_type, size_t ac, char *arg);
static int64_t ft_atoi(char *num);

bool is_input_valid(t_philo_data *data, size_t ac, char **av)
{
    size_t index;
    int64_t num;
    t_invalid_input_type error_type;

    index = 0;
    if (ac < 5)
        error_type = ARGS_TOO_FEW;
    else if (ac > 6)
        error_type = ARGS_TOO_MANY;
    else
    {
        while (++index < ac)
        {
            num = ft_atoi(av[index]);
            error_type = is_valid_number(av[index], num);
            if (error_type)
                break ;
            init_input_data(data, index, num);
        }
    }
    print_input_error(error_type, ac, av[index]);
    return (!error_type);
}

static void init_input_data(t_philo_data *data, size_t index, size_t input)
{
    if (index == 1)
        data->philo_count = input;
    if (index == 2)
        data->time_die = input;
    if (index == 3)
        data->time_eat = input;
    if (index == 4)
        data->time_eat = input;
    if (index == 5)
        data->eat_count = input;
}

static t_invalid_input_type is_valid_number(char *num, int64_t nb)
{
    size_t num_len;

    while(*num == ' ')
        num++;
    if (*num == '+' || *num == '-')
        num++;
    if (*num == '0' && *(num + 1) >= '0' && *(num + 1) <= '9')
        return (INVALID);
    num_len = 0;
    while (*num >= '0' && *num <= '9')
    {
        num_len++;
        num++;
    }
    while(*num == ' ')
        num++;
    if (*num != 0 || !num_len)
        return (INVALID);
    if (num_len > INT_MAX_DIGITS || nb > INT_MAX)
        return (TOO_LARGE);
    if (nb < 0)
        return (NEGATIVE);
    return (VALID);
}

static void print_input_error(t_invalid_input_type error_type, size_t ac, char *arg)
{
    if (error_type == ARGS_TOO_FEW)
        printf("%s %s %lu\n", ERRMSG, ERRMSG_ARG_FEW, ac - 1);
    else if (error_type == ARGS_TOO_MANY)
        printf("%s %s %lu\n", ERRMSG, ERRMSG_ARG_MANY, ac - 1);
    else if (error_type == INVALID)
        printf("%s %s \"%s\"\n", ERRMSG, ERRMSG_NUM_INVALID, arg);
    else if (error_type == TOO_LARGE)
        printf("%s %s \"%s\"\n", ERRMSG, ERRMSG_NUM_LARGE, arg);
    else if (error_type == NEGATIVE)
        printf("%s %s \"%s\"\n", ERRMSG, ERRMSG_NUM_NEG, arg);
}

static int64_t ft_atoi(char *num)
{
    int64_t nb;
    size_t factor;
    int sign;

    while(*num == ' ')
        num++;
    sign = 1;
    if (*num == '-' || *num == '+')
    {
        if (*num == '-')
            sign = -sign;
        num++;
    }
    factor = 10;
    nb = 0;
    while(*num >= '0' && *num <= '9')
    {
        nb = nb * factor + ((*num) - '0');
        num++;
    }
    return (nb * sign);
}
