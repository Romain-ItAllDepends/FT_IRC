/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:19:06 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/25 15:51:20 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdWho(std::string &str, int fd, Server &server)
{
	std::istringstream ss(str);
	std::string cmd, channelName, error, message, modes;
	ss >> cmd >> channelName;
	Client &client = server.getClients(fd);
	if(channelName.empty())
	{
		std::string error =	ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "MODE +l");
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return ;
	}
	Channel &channel = server.getChannel(channelName);
	message = "";
	if(channel.getName() == "no name")
	{
		error = ERR_NOSUCHCHANNEL(channelName);
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return ;
	}
	int nbUser = channel.nbUser();
	std::string listtname = channel.listUsername();
	std::string listtnick = channel.listNicknames();
	std::istringstream tss(listtname);
	std::istringstream iss(listtnick);
	std::string tname, tnick;
	for(;nbUser > 0; --nbUser)
	{
		tss >> tname;
		iss >> tnick;
		if(tnick.find("@") != std::string::npos)
		{
			tnick.erase(0, 1);
			message += WHO(client.getNickname(), tname, tnick, channelName, "@");
		}
		else
			message += WHO(client.getNickname(), tname, tnick, channelName, "");
	}
	message += ENDWHO(channel.getClient(fd), channelName);
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		throw std::runtime_error("Error: An error occured while sending the message!");
}