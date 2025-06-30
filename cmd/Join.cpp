/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:37:42 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/30 08:36:20 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void joinChannel(std::string &str, int fd, Server &server)
{
	std::istringstream ss(str);
	std::string cmd, channelName, key, password, error;
	ss >> cmd >> channelName >> key;
	Channel &channel = server.getOrCreateChannel(channelName);
	password = channel.getPassword();

	if (channelName[0] != '#')
	{
		error = ERR_NOSUCHCHANNEL(channelName);
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	} 
	else
	{
		if (password.size() > 0)
		{
			Client &client = server.getClients(fd);
			if (channel.getInvitOnly() == true || password == key)
			{
				if (client.getClientSocket() == -1)
					throw std::runtime_error("Error: This client doesn't exist!");
				if (channelName.size() > 50)
					throw std::runtime_error("Error: Too long channel name!");
				server.handleJoinCommand(client, channelName, key);
				return ;
			}
			error = ERR_PASSWDMISMATCH(CLIENT(client.getNickname(), client.getUsername()));
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
		Client &client = server.getClients(fd);
		if (client.getClientSocket() == -1)
			throw std::runtime_error("Error: This client doesn't exist!");
		if (channelName.size() > 50)
		{
			error = ERR_NOSUCHCHANNEL(channelName);
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
		}
		server.handleJoinCommand(client, channelName, password);
	}
}