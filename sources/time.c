#include "ft_ping.h"

double	time_diff(struct timespec start_time, struct timespec end_time)
{
	return (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
            (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
}
