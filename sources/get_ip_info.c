#include "ft_ping.h"

extern int total_packet_size;
extern t_ping_data ping_data;

void setup_ip_address(char *hostname)
{
	struct addrinfo 	hints, *res;
	int 				status;
	struct sockaddr_in	*ipv4;

	memset(&hints, 0, sizeof(hints));
	// Get IP address from DNS (dns_lookup)
	hints.ai_family = AF_INET;
	if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
	{
		fprintf(stderr, "ping: %s: %s\n", hostname, gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	ipv4 = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &ipv4->sin_addr, ping_data.ip_address, INET_ADDRSTRLEN);
	freeaddrinfo(res);
	
	status = inet_pton(ping_data.addr.sin_family, ping_data.ip_address, &ping_data.addr.sin_addr);
	printf("status inet_pton = %d\n", status);
	if (ping_data.addr.sin_addr.s_addr != INADDR_ANY)
	{
		// Get FQDN (Fully qualified domain name) from IP address (reverse_lookup)
		getnameinfo((struct sockaddr *)&ping_data.addr, sizeof(ping_data.addr),
						ping_data.fqdn, NI_MAXHOST, NULL, 0, NI_NAMEREQD);
	}
	else
	{
		strcpy(ping_data.fqdn, LOOPBACK_ADDRESS);
		strcpy(ping_data.ip_address, LOOPBACK_ADDRESS);
		ping_data.only_addr = true;
	}
}
