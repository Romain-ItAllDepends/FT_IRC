/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_bot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:29:29 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 11:44:08 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sstream>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 512

int	parsingPort(const std::string &port)
{
	std::istringstream ss(port);
	unsigned short int	result;
	ss >> result;
	if (ss.fail())
		return -1;
	return result;
}

void sigint(int pid)
{
	(void) pid;
	throw std::runtime_error("Error bot: Ctrl+C signal!");
}

int main(int ac, char **av)
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("Socket");
		return 1;
	}

	try
	{
		if (ac != 4)
		{
			std::cerr << "\033[0;31m" << "\033[1m" << "Bot error: Wrong number of arguments!" << "\033[0m" << std::endl;
			return 1;
		}
		signal(SIGINT, sigint);
		const int port = parsingPort(av[2]);
		const std::string nick = "MEE2";
		const std::string user = "Bot 0 * :Bot";
		const std::string channel = "#general";


		struct sockaddr_in serv_addr;
		struct hostent *host;

		if (port == -1)
		{
			std::cerr << "\033[0;31m" << "\033[1m" << "Bot error: Wrong port!" << "\033[0m" << std::endl;
			return 1;
		}

		host = gethostbyname(av[1]);
		if (!host)
		{
			std::cerr << "Error bot: Cannot resolve hostname." << std::endl;
			return 1;
		}

		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);
		memcpy(&serv_addr.sin_addr, host->h_addr, host->h_length);
		std::cout << sockfd << std::endl;
		if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		{
			perror("Connect");
			return 1;
		}

		const std::string password = av[3];

		std::string out = "PASS ";
		out += password;
		out += "\r\nNICK ";
		out += nick;
		out += "\r\nUSER ";
		out += user;
		out += "\r\n";

		if (send(sockfd, out.c_str(), out.length(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");

		char	buffer[BUFFER_SIZE];
		int	nbMsg = 0;
		while (true)
		{
			memset(buffer, 0, sizeof(buffer));
			int bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
			if (bytes <= 0)
			{
				std::cerr << "Disconnected or error." << std::endl;
				break;
			}

			std::string msg(buffer);
			std::cout << msg;

			if (msg.find("PRIVMSG") != std::string::npos)
				nbMsg++;

			if (msg.find(" 001 ") != std::string::npos || msg.find("KICK #general MEE2") != std::string::npos)
			{
				std::string joinCmd = "JOIN ";
				joinCmd += channel;
				joinCmd += "\r\n";
				if (send(sockfd, joinCmd.c_str(), joinCmd.length(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}

			if (msg.find("PRIVMSG") != std::string::npos && msg.find("!hello") != std::string::npos)
			{
				std::string response = "PRIVMSG ";
				response += channel;
				response += " :Hello, I am MEE2 Bot, I was created to destroy the human kind.\r\n";
				if (send(sockfd, response.c_str(), response.length(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}

			if (msg.find("PRIVMSG") != std::string::npos && msg.find("!nbmsg") != std::string::npos)
			{
				std::string response = "PRIVMSG ";
				std::stringstream ss;
				ss << nbMsg;
				std::string nbMessage;
				ss >> nbMessage;
				response += channel;
				response += " :general channel count: ";
				response += std::string(nbMessage);
				response += "\r\n";
				if (send(sockfd, response.c_str(), response.length(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}

			if (msg.find("PRIVMSG") != std::string::npos && msg.find("!version") != std::string::npos)
			{
				std::string response = "PRIVMSG ";
				response += channel;
				response += " :MEE2 is currently in v1.0.0!\r\n";
				if (send(sockfd, response.c_str(), response.length(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}
		}

		close(sockfd);
	}
	catch (std::exception &e)
	{
		std::cerr << "\033[0;31m" << "\033[1m" << e.what() << "\033[0m" << std::endl;
		close(sockfd);
	}
    return 0;
}