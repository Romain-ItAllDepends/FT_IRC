/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:10:30 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/27 20:52:01 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string const &name, const char *port, std::string const &password)
{
	std::istringstream ss(port);
	int result;
	ss >> result;
	if (ss.fail())
		throw std::runtime_error("Error: Invalid integer format! (Port: 1024 to 65 535)");
	if (result < 0 || result > 65535)
		throw std::runtime_error("Error: Invalid integer format! (Port: 1024 to 65 535)");
	_name = name;
	_port = result;
	if (password.size() < 1 || password.size() > 10)
		throw std::runtime_error("Error: Password size limit!");
	for(size_t i = 0; password[i]; ++i)
		if(isspace(password[i]))
			throw std::runtime_error("Error: Password contains non-printable characters.");

	_password = password;
	std::memset(_buffer, 0, 514);
	if (_port < 1024)
		throw std::runtime_error("Error: We do not allow port under 1024 because it needs root access!");
	memset(_buffer, 0, 514);
	_fds.clear();
}

Server::Server(Server const &server)
{
	*this = server;
}

Server::~Server()
{
	closeFileDescriptor();
	PRINTGB "The server has been closed!" END
}

void	Server::startServer()
{
	PRINTGB "The server started!" END

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		throw std::runtime_error("Error: The socket did not initialize properly!");
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK))
		throw std::runtime_error("Error: fcntl failed on client socket!");
	// This allows calls like accept(), read(), or write() to not block program execution if they cannot execute immediately.
	// This is essential for using poll()/select() efficiently and responsively in a threadless server.
	_serverAddress.sin_family = AF_INET; // AF_INET IPv4
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // htonl 32 bits / INADDR_ANY accept all (0.0.0.0)
	_serverAddress.sin_port = htons(_port); // htons 16 bits transform the address as "network byte order"
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Error: setsockopt failed!");

	if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0)
	{
		close(_serverSocket);
		throw std::runtime_error("Error: The bind between the socket and the server IP address is failing!");
	}
	if (listen(_serverSocket, 10) == -1)
		throw std::runtime_error("Error: Listen failure on the server!");
	newConnection();
}

void Server::newConnection()
{
	pollfd serverPollFd;
	serverPollFd.fd = _serverSocket;
	serverPollFd.events = POLLIN;
	serverPollFd.revents = 0;
	_fds.push_back(serverPollFd);

	std::vector<int> fdsToHandle;
	std::vector<int> fdsToRemove;


	while (true)
	{
		fdsToHandle.clear();
		fdsToRemove.clear();

		int ret = poll(_fds.data(), _fds.size(), -1);
		if (ret == -1 || ret == 0)
			throw std::runtime_error("Error: poll failure!");

		for (size_t i = 0; i < _fds.size(); ++i)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverSocket)
					newClient();
				else
					fdsToHandle.push_back(i);
			}

			if (_fds[i].revents & (POLLHUP | POLLERR))
				fdsToRemove.push_back(_fds[i].fd);
		}
		for (size_t i = 0; i < fdsToHandle.size(); ++i)
			handleRequest(fdsToHandle[i]);
		for (size_t i = 0; i < fdsToRemove.size(); ++i)
			handleQuit(fdsToRemove[i]);
	}
}

void	Server::newClient()
{
	sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	std::cout << "Waiting for a new connection..." << std::endl;
	int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &len);
	if (clientSocket < 0)
		return;
	Client newClient(clientSocket, "", "", "");
	addClient(clientSocket, newClient);
	pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	_fds.push_back(clientPollFd);
}

void Server::addPass(std::string str, Client &client)
{
	size_t pos = str.find("PASS");
	if (pos == std::string::npos)
		return ;
	pos += 4;
	while (pos < str.length() && std::isspace(str[pos]))
		++pos;
	size_t end = pos;
	while (end < str.length() && !std::isspace(str[end]))
		++end;
	std::string password = str.substr(pos, end - pos);
	if(password.size() == 0)
	{
		std::string empty = "";
		std::string cmd = "PASS";
		std::string error = ERR_NEEDMOREPARAMS(CLIENT(empty, empty), cmd);
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	if (password == _password && !client.getPassServ())
		client.setPassServ(true);
	else if (password != _password && client.getPassServ()){
		client.setPassServ(false);
	}
}

void Server::addNick(std::string str, Client &client)
{
	std::string	error, tmp, nick, rest;
	if(client.getPassServ() == false)
		return ;
	size_t pos = str.find("NICK");
	if (pos == std::string::npos)
		return;
	pos += 4;
	while (pos < str.length() && std::isspace(str[pos]))
		pos++;
	if (pos >= str.length())
	{
		error = ERR_NONICKNAMEGIVEN(std::string("Unregistred"));
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
	size_t end = pos;
	while (end < str.length() && !std::isspace(str[end]))
		end++;
	nick = str.substr(pos, end - pos);
	if (nick.empty() || nick[0] == '#')
		return;
	rest = str.substr(end);
	bool onlyWhitespace = true;
	for (size_t i = 0; i < rest.length(); ++i)
	{
		if (!std::isspace(rest[i]))
		{
			onlyWhitespace = false;
			break;
		}
	}
	if (getClients(nick).getClientSocket() != -1 || getClients(nick).getClientSocket() == client.getClientSocket())
	{
		error = ERR_NICKNAMEINUSEC(nick);
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	if (onlyWhitespace)
		client.setNickname(nick);
}

void Server::addUser(std::string str, Client &client)
{
	std::string username, mode, unused, realname, error;

	if(client.getPassServ() == false)
		return ;
	size_t pos = str.find("USER");
	if (pos == std::string::npos)
		return;
	pos += 4;
	while (pos < str.length() && std::isspace(str[pos]))
		pos++;
	if (pos >= str.length())
	{
		error = ERR_NEEDMOREPARAMS(std::string("Unregistred"), "USER");
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	std::string args = str.substr(pos);
	std::istringstream ss(args);
	ss >> username >> mode >> unused;
	std::getline(ss, realname);
	if (username.empty() || mode.empty() || unused.empty() || realname.empty())
	{
		error = ERR_NEEDMOREPARAMS(std::string("Unregistred"), "USER");
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	size_t start = 0;
	while (start < realname.size() && std::isspace(realname[start]))
		++start;
	realname = realname.substr(start);
	if (!realname.empty() && realname[0] == ':')
	{
		realname = realname.substr(1);
		if (realname.empty())
		{
			error = ERR_NEEDMOREPARAMS(std::string("Unregistred"), "USER");
			if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return;
		}
		start = 0;
		while (start < realname.size() && std::isspace(realname[start]))
			++start;
		realname = realname.substr(start);
	}
	while (!realname.empty() && !isprint(realname[realname.size() - 1]))
		realname.erase(realname.size() - 1);
	if (username[0] == '#')
		return;
	if (!client.getUsername().empty())
		return;
	client.setUsername(username);
	client.setRealname(realname);
}

bool	Server::handleConnexion(int fd, const std::string &str)
{
	Client &client = getClients(fd);
	if(client.getRealname().size() != 0 && client.getNickname().size() != 0)
		return true;

	size_t pos = 0;
	while (pos < str.size())
	{
		size_t end = str.find("\r\n", pos);
		if (end == std::string::npos)
			end = str.find('\n', pos);
		if (end == std::string::npos)
			end = str.size();
		std::string line = str.substr(pos, end - pos);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		pos = end + ((end < str.size() && str[end] == '\r' && end + 1 < str.size() && str[end + 1] == '\n') ? 2 : 1);
		if (line.find("PASS") == 0)
			addPass(line, client);
		else if (line.find("NICK") == 0)
			addNick(line, client);
		else if (line.find("USER") == 0)
			addUser(line, client);
	}
	if(client.getRealname().size() != 0 && client.getNickname().size() != 0)
	{
		std::string name = client.getNickname();
		std::string tmp = WELCOME(name);
		tmp += HOST(name);
		tmp += DATE(name);
		tmp += MYINFO(name);	
		tmp	+= SERVCAP(name);
		tmp += MOTDST(name);
		tmp += MOTDED(name);
		if (send(fd, tmp.c_str(), tmp.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
	return false;
}

void	Server::handleRequest(const int it)
{
	std::string	fullCommand;

	if (getClients(_fds[it].fd).getClientSocket() == -1)
		throw std::runtime_error("Error: The client doesn't exist!");
	if (static_cast<size_t>(it) >= _fds.size())
		return ;
	int lenRead = read(_fds[it].fd, _buffer, 514);
	if (lenRead < 0)
		return;
	if (lenRead == 0)
	{
		handleQuit(_fds[it].fd);
		return ;
	}

	_buffer[lenRead] = '\0';
	getClients(_fds[it].fd).appendToBuffer(_buffer);
	while ((fullCommand = getClients(_fds[it].fd).manageRequest()).empty() == false) 
		readTheLine(fullCommand, _fds[it].fd, *this);
	resetBuffer();
}

void	Server::closeFileDescriptor()
{
	for (size_t i = 0 ; i < _fds.size(); i++)
		close(_fds[i].fd);
}

void	Server::resetBuffer()
{
	std::memset(_buffer, 0, 514);
}

Channel &Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel>::iterator it = _channels.begin();
	while(it != _channels.end())
	{
		if(it->first == name)
			return it->second;
		++it;
	}
	static Channel fail("no name");
	return fail;
}

Channel &Server::getOrCreateChannel(const std::string& name)
{
	if (_channels.find(name) == _channels.end())
		_channels[name] = Channel(name);
	return _channels[name];
}

void Server::removeClientFromAllChannels(Client &client)
{
	std::map<std::string, Channel>::iterator it = _channels.begin();
	for (; it != _channels.end(); ++it)
		it->second.removeClient(client);
}

void Server::handleJoinCommand(Client &client, const std::string &channelName, const std::string &password)
{
	std::string	message, error;
	Channel& channel = getOrCreateChannel(channelName);

	if (channel.getPassword().empty() == false && password != channel.getPassword())
	{
		error = ERR_BADCHANNELKEY(CLIENT(client.getNickname(), client.getUsername()), channelName);
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return ;
	}
	if (channel.getPassword().empty() == false && password.empty() == true)
	{
		error = ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "JOIN");
		if (send(client.getClientSocket(), error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return ;
	}

	channel.addClient(client, password);
	if (channel.getClient(client.getClientSocket()) == client.getNickname())
	{
		message = RPL_JOIN(CLIENT(client.getNickname(), client.getUsername()), channelName);
		if (send(client.getClientSocket(), message.c_str(), message.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		message = JOIN(client.getNickname(), channelName);
		channel.sendAllChannel(message, client.getClientSocket());
		message = RPL_NAMEREPLY(client.getNickname(), channelName, channel.listNicknames());
		if (send(client.getClientSocket(), message.c_str(), message.length(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		message = RPL_ONLY(CLIENT(client.getNickname(), client.getUsername()), channelName);
		if (send(client.getClientSocket(), message.c_str(), message.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
	resetBuffer();
}

void	Server::handlePrivmsgCommand(const Client &client, const std::string &target, const std::string &msg)
{
	int			targetFd;
	std::string	request;
	std::map<std::string, Channel>::iterator	it = _channels.find(target);

	if (it != _channels.end() && target[0] == '#')
		it->second.sendAllChannel(client, msg);
	else
	{
		targetFd = getClients(target).getClientSocket();
		if (targetFd == -1)
			throw std::runtime_error("Error: No such User!");
		request = PRIVMSG(CLIENT(client.getNickname(), client.getUsername()), target, msg);
		if (send(targetFd, request.c_str(), request.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
}

void	Server::handleQuit(int fd)
{
	if (getClients(fd).getClientSocket() != -1)
	{
		std::vector<pollfd>::iterator it = _fds.begin();
			
		if(getClients(fd).getNickname().size() != 0)
		{
			std::map<std::string, Channel>::iterator it_ch = _channels.begin();
			while(it_ch != _channels.end())
			{
				std::string name = it_ch->second.getClient(fd);
				if(name.size() > 0)
					handleRemoveClientFomChannel(fd, it_ch->first, it_ch->first + " crash");
				it_ch++;
			}
		}

		removeClient(fd);
		for (; it != _fds.end(); ++it)
		{
			if (it->fd == fd)
			{
				_fds.erase(it);
				break;
			}
		}
		close(fd);
	}
	PRINTGB "Client n°" N fd N " leave the server!" END
}

void	Server::handleKick(int fd, const std::string &msg, const std::string &channelName, int target, const std::string &sender)
{
	std::string error;
	Client &client = getClients(sender);
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it != _channels.end() && it->second.getOperator(fd) == false)
	{
		error = ERR_CHANOPRIVSNEEDED(CLIENT(client.getNickname(), client.getUsername()), channelName);
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	} else if (it == _channels.end())
	{
		error = ERR_NOSUCHCHANNEL(channelName);
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	} else if (it->second.getClient(target).empty() == true)
	{
		error = ERR_NOTONCHANNEL(CLIENT(client.getNickname(), client.getUsername()), channelName);
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	} else if (it->second.getOperator(fd) == true)
	{
		it->second.kickClientFomChannel(target);
		it->second.channelBroadcast(msg);
		if (send(target, msg.c_str(), msg.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
}

void	Server::handleRemoveClientFomChannel(int fd, const std::string &channelName, const std::string &str)
{
	std::string error;
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);

	if (it != _channels.end())
		it->second.removeClientFomChannel(fd, str);
	else if (it == _channels.end())
	{
		error = ERR_NOSUCHCHANNEL(channelName);
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
}

bool	Server::inviteVerification(const Client &sender, const Client &target, const std::string& channelName)
{
	std::string error;
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);

	if (it != _channels.end())
	{
		if (it->second.getClient(sender.getClientSocket()) != sender.getNickname())
		{
			error = ERR_NOTONCHANNEL(CLIENT(sender.getNickname(), sender.getUsername()), channelName);
			if (send(sender.getClientSocket(), error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return false;
		}
		if (it->second.getClient(target.getClientSocket()) == target.getNickname())
		{
			error = ERR_USERONCHANNEL(target.getNickname(), channelName);
			if (send(sender.getClientSocket(), error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return false;
		}
		if (it->second.getInvitOnly() == true && it->second.getOperator(sender.getClientSocket()) == false)
		{
			error = ERR_CHANOPRIVSNEEDED(CLIENT(sender.getNickname(), sender.getUsername()), channelName);
			if (send(sender.getClientSocket(), error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return false;
		}
	}
	return true;
}

Client &Server::getClients(const int fd)
{
	std::map<int, Client>::iterator it = _clientsMap.find(fd);
	if(it == _clientsMap.end())
	{
		static Client fail(-1, "no name", "no nick", "no realname");
		return fail;
	}
	return it->second;
}

Client	&Server::getClients(const std::string& name)
{
	std::map<int, Client>::iterator it = _clientsMap.begin();
	for (; it != _clientsMap.end() ; it++)
	{
		if (it->second.getNickname() == name)
			return it->second;
	}
	static Client fail(-1, "no name", "no nick", "no realname");
	return fail;
}

void	Server::addClient(int const &fd, Client const &newClient)
{
	_clientsMap.insert(std::pair<int, Client>(fd, newClient));
}

void	Server::removeClient(int fd)
{
	_clientsMap.erase(fd);
}

std::string Server::listNicknames()
{
	std::string list;
	std::map<int, Client>::const_iterator it = _clientsMap.begin();
	for (; it != _clientsMap.end() ; it++)
		list += it->second.getNickname() + " ";
	return list;
}

bool isAllWhitespace(const std::string& s) {
	for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
	{
		if (!std::isspace(static_cast<unsigned char>(*it)))
			return false;
	}
	return true;
}
