/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:23:24 by huvillat          #+#    #+#             */
/*   Updated: 2025/06/27 20:48:13 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdNick(std::string &str, int fd, Server &server)
{
	std::istringstream ss(str);
	std::string cmd, newnick;
	ss >> cmd >> newnick;
	Client &client = server.getClients(fd);
	if(newnick == client.getNickname())
		return;
	if(newnick.empty())
	{
		std::string error = ERR_NONICKNAMEGIVEN(CLIENT(client.getNickname(), client.getUsername()));
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	std::string listtnick = server.listNicknames();;
	std::istringstream iss(listtnick);
	std::string tnick;
	while(1)
	{
		iss >> tnick;
		if(tnick.empty() || iss.fail())
			break;
		if(newnick == client.getNickname())
			continue;
		if(newnick == tnick)
		{
			std::string error = ERR_NICKNAMEINUSE(client.getNickname(), newnick);
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
	}
	std::string message = RPL_NICK(CLIENT(client.getNickname(), client.getUsername()), newnick);
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		throw std::runtime_error("Error: An error occured while sending the message!");
	client.setNickname(newnick);
}