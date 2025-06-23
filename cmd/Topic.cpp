/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:32:29 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/23 11:19:32 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdTopic(std::string &str, int fd, Server &server)
{
	std::istringstream ss(str);
	std::string cmd, channelName, newTopic;
	ss >> cmd >> channelName;
	Channel &channel = server.getOrCreateChannel(channelName);
	std::string topic = channel.getTopic();
	Client &client = server.getClients(fd);
		size_t colonPos = str.find(" :");
	if (colonPos == std::string::npos)
	{
		if(topic.size() == 0){
			str = RPL_NOTOPIC(CLIENT(client.getNickname(), client.getUsername()), channelName);
			if (send(client.getClientSocket(), str.c_str(), str.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");
			return ;
		}
		str = RPL_SEETOPIC(CLIENT(client.getNickname(), client.getUsername()), channelName, topic);
		if (send(client.getClientSocket(), str.c_str(), str.size(), 0) == -1)
			throw std::runtime_error("Error: An error occured while sending the message!");;
		return ;
	}
	
	if(channel.getTopicUser() == true && channel.getOperator(fd) == false)
	{
		str = ERR_CHANOPRIVSNEEDED(CLIENT(client.getNickname(), client.getUsername()), channelName);
		if (send(client.getClientSocket(), str.c_str(), str.size(), 0) == -1)
				throw std::runtime_error("Error: An error occured while sending the message!");;
		return ;
	}
	newTopic = &str[colonPos + 2];
	if(newTopic.size() == 0 || isAllWhitespace(newTopic) == true)
	{
		channel.setTopic("");
		str = RPL_TOPIC(CLIENT(client.getNickname(), client.getUsername()), channelName, "");
		channel.channelBroadcast(str);
		return ;
	}
	channel.setTopic(newTopic);
	str = RPL_TOPIC(CLIENT(client.getNickname(), client.getUsername()), channelName, newTopic);
	channel.channelBroadcast(str);
}
