/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-goff <ale-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/15 15:58:53 by ale-goff          #+#    #+#             */
/*   Updated: 2019/07/15 22:29:32 by ale-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <libft.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# define RED   "\x1B[31m"
# define GRN   "\x1B[32m"
# define YEL   "\x1B[33m"
# define BLU   "\x1B[34m"
# define MAG   "\x1B[35m"
# define CYN   "\x1B[36m"
# define WHT   "\x1B[37m"
# define RESET "\x1B[0m"

# define PROTOCOL		"tcp"
# define E_UNKWN		"an error occured"
# define E_SOCKET		"socket failed"
# define E_BIND			"bind failed"
# define E_ACCEPT		"accept failed"

typedef struct			s_server
{
	int					sockfd;
	int					csockfd;
	unsigned int		clen;
	size_t				len;
	char				*path;
	char				intial_path[1024];
	struct protoent		*proto;
	struct sockaddr_in	sin;
	struct sockaddr_in	csin;
}						t_server;

int						error_usage(char *str);
int						error_program(char *str);
int						create_server(int port);

int						handle_quit(t_server *server, char *arg);
int						handle_pwd(t_server *server, char *arg);
int						handle_ls(t_server *server, char *arg);
int						handle_put(t_server *server, char *arg);
int						handle_get(t_server *server, char *arg);
int						handle_cd(t_server *server, char *arg);

int						exec_cmd(char *cmd, char **arg, t_server *newc);

int						init_connection(t_server *server);
void					accept_con(t_server *server, t_server *newc);
int						verify_permission(t_server *server, char **arr);

int						count_directory_below(char *arr);
int						count_directory(char *arr);
void					cut_path(char *arr);
void					free_arr(char **arr);
typedef int				t_handle_func(t_server *server, char *arg);

typedef struct			s_handle_fun
{
	char				*cmd;
	uint8_t				size;
	t_handle_func		*fn;
}						t_handle_fun;

#endif
