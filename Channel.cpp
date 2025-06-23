/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:13:58 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 11:19:49 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name):_name(name), _topic(""), _topicuser(false), _password("")
, _invitonly(false), _userlimit(-1), _nbUser(0){}

Channel::~Channel() {}

const std::string& Channel::getName() const
{
	return _name;
}

void Channel::addClient(Client &client, const std::string &password)
{
	std::string	error, message;
	if (_invitonly == true && client.getInvited() != getName())
	{
		if (getPassword().empty() == true || getPassword() == password)
		{
			error = ERR_INVITEONLYCHAN(CLIENT(client.getNickname(), client.getUsername()), getName());
			if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
	}
	if (_userlimit != -1 && _nbUser >= _userlimit)
	{
		error = ERR_CHANNELISFULL(CLIENT(client.getNickname(), client.getUsername()), getName());
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return ;
	}
	if ((_userlimit != -1 && _nbUser < _userlimit)
		|| (client.getInvited() == getName() && _userlimit != -1 && _nbUser < _userlimit))
	{
		client.setInvited("none");
		_clients.insert(std::pair<int, Client>(client.getClientSocket(), client));
		_operator.insert(std::pair<int, bool>(client.getClientSocket(), false));
		_nbUser++;
	} else if (_userlimit == -1)
	{
		client.setInvited("none");
		_clients.insert(std::pair<int, Client>(client.getClientSocket(), client));
		_operator.insert(std::pair<int, bool>(client.getClientSocket(), false));
		_nbUser++;
	}

	if (_nbUser == 1 && getOperator(client.getClientSocket()) == false)
	{
		setOperator(client.getClientSocket());
		message = RPL_SEETOPIC(CLIENT(client.getNickname(), client.getUsername()), getName(), "");
		if (send(client.getClientSocket(), message.c_str(), message.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		message = RPL_ONLY(CLIENT(client.getNickname(), client.getUsername()), getName());
		if (send(client.getClientSocket(), message.c_str(), message.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		message = RPL_CHANNELMODEIS(client.getNickname(), getName(), "+");
		if (send(client.getClientSocket(), message.c_str(), message.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
}

void Channel::removeClient(Client &client)
{
	std::map<int, bool>::iterator it_op = _operator.find(client.getClientSocket());
	if (it_op != _operator.end())
		_operator.erase(it_op);
	std::map<int, Client>::iterator it = _clients.find(client.getClientSocket());
	if (it != _clients.end())
		_clients.erase(it);
	_nbUser--;
	if (_nbUser == 1)
	{
		int newOp = _clients.begin()->second.getClientSocket();
		std::map<int, Client>::iterator name = _clients.find(newOp);
		if (getOperator(newOp) == false)
		{
			setOperator(newOp);
			std::string message = RPL_MODE(name->second.getNickname(), _name, "+o", name->second.getNickname());
			if (send(newOp, message.c_str(), message.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
		}
	}
}

bool Channel::hasClient(const Client &client) const
{
	if (_clients.find(client.getClientSocket()) != _clients.end())
		return true;
	return false;
}

std::string Channel::listNicknames()
{
	std::string list;
	std::map<int, Client>::const_iterator it = _clients.begin();
	for (; it != _clients.end() ; it++)
	{
		if (getOperator(it->second.getClientSocket()) == true)
			list += "@" + it->second.getNickname() + " ";
		else
			list += it->second.getNickname() + " ";
	}
	return list;
}

void	Channel::setName(std::string const &name)
{
	_name = name;
}

std::string Channel::getTopic() const
{
	return _topic;
}

void	Channel::setTopic(const std::string& topic)
{
	_topic = topic;	
}

void	Channel::sendAllChannel(const Client &client, const std::string &msg)
{
	std::string message;
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator end = _clients.end();
	for (; it != end ; it++)
	{
		if (client.getClientSocket() != it->second.getClientSocket())
		{
			message = PRIVMSG(CLIENT(client.getNickname(), client.getNickname()), this->getName(), msg);
			if (send(it->second.getClientSocket(), message.c_str(), message.length(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
		}
	}
}

void	Channel::sendAllNewClient(const Client &client, const std::string &channelName, const std::string &listNicknames)
{
	std::string message;
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator end = _clients.end();
	for (; it != end ; it++)
	{
		message = RPL_NAMEREPLY(client.getNickname(), channelName, listNicknames);
		if (send(it->second.getClientSocket(), message.c_str(), message.length(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
}


void	Channel::removeClientFomChannel(int fd, const std::string &str)
{
	std::string	error;
	std::map<int, Client>::iterator it = _clients.find(fd);

	if (it != _clients.end())
	{
		std::string	message = RPL_PART(it->second.getNickname(), str);
		channelBroadcast(message);
		removeClient(it->second);
		PRINTGB "Client n°" N fd N " leave the channel!" END
	}
}

void	Channel::kickClientFomChannel(int fd)
{
	std::string	error;
	std::map<int, Client>::iterator it = _clients.find(fd);

	if (it != _clients.end())
		removeClient(it->second);
	else if (it == _clients.end())
	{
		error = ERR_USERNOTINCHANNEL(it->second.getNickname(), it->second.getNickname(), getName());
		if (send(fd, error.c_str(), error.length(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
}

void	Channel::channelBroadcast(const std::string &msg)
{
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator end = _clients.end();
	for (; it != end ; it++)
		if (send(it->second.getClientSocket(), msg.c_str(), msg.length(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
}

bool	Channel::getTopicUser()
{
	return _topicuser;
}

std::string	Channel::getPassword()
{
	return _password;
}
		
bool	Channel::getInvitOnly()
{
	return _invitonly;
}

int		Channel::getUserLimit()
{
	return _userlimit;
}

bool	Channel::getOperator(int fd)
{
	std::map<int, bool>::iterator	it = _operator.find(fd);
	if(it != _operator.end())
	{
		return it->second;
	}
	return false;
}

void	Channel::setTopicUser()
{
	if(_topicuser == true)
		_topicuser = false;
	else
		_topicuser = true;
}

void	Channel::setPassword(std::string password)
{
	_password = password;
}
void	Channel::setInvitOnly(){
	if(_invitonly == true)
		_invitonly = false;
	else
		_invitonly = true;
}

void	Channel::setUserLimit(size_t limit)
{
	_userlimit = limit;
}

void	Channel::setOperator(int fd)
{
	std::map<int, bool>::iterator	it = _operator.find(fd);
	if(it != _operator.end())
	{
		if(it->second == true)
			it->second = false;
		else
			it->second = true;
	}
}

Client	&Channel::getClients(const std::string &name)
{
	std::map<int, Client>::iterator it = _clients.begin();
	for (; it != _clients.end() ; it++)
	{
		if (it->second.getNickname() == name)
			return it->second;
	}
	sockaddr_in addr;
	static Client fail(-1, addr, "no name", "no nick", "no realname", true);
	return fail;
}

std::string Channel::getClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if(it != _clients.end())
		return it->second.getNickname();
	return "";
}

int 	Channel::getClient(std::string name)
{
	std::map<int, Client>::iterator it = _clients.begin();
	while(it != _clients.end())
	{
		if(it->second.getNickname() == name)
			return it->first;
		++it;
	}
	return -1;
}