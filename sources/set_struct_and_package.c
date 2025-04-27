#include "ft_ping.h"

extern t_ping_data p_data;

uint16_t calculate_checksum(void *b, int len)
{
	uint16_t *buf = b;
	uint32_t sum = 0;

	while (len > 1)
	{
		sum += *buf++;
		len -=2;
	}
	if (len == 1)
		sum += *(uint8_t *)buf;
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)(~sum);
}

void	set_icmp_package(struct icmphdr *icmp_package, unsigned int sequence)
{
	icmp_package->type = ICMP_ECHO;
	icmp_package->code = 0;
	icmp_package->un.echo.id = p_data.pid;
	icmp_package->un.echo.sequence = sequence;
	icmp_package->checksum = calculate_checksum((uint16_t *)icmp_package, sizeof(struct icmphdr));
}

void	set_p_data(t_ping_data *p_data)
{
	p_data->pid = getpid();
	p_data->rtt_min = 1e9;
	p_data->direct_ip = false;
	p_data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (p_data->sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	p_data->addr.sin_family = AF_INET;
	p_data->addr.sin_port = 0;
}
