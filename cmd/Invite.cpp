/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:23:16 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 11:18:19 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdInvite(std::string &str, int fd, Server &server)
{
	std::istringstream ss(str);
	std::string cmd, channelName, user, error, message;
	ss >> cmd >> user >> channelName;
	Client &invitee = server.getClients(user);
	Client &client = server.getClients(fd);

	if (server.inviteVerification(client, invitee, channelName) == true)
	{
		if (invitee.getClientSocket() == -1)
		{
			error = ERR_NOSUCHNICK(CLIENT(client.getNickname(), client.getUsername()), client.getNickname());
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
		} else if (user.empty() == true)
		{
			error = ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "INVITE");
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
		} else
		{
			invitee.setInvited(channelName);
			message = RPL_INVITESNDR(CLIENT(client.getNickname(), client.getUsername()), invitee.getNickname(), channelName);
			if (send(fd, message.c_str(), message.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			message = RPL_INVITERCVR(CLIENT(client.getNickname(), client.getUsername()), invitee.getNickname(), channelName);
			if (send(invitee.getClientSocket(), message.c_str(), message.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
		}
	}
}