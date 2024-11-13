#include "ft_ping.h"

void	calculate_time_stats(t_ping_data *p_data)
{
	p_data->percent_loss = (((float)p_data->packet_send - (float)p_data->packet_received)
								/ (float)p_data->packet_send) * 100;
	if (p_data->packet_received != 0)
	{
		p_data->rtt_avg = p_data->rtt_sum / (float)p_data->packet_received;
		p_data->rtt_mdev = sqrt((p_data->rtt_sum_sq / (float)p_data->packet_received)
									- (p_data->rtt_avg * p_data->rtt_avg));
	}
	else
	{
		p_data->rtt_avg = 0;
		p_data->rtt_mdev = 0;
	}
	p_data->total_time = time_diff(p_data->start_time, p_data->end_time);
	if (p_data->total_time < 0)
		p_data->total_time = 0;
}

double	time_diff(struct timespec start_time, struct timespec end_time)
{
	return (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
            (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
}

void	store_time_info(t_ping_data *p_data, struct timespec start_time,
												struct timespec end_time)
{
	p_data->rtt = time_diff(start_time, end_time);
	if (p_data->rtt_min > p_data->rtt)
		p_data->rtt_min = p_data->rtt;
	if (p_data->rtt_max < p_data->rtt)
		p_data->rtt_max = p_data->rtt;
	p_data->rtt_sum += p_data->rtt;
	p_data->rtt_sum_sq += (p_data->rtt * p_data->rtt);
}