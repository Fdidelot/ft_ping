#include "ft_ping.h"

void	set_sockaddr_struct(struct sockaddr_in *addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = 0;
}

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

void create_icmp_packet(struct icmphdr *icmp_package, int sequence)
{
	icmp_package->type = ICMP_ECHO;
	icmp_package->code = 0;
	icmp_package->un.echo.id = getpid();
	icmp_package->un.echo.sequence = sequence;
	icmp_package->checksum = calculate_checksum((uint16_t *)icmp_package, sizeof(struct icmphdr));
}