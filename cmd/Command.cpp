/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:09:50 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/20 15:47:15 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

typedef void (*FunctionPointer)(std::string&, int, Server&);

void    readTheLine(std::string &str, int fd, Server &server)
{
	std::string listcmd[8] = {"JOIN", "PRIVMSG", "INVITE", "KICK", "MODE", "TOPIC", "PART", "QUIT"};
	FunctionPointer funct[8] = {joinChannel, cmdPrivmsg, cmdInvite, cmdKick, cmdMode, cmdTopic, cmdPart, cmdQuit};
	if (str.find("CAP LS 302") == std::string::npos && server.handleConnexion(fd, str) == false)
		return ;

	int i = -1;
	while(++i < 8)
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
	}
}
