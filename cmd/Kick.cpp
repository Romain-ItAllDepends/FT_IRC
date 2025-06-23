/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:23:16 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 10:38:37 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

size_t find_comment(std::string &str, size_t i)
{
	while(i < str.size())
	{
		while(i < str.size() && isspace(str[i]))
		{
			if(i + 1 < str.size() && isalnum(str[i + 1]))
			{
				i++;
				return(i);
			}
			++i;
		}
		++i;
	}
	return(0);
}

void	cmdKick(std::string &str, int fd, Server &server)
{
	std::istringstream ss(str);
	std::string cmd, channelName, user, message, error, sender;
	ss >> cmd >> channelName >> user;

	Client client = server.getClients(fd);
	if (client.getClientSocket() == -1)
		throw std::runtime_error("Error: This client doesn't exist!");
	sender = client.getNickname();

	if(channelName.size() != 0 && user.size() != 0)
	{
		size_t i = str.find(user);
		i = find_comment(str, i);
		client = server.getClients(user);
		if(i == 0)
		{
			message = RPL_KICK(CLIENT(client.getNickname(), client.getUsername()), channelName, user);
			server.handleKick(fd, message, channelName, client.getClientSocket(), sender);
		}
		else
		{
			str = &str[i];
			message = RPL_NEWKICK(CLIENT(client.getNickname(), client.getUsername()), channelName, user, str);
			server.handleKick(fd, message, channelName, client.getClientSocket(), sender);
		}
	}
	else if (channelName.empty() == true)
		error = ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "KICK");
}
