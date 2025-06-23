/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:23:16 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/17 10:25:04 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "../Server.hpp"

class Server;

void	cmdInvite(std::string &str, int fd, Server &server);
void	cmdKick(std::string &str, int fd, Server &server);
void	cmdMode(std::string &str, int fd, Server &server);
void	cmdTopic(std::string &str, int fd, Server &server);
void	joinChannel(std::string &str, int fd, Server &server);
void	readTheLine(std::string &, int, Server &);
void	cmdPrivmsg(std::string &str, int fd, Server &server);
void	cmdPart(std::string &str, int fd, Server &server);
void	cmdQuit(std::string &str, int fd, Server &server);