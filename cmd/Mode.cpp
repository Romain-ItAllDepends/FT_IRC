/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:45:56 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 11:35:50 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	mode_i(std::string str, int fd, Channel &channel)
{
	std::string message, error;
	std::string name = channel.getClient(fd);

	if (!channel.getOperator(fd))
	{
		error = ERR_CHANOPRIVSNEEDED(name, channel.getName());
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}

	if (str.find("+i") != std::string::npos && !channel.getInvitOnly())
	{
		channel.setInvitOnly();
		message = RPL_MODE(name, channel.getName(), "+i", name);
		if (send(fd, message.c_str(), message.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
	else if (str.find("-i") != std::string::npos && channel.getInvitOnly())
	{
		channel.setInvitOnly();
		message = RPL_MODE(name, channel.getName(), "-i", name);
		channel.channelBroadcast(message);
	}
}

void	mode_topic(std::string str, int fd, Channel &channel)
{
	std::string	error;
	std::string name = channel.getClient(fd);

	if (!channel.getOperator(fd))
	{
		error = ERR_CHANOPRIVSNEEDED(name, channel.getName());
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	if (str.find("+t") != std::string::npos && !channel.getTopicUser())
	{
		channel.setTopicUser();
		std::string message = RPL_MODE(name, channel.getName(), "+t", name);
		channel.channelBroadcast(message);
	}
	else if (str.find("-t") != std::string::npos && channel.getTopicUser())
	{
		channel.setTopicUser();
		std::string message = RPL_MODE(name, channel.getName(), "-t", name);
		channel.channelBroadcast(message);
	}
}

void	mode_k(std::string str, int fd, Channel &channel)
{
	std::string	error;
	std::string name = channel.getClient(fd);

	if (!channel.getOperator(fd))
	{
		error = ERR_CHANOPRIVSNEEDED(name, channel.getName());
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}

	std::istringstream iss(str);
	std::string command, channelName, mode, password;
	iss >> command >> channelName >> mode;

	if (mode == "+k")
	{
		iss >> password;
		if (password.empty())
			return;

		channel.setPassword(password);
		std::string message = RPL_MODE(name, channel.getName(), "+k", name);
		channel.channelBroadcast(message);
	}
	else if (mode == "-k")
	{
		channel.setPassword("");
		std::string message = RPL_MODE(name, channel.getName(), "-k", name);
		channel.channelBroadcast(message);
	}
}

void	mode_o(std::string str, int fd, Channel &channel)
{
	std::string error;
	std::string name = channel.getClient(fd);

	if (!channel.getOperator(fd))
	{
		error = ERR_CHANOPRIVSNEEDED(name, channel.getName());
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}
	std::istringstream ss(str);
	std::string command, channelName, mode, target;	
	ss >> command >> channelName >> mode >> target;
	if (mode == "+o")
	{
		int targetFd = channel.getClient(target);
		if (targetFd == -1)
		{
			Client &client = channel.getClients(name);
			std::string message = ERR_USERNOTINCHANNEL(CLIENT(client.getNickname(), client.getUsername()), channel.getName(), target);
			if (send(fd, message.c_str(), message.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return;
		}
		if (!channel.getOperator(targetFd))
		{
			channel.setOperator(targetFd);
			std::string message = RPL_MODE(name, channel.getName(), "+o", target);
			channel.channelBroadcast(message);
		}
	}
	else if (mode == "-o")
	{
		int targetFd = channel.getClient(target);
		if (targetFd == -1)
			return;
		if (channel.getOperator(targetFd))
		{
			channel.setOperator(targetFd);
			std::string message = RPL_MODE(name, channel.getName(), "-o", target);
			channel.channelBroadcast(message);
		}
	}
}

void	mode_l(std::string str, int fd, Channel &channel)
{
	std::string error;
	std::string name = channel.getClient(fd);

	if (!channel.getOperator(fd))
	{
		error = ERR_CHANOPRIVSNEEDED(name, channel.getName());
		if (send(fd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
		return;
	}

	std::istringstream ss(str);
	std::string command, channelName, mode;
	ss >> command >> channelName >> mode;

	if (mode == "+l")
	{
		size_t userLimit;
		ss >> userLimit;

		if (ss.fail())
			return;
		channel.setUserLimit(userLimit);
		std::string message = RPL_MODE(name, channel.getName(), "+l", name);
		channel.channelBroadcast(message);
	}
	else if (mode == "-l")
	{
		channel.setUserLimit(-1);
		std::string message = RPL_MODE(name, channel.getName(), "-l", name);
		channel.channelBroadcast(message);
	}
}

typedef void (*FunctionPointer)(std::string, int, Channel&);

void	cmdMode(std::string &str, int fd, Server &server)
{
	try
	{
		size_t channelPos = str.find("#");
		if (channelPos == std::string::npos)
			return;
		std::istringstream ss(str.substr(channelPos));
		std::string channelname, modeflag;
		ss >> channelname >> modeflag;
		Channel& channel = server.getOrCreateChannel(channelname);
		if (modeflag.empty() || modeflag == "WHO")
		{
			if (modeflag.empty())
			{
				std::string modes = "+";
				switch(0)
				{
					case 0:
						if (channel.getTopicUser() == true)
							modes += "t";				
					case 1:
						if (channel.getInvitOnly() == true)
							modes += "i";
					case 2:
						if (channel.getUserLimit() > 0)
							modes += "l";
					case 3:
						if (channel.getPassword().size() != 0)
							modes += "k";
					case 4:
						if (modes.find("l") != std::string::npos)
						{
							std::stringstream ss;
							ss << channel.getUserLimit();
							std::string pwd;
							ss >> pwd;
							modes += " " + pwd;
						}
					case 5:
						if (modes.find("k") != std::string::npos)
							modes += " " + channel.getPassword();
				}
				std::string error = RPL_CHANNELMODEIS(channel.getClient(fd), channelname, modes);
				if (send(fd, error.c_str(), error.size(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}
			return;
		}
			
		std::string modes[5] = {"i", "t", "k", "o", "l"};
		FunctionPointer handlers[5] = {mode_i, mode_topic, mode_k, mode_o, mode_l};
		if (modeflag.size() != 2 || (modeflag[0] != '+' && modeflag[0] != '-'))
		{
			std::string msg = ERR_UNKNOWNMODE(channel.getClient(fd), channelname);
			if (send(fd, msg.c_str(), msg.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return;
		}
		char flag = modeflag[1];
		for (int i = 0; i < 5; ++i)
		{
			if (modes[i][0] == flag)
			{
				handlers[i](str, fd, channel);
				return;
			}
		}
		std::string msg = ERR_UNKNOWNMODE(channel.getClient(fd), channelname);
		if (send(fd, msg.c_str(), msg.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
	}
	catch (const std::exception& e){}
}
