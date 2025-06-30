/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:30:13 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/30 08:14:29 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdPart(std::string &str, int fd, Server &server)
{
	std::istringstream ss(str);
	std::string cmd, channelName, leave, error;
	ss >> cmd >> channelName >> leave;

	if (channelName.empty() == true)
	{
		Client &client = server.getClients(fd);
		if (client.getClientSocket() == -1)
			throw std::runtime_error("Error: This client doesn't exist!");
		error = ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "PART");
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return ;
	}

	str = channelName + " leave";
	server.handleRemoveClientFomChannel(fd, channelName, str);
}
