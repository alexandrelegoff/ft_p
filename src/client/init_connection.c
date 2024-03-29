/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_connection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-goff <ale-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/14 16:15:03 by ale-goff          #+#    #+#             */
/*   Updated: 2019/07/16 12:20:51 by ale-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

static int				init_connection(t_client *client, unsigned short port,
				unsigned long s_addr)
{
	client->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	client->sin.sin_family = AF_INET;
	client->sin.sin_port = port;
	client->sin.sin_addr.s_addr = s_addr;
	return (EXIT_SUCCESS);
}

int						init_addr(t_client *client, char *addr, int port)
{
	client->sockfd = socket(AF_INET, SOCK_STREAM, client->proto->p_proto);
	client->sin.sin_family = AF_INET;
	client->sin.sin_port = htons(port);
	client->hp = gethostbyname(addr);
	if (client->hp)
		client->sin.sin_addr.s_addr = inet_addr((inet_ntoa(*(struct in_addr *)
		client->hp->h_addr_list[0])));
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int						receive_data(t_client *newc, t_client *client)
{
	unsigned short		port;
	unsigned long		s_addr;

	if (recv(client->sockfd, &port, sizeof(unsigned short), 0) == -1)
		return (RECV);
	if (recv(client->sockfd, &s_addr,
	sizeof(newc->sin.sin_addr.s_addr), 0) == -1)
		return (RECV);
	init_connection(newc, port, s_addr);
	if (connect(newc->sockfd, (const struct sockaddr *)&newc->sin,
	sizeof(newc->sin)) == -1)
		return (EXIT_FAILURE);
	return (SUCCESS);
}
