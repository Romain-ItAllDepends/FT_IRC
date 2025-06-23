/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 13:15:30 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 11:28:01 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define RED "\033[0;31m"
#define NC "\033[0m" // No Color
#define BOLD "\033[1m"

#define PRINT std::cout <<
#define PRINTR std::cout << RED <<
#define PRINTG std::cout << GREEN <<
#define PRINTB std::cout << BLUE <<
#define PRINTRB std::cerr << RED << BOLD <<
#define PRINTGB std::cout << GREEN << BOLD <<
#define PRINTBB std::cout << BLUE << BOLD <<
#define N <<
#define END << NC << std::endl;

#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <poll.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <cctype>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "cmd/Command.hpp"

class Clients;
class Channel;

class Server
{
private:
	std::string						_name;
	unsigned short int				_port;
	std::string						_password;
	sockaddr_in						_serverAddress;
	int								_serverSocket;
	std::map<int, Client>			_clientsMap;
	char							_buffer[514];
	std::vector<pollfd>				_fds;
	std::map<std::string, Channel> _channels;
private:
	Server();
public:
	Server(std::string const &name, const char *port, std::string const &password);
	Server(Server const &server);
	~Server();

	void	startServer();
	void	newConnection();
	void	newClient();
	void	handleRequest(const int it);
	void	resetBuffer();
	void	closeFileDescriptor();
	Client	&getClients(int fd);
	Channel	&getOrCreateChannel(const std::string& name);
	Channel &getChannel(const std::string&);
	Client	&getClients(const std::string& name);
	void	handleKick(int fd, const std::string &msg, const std::string &channelName, int target, const std::string &sender);
	void	handleQuit(int fd);
	void 	removeClientFromAllChannels(Client &client);
	void	addClient(int const &fd, Client const &newClient);
	void	removeClient(int fd);
	bool	inviteVerification(const Client &sender, const Client &target, const std::string channelName);
	void 	handleJoinCommand(Client &client, const std::string &channelName, const std::string &password);
	void	handlePrivmsgCommand(const Client &client, const std::string &target, const std::string &msg);
	void	handleRemoveClientFomChannel(int fd, const std::string &channelName, const std::string &str);
	bool	handleConnexion(int fd, const std::string &str);
	void	addPass(std::string str, Client &client);
	void	addNick(std::string str, Client &client);
	void	addUser(std::string str, Client &client);
	bool	sameUsername(std::string name);
};

bool isAllWhitespace(const std::string& s);