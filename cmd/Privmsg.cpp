/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 14:48:31 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 11:14:54 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdPrivmsg(std::string &str, int fd, Server &server)
{
	char	sep;
	std::istringstream ss(str);
	std::string cmd, target, msg, error;
	ss >> cmd >> target >> sep;

	std::getline(ss, msg);
	Client &client = server.getClients(fd);
	size_t pos = str.find(" :");
	if (str[pos + 1] == ':' && target[0] == '#')
	{
		if (client.getClientSocket() == -1)
			throw std::runtime_error("Error: This client doesn't exist!");
		server.handlePrivmsgCommand(client, target, msg);
		return ;
	}
	if (str[pos + 1] == ':' && target[0] != '#')
	{
		Client &clientTarget = server.getClients(target);
		if (clientTarget.getClientSocket() == -1)
		{
			error = ERR_NOSUCHNICK(CLIENT(client.getNickname(), client.getNickname()), target);
			if (send(fd, error.c_str(), error.size(), 0) == -1)
		      throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
		if (target.empty() == true)
		{
			error = ERR_NORECIPIENT(CLIENT(client.getNickname(), client.getUsername()), "PRIVMSG");
			if (send(fd, error.c_str(), error.size(), 0) == -1)
		      throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
		server.handlePrivmsgCommand(client, target, msg);
		return ;
	}
	error = ERR_NOTEXTTOSEND(CLIENT(client.getNickname(), client.getUsername()));
	if (send(fd, error.c_str(), error.size(), 0) == -1)
		throw std::runtime_error("Error: An error occured while sending the message!");
}
