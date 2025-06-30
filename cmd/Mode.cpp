/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:45:56 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/27 09:11:33 by huvillat         ###   ########.fr       */
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
	Client &client = channel.getClients(name);
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
		{
			std::string error =	ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "MODE +k");
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return;
		}
		if(password == channel.getPassword())
		{
			std::string error =	ERR_KEYSET(client.getNickname(), channelName);
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return;
		}
		channel.setPassword(password);
		std::string message = RPL_MODE(name, channel.getName(), "+k", password);
		channel.channelBroadcast(message);
	}
	else if (mode == "-k")
	{
		channel.setPassword("");
		std::string message = RPL_MODE(name, channel.getName(), "-k", "");
		channel.channelBroadcast(message);
	}
}

void	mode_o(std::string str, int fd, Channel &channel)
{
	std::string error;
	std::string name = channel.getClient(fd);
	Client &client = channel.getClients(name);
	if (!channel.getOperator(fd))
	{
		error = ERR_CHANOPRIVSNEEDED(CLIENT(client.getNickname(), client.getUsername()), channel.getName());
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
			std::string error = ERR_USERNOTINCHANNEL(CLIENT(client.getNickname(), client.getUsername()), channel.getName(), target);
			if (send(fd, error.c_str(), error.size(), 0) == -1)
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
		{
			std::string error = ERR_USERNOTINCHANNEL(CLIENT(client.getNickname(), client.getUsername()), channel.getName(), target);
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return;
		}
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
	Client &client = channel.getClients(name);
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
		int userLimit;
		ss >> userLimit;
		if(userLimit < 1 || userLimit > 100)
			return;
		if (ss.fail())
		{
			std::string error =	ERR_NEEDMOREPARAMS(CLIENT(client.getNickname(), client.getUsername()), "MODE +l");
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
		channel.setUserLimit(userLimit);
		std::stringstream iss;
		iss << userLimit;
		iss >> mode;
		std::string message = RPL_MODE(name, channel.getName(), "+l", mode);
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
		size_t channelPos = str.find("#");
		if (channelPos == std::string::npos)
			return;
		std::istringstream ss(str.substr(channelPos));
		std::string channelName, modeFlag;
		ss >> channelName >> modeFlag;
		Channel &channel = server.getChannel(channelName);
		if(channel.getName() == "no name")
		{
			std::string error = ERR_NOSUCHCHANNEL(channelName);
			if (send(fd, error.c_str(), error.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
		if (modeFlag.empty() || modeFlag == "WHO")
		{
			if (modeFlag.empty())
			{
				std::string modes = "+";
				if (channel.getTopicUser() == true)
					modes += "t";
				if (channel.getInvitOnly() == true)
					modes += "i";
				if (channel.getUserLimit() > 0)
					modes += "l";
				if (channel.getPassword().size() != 0)
					modes += "k";
				if (modes.find("l") != std::string::npos)
				{
					std::stringstream ss;
					ss << channel.getUserLimit();
					std::string pwd;
					ss >> pwd;
					modes += " " + pwd;
				}
				if (modes.find("k") != std::string::npos && channel.getOperator(fd) == true)
					modes += " " + channel.getPassword();
				else if(modes.find("k") != std::string::npos)
					modes += " *";
				std::string error = RPL_CHANNELMODEIS(channel.getClient(fd), channelName, modes);
				if (send(fd, error.c_str(), error.size(), 0) == -1)
					throw std::runtime_error("Error: An error occured while sending the message!");
			}
			return;
		}
		std::string modes[5] = {"i", "t", "k", "o", "l"};
		FunctionPointer handlers[5] = {mode_i, mode_topic, mode_k, mode_o, mode_l};
		if (modeFlag.size() != 2 || (modeFlag[0] != '+' && modeFlag[0] != '-'))
		{
			std::string msg = ERR_UNKNOWNMODE(channel.getClient(fd), channelName);
			if (send(fd, msg.c_str(), msg.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return;
		}
		char flag = modeFlag[1];
		for (int i = 0; i < 5; ++i)
		{
			if (modes[i][0] == flag)
			{
				handlers[i](str, fd, channel);
				return;
			}
		}
		std::string msg = ERR_UNKNOWNMODE(channel.getClient(fd), channelName);
		if (send(fd, msg.c_str(), msg.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");
}
