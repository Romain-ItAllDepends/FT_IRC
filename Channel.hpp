/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:13:58 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/25 14:52:46 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "Request.hpp"
#include "Client.hpp"

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		bool				_topicuser;
		std::string			_password;
		bool				_invitonly;
		int					_userlimit;
		int					_nbUser;
		std::map<int, bool> _operator;
		std::map<int, Client> _clients;
	public:
		Channel();
		Channel(const std::string& name);
		~Channel();

		const std::string	&getName() const;
		void				setName(std::string const &name);

		void	addClient(Client &client, const std::string &password);
		void	removeClient(Client &client);
		bool	hasClient(const Client &client) const;
		Client&	getClients(const std::string &name);
		std::string getClient(int fd);
		int 	getClient(std::string name);
		std::string getTopic() const;
		bool	getTopicUser();
		std::string getPassword();
		bool	getInvitOnly();
		int		getUserLimit();
		bool	getOperator(int);
		void	setTopic(const std::string&);
		void	setTopicUser();
		void	setPassword(std::string);
		void	setInvitOnly();
		void	setUserLimit(size_t);
		void	setOperator(int);
		std::string listNicknames();
		std::string listUsername();
		int			nbUser();

		void	channelBroadcast(const std::string &msg);
		void	sendAllChannel(const std::string &msg, int fd);
		void	sendAllChannel(const Client &client, const std::string &msg);
		void	removeClientFomChannel(int fd, const std::string &str);
		void	kickClientFomChannel(int fd);
};
