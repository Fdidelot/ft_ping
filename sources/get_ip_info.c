#include <ft_ping.h>

void	dns_lookup(t_ping_data *p_data, char *ip)
{
	struct addrinfo		hints, *res;
	int					status;
	struct sockaddr_in	*ipv4;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	status = getaddrinfo(ip, NULL, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "ft_ping: %s: %s\n", ip, gai_strerror(status));
		close(p_data->sockfd);
		exit(EXIT_FAILURE);
	}
	ipv4 = (struct sockaddr_in *)res->ai_addr;
	if (res->ai_canonname != NULL)
	{
		if (strlen(res->ai_canonname) < MAX_SIZE_NAME)
			strcpy(p_data->canonname, res->ai_canonname);
	}
	if (inet_ntop(AF_INET, &ipv4->sin_addr, p_data->ip_address, INET_ADDRSTRLEN) == NULL)
	{
		perror("inet_ntop");
		close(p_data->sockfd);
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(res);
}

void	reverse_dns_lookup(t_ping_data *p_data)
{
	int status;

	status = inet_pton(p_data->addr.sin_family, p_data->ip_address, &p_data->addr.sin_addr);
	if (status == -1)
	{
		perror("inet_pton");
		close(p_data->sockfd);
		exit(EXIT_FAILURE);
	}
	if (p_data->addr.sin_addr.s_addr == INADDR_ANY)
	{
		strcpy(p_data->fqdn, LOOPBACK_ADDRESS);
		strcpy(p_data->ip_address, LOOPBACK_ADDRESS);
		p_data->direct_ip = true;
	}
	else
	{
		getnameinfo((struct sockaddr *)&p_data->addr, sizeof(p_data->addr),
						p_data->fqdn, NI_MAXHOST, NULL, 0, NI_NAMEREQD);
	}
}

void	get_ip_info(t_ping_data *p_data, char *ip)
{
	dns_lookup(p_data, ip);
	reverse_dns_lookup(p_data);
	if (strcmp(p_data->arg, p_data->ip_address) == 0)
		p_data->direct_ip = true;
}