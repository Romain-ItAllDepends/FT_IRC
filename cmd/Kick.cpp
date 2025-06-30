/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:23:16 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/26 10:58:54 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdKick(std::string &str, int fd, Server &server)
{
	char sep;
	std::istringstream ss(str);
	std::string cmd, channelName, user, message, error, sender;
	ss >> cmd >> channelName >> user >> sep;

	Client &client = server.getClients(fd);
	if (client.getClientSocket() == -1)
		throw std::runtime_error("Error: This client doesn't exist!");
	sender = client.getNickname();

	if(channelName.size() != 0 && user.size() != 0)
	{
		size_t i = str.find(user + " :");
		if (i != std::string::npos)
			std::getline(ss, str);
		if(i == std::string::npos)
		{
			message = RPL_KICK(CLIENT(client.getNickname(), client.getUsername()), channelName, user);
			server.handleKick(fd, message, channelName, server.getClients(user).getClientSocket(), sender);
		}
		else if (i != std::string::npos)
		{
			message = RPL_NEWKICK(CLIENT(client.getNickname(), client.getUsername()), channelName, user, str);
			server.handleKick(fd, message, channelName, server.getClients(user).getClientSocket(), sender);
		}
	}
	else if (channelName.empty() == true)
	{
		error = ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "KICK");
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
}
