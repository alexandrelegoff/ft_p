/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-goff <ale-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/14 23:32:23 by ale-goff          #+#    #+#             */
/*   Updated: 2019/07/16 12:22:44 by ale-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

const t_handle_fun		g_handle_fun[] = {
	{"ls", 2, handle_ls},
	{"cd", 2, handle_cd},
	{"get", 3, handle_get},
	{"pwd", 3, handle_pwd},
	{"put", 3, handle_put},
	{"quit", 4, handle_quit}
};

static int		analyze_data(t_client *client, char *buff)
{
	uint8_t		i;

	i = -1;
	while (++i < 6)
	{
		if (!ft_strncmp(buff, g_handle_fun[i].cmd, g_handle_fun[i].size))
			return (g_handle_fun[i].fn(client, buff));
	}
	return (UNKWN_CMD);
}

static void		print_status(int status)
{
	if (status == SUCCESS)
		ft_putendl(GRN": The command has been executed successfully\n"RESET);
	if (status == UNKWN_CMD)
		ft_putendl(RED": The command you entered is invalid\n"RESET);
	if (status == RECV)
		ft_putendl(RED": The command receive failed\n"RESET);
	if (status == CON)
		ft_putendl(RED": The command connect failed\n"RESET);
	if (status == ARG)
		ft_putendl(RED": Invalid number of arguments\n"RESET);
	if (status == FILE_ERROR)
		ft_putendl(RED": Couldn't open the file.\n"RESET);
	if (status == PATH)
		ft_putendl(RED": Invalid path.\n"RESET);
}

static int		launch_client(t_client *client)
{
	char		buff[1024];
	ssize_t		r;
	int			status;

	while (42)
	{
		ft_bzero(buff, 1024);
		ft_putstr(RESET "Client -> ");
		r = read(0, buff, sizeof(buff));
		if (r > 0)
			buff[r - 1] = '\0';
		else
			break ;
		write(client->sockfd, buff, r);
		status = analyze_data(client, buff);
		if (recv(client->sockfd, buff, 8, 0) < 0)
			return (error_program(E_RECV));
		else if (!ft_strcmp("SUCCESS", buff))
			ft_putstr(GRN "SUCCESS" RESET);
		else
			ft_putstr(RED "FAILURE" RESET);
		print_status(status);
	}
	return (EXIT_SUCCESS);
}

int				create_client(char *addr, int port)
{
	t_client	client;
	int			status;

	if ((client.proto = getprotobyname(PROTOCOL)) == 0)
		return (error_program(E_UNKWN));
	ft_bzero((char *)&client.sin, sizeof(client.sin));
	getcwd(client.path, sizeof(client.path));
	init_addr(&client, addr, port);
	if (connect(client.sockfd, (const struct sockaddr *)&client.sin,
	sizeof(client.sin)) == -1)
		return (error_program(E_CONNECT));
	mkdir(ft_strcat(client.path, "/clientdata/"), 0744);
	ft_strcat(client.init_path, "/");
	client.len = ft_strlen(client.path);
	chdir(client.path);
	status = launch_client(&client);
	handle_quit(&client, NULL);
	close(client.sockfd);
	return (status);
}
