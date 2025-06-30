/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:01:41 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/27 14:44:21 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

#include <unistd.h>

Client::Client(int clientSocket, std::string const &nickname, std::string const &username, std::string const &realname):
	_clientSocket(clientSocket), _nickname(nickname), _username(username), _realname(realname)
	, _passwordserver(false), _invited("none")
{
	if (_clientSocket == -1)
		return ;
	PRINTGB "New client n°" N clientSocket N " is connected!" END
	if (_clientSocket > 1024 || _clientSocket < 0)
		throw std::runtime_error("Error: A client got a wrong socket!");
}

Client::~Client()
{
	_partialInput.clear();
}

int	Client::getClientSocket() const
{
	return _clientSocket;
}

std::string Client::getNickname() const
{
	return _nickname;
}

std::string Client::getUsername() const
{
	return _username;
}

std::string Client::getRealname() const
{
	return _realname;
}

std::string Client::getInvited() const
{
	return _invited;
}

bool		Client::getPassServ() const
{
	return _passwordserver;
}

void	Client::setInvited(const std::string &channelName)
{
	_invited = channelName;
}

void	Client::appendToBuffer(const std::string &buffer)
{
	_partialInput[_clientSocket] += buffer;
}

std::string	Client::manageRequest()
{
	size_t pos = _partialInput[_clientSocket].find("\r\n");
	if (pos != std::string::npos)
	{
		std::string fullCommand = _partialInput[_clientSocket].substr(0, pos + 2);
		_partialInput[_clientSocket].erase(0, pos + 2);
		return fullCommand;
	}
	return "";
}

void		Client::setNickname(const std::string &nick)
{
	_nickname = nick;
}

void		Client::setUsername(const std::string &user)
{
	_username = user;
}

void		Client::setRealname(const std::string &real)
{
	_realname = real;
}

void		Client::setPassServ(const bool password)
{
	_passwordserver = password;
}

std::ostream& operator<<(std::ostream& os, const Client& client)
{
	os << client.getClientSocket() << client.getNickname() << std::endl;
	return os;
}