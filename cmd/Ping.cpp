/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 09:42:32 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/25 12:43:43 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdPing(std::string &str, int fd, Server &server)
{
	(void) server;
	std::stringstream ss(str);
	std::string cmd, token, message, error;
	ss >> cmd >> token;

	Client &client = server.getClients(fd);
	if(token.empty())
	{
		error = ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "PING");
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	message = PONG(token);
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		throw std::runtime_error("Error: An error occured while sending the message!");
}