/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:01:41 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/27 14:44:40 by rgobet           ###   ########.fr       */
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

#include <map>
#include <iostream>
#include "Request.hpp"
#include <netinet/in.h>

class Client
{
private:
	int			_clientSocket;
	std::string	_nickname;
	std::string	_username;
	std::string _realname;
	bool		_passwordserver;
	std::string	_invited;
	std::map<int, std::string>	_partialInput;
public:
	Client(int clientSocket, std::string const &nickname, std::string const &username, std::string const &realname);
	~Client();

	int			getClientSocket() const;
	std::string getNickname() const;
	std::string getUsername() const;
	std::string getRealname() const;
	std::string getInvited() const;
	bool		getPassServ() const;

	void		setInvited(const std::string &channelName);

	void		clearPartialInput();

	std::string	manageRequest();
	void		appendToBuffer(const std::string &buffer);
  
	void		setNickname(const std::string &);
	void		setUsername(const std::string &);
	void		setRealname(const std::string &);
	void		setPassServ(bool password);
};

std::ostream& operator<<(std::ostream& os, const Client& client);
