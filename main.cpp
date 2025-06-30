/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 13:15:30 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/26 10:38:06 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <exception>
#include <signal.h>

void sigint(int pid)
{
	(void) pid;
	throw std::runtime_error("Error: Ctrl+C signal!");
}

void sigquit(int pid)
{
	(void) pid;
	throw std::runtime_error("Error: Ctrl+/ signal!");
}

void sigpipe(int pid)
{
	(void) pid;
	throw std::runtime_error("Error: Unknown crash!");
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		PRINTRB "Error: A port and password are required!" END
		return 1;
	}
	try
	{
		signal(SIGINT, sigint);
		signal(SIGQUIT, sigquit);
		signal(SIGPIPE, sigpipe);
		Server	server("JEANJAK", av[1], av[2]);
		server.startServer();
	}
	catch (std::exception &e)
	{
		PRINTRB e.what() END
	}
	return 0;
}
