/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:09:50 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/27 20:04:43 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

typedef void (*FunctionPointer)(std::string&, int, Server&);

void    readTheLine(std::string &str, int fd, Server &server)
{
	std::string listcmd[13] = {"JOIN", "PRIVMSG", "INVITE", "KICK", "MODE", "TOPIC", "PART", "QUIT", "PING", "WHO", "NICK", "PASS", "USER"};
	FunctionPointer funct[11] = {joinChannel, cmdPrivmsg, cmdInvite, cmdKick, cmdMode, cmdTopic, cmdPart, cmdQuit, cmdPing, cmdWho, cmdNick};
	if (str.find("CAP LS 302") == std::string::npos && server.handleConnexion(fd, str) == false)
		return;

	int i = -1;
	while(++i < 13)
		if(str.find(listcmd[i]) == 0)
			break;
	switch(i)
	{
		case 0:
			funct[0](str, fd, server);
			break;
		case 1:
			funct[1](str, fd, server);
			break;
		case 2:
			funct[2](str, fd, server);
			break;
		case 3:
			funct[3](str, fd, server);
			break;
		case 4:
			funct[4](str, fd, server);
			break;
		case 5:
			funct[5](str, fd, server);
			break;
		case 6:
			funct[6](str, fd, server);
			break;
		case 7:
			funct[7](str, fd, server);
			break;
		case 8:
			funct[8](str, fd, server);
			break;
		case 9:
			funct[9](str, fd, server);
			break;
		case 10:
			funct[10](str, fd, server);
			break;
		case 11:
			{
				Client &client = server.getClients(fd);
				std::string error = ERR_ALREADYREGISTRED(CLIENT(client.getNickname(), client.getUsername()));
				if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}
			break;
		case 12:
			{
				Client &client = server.getClients(fd);
				std::string error = ERR_ALREADYREGISTRED(CLIENT(client.getNickname(), client.getUsername()));
				if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}
	}
}
