/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 09:57:03 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/20 11:00:30 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmdQuit(std::string &str, int fd, Server &server)
{
	(void)str;

	server.handleQuit(fd);
}