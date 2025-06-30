/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huvillat <huvillat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:08:28 by rgobet            #+#    #+#             */
/*   Updated: 2025/06/27 20:52:11 by huvillat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define CLIENT(nick, user)(nick + "!" + user + "@localhost")
#define PRIVMSG(client, target, message)(":" + client + " PRIVMSG " + target + " :" + message + "\r\n")
#define NOTICE(client, target, message)(":" + client + " NOTICE " + target + " :" + message + "\r\n")

#define WELCOME(nickname)(":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network, " + nickname + "\r\n")
#define HOST(name) std::string(":127.0.0.1 002 " + name + " :Your host is 127.0.0.1, running version ft_irc-1.0\r\n")
#define DATE(name) std::string(":127.0.0.1 003 " + name + " :This server was created Mon June 2 2025 at 08:00 UTC\r\n")
#define MYINFO(name) std::string(":127.0.0.1 004 " + name + " 127.0.0.1 ft_irc-1.0 -\r\n")
#define SERVCAP(name) std::string(":127.0.0.1 005 " + name + " :CHANTYPES=# PREFIX=(o)@ MODES=1 TARGMAX=JOIN:1,PRIVMSG:1,KICK:1\r\n")
#define MOTDST(name) std::string(":127.0.0.1 375 " + name + " :- Message of the day -\n" + "Today we learn why people talk.\r\n")
#define MOTDED(name) std::string(":127.0.0.1 376 " + name + " :End of /MOTD command\r\n")

#define PONG(token) std::string(":127.0.0.1 PONG 127.0.0.1 :" + token + "\r\n")

#define RPL_ONLY(client, channel)(":127.0.0.1 366 " + client + " " + channel + " :End of /NAMES list\r\n")

#define JOIN(name, channel) std::string(":" + name + " JOIN :" + channel + "\r\n")
#define WHO(name, tname, tnick, channel, Op) std::string(":127.0.0.1 352 " + name + " " + channel + " " + tname + " ircserv 127.0.0.1 " + tnick + " H" + Op + ":0 realname\r\n")
#define ENDWHO(name, channel) std::string(":127.0.0.1 315 " + name + " " + channel + " :End of /WHO list.\r\n")
#define RPL_JOIN(client, channel)(":" + client + " JOIN " + channel + "\r\n")
#define RPL_PART(client, channel)(":" + client + " PART " + channel + "\r\n")
#define RPL_MODE(client, channel, mode, name)(":" + client + " MODE " + channel + " " + mode + " " + name + "\r\n")
#define RPL_KICK(client, channel, target)(":" + client + " KICK " + channel + " " + target + "\r\n")
#define RPL_NEWKICK(client, channel, target, comment)(":" + client + " KICK " + channel + " " + target + " :" + comment + "\r\n")
#define RPL_INVITERCVR(client, invitee, channel)(":" + client + " INVITE " + invitee + " " + channel + "\r\n")
#define RPL_NICK(client, newNick)(":" + client + " NICK " + newNick + "\r\n")
#define RPL_TOPIC(client, channel, topic)(":" + client + " TOPIC " + channel + " :" + topic + "\r\n")
#define RPL_NOTOPIC(client, channel)(":127.0.0.1 331 " + client + " " + channel + " :No topic is set\r\n")
#define RPL_SEETOPIC(client, channel, topic)(":127.0.0.1 332 " + client + " " + channel + " :" + topic + "\r\n")
#define RPL_INVITESNDR(client, invitee, channel)(": 341 " + client + " " + invitee + " " + channel + "\r\n")
#define RPL_NAMEREPLY(nick, channel, nicknames)(":127.0.0.1 353 " + nick + " = " + channel + " :" + nicknames + "\r\n")
#define ENDLISTCLIENT(name, channel) std::string(":127.0.0.1 366 " + name + " " + channel + " :End of /NAMES list.\r\n")

#define ERR_TOOMUCHPARAMS(client, cmd)(client + " " + cmd + " :Too much parameters\r\n")
#define ERR_USERONCHANNEL(nick, channel)(": 303 " + nick + " " + channel + " :is already on channel\r\n")
#define ERR_NOSUCHNICK(client, nickname)(": 401 " + client + " " + nickname + " :No such nickname\r\n")
#define ERR_NOSUCHCHANNEL(channel)(": 403 " + channel + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel)(": 404 " + client + " " + channel + " :Cannot send to channel\r\n")
#define ERR_NOTEXTTOSEND(client)(": 412 " + client + " :No text to send\r\n")

#define ERR_NONICKNAMEGIVEN(client)(":127.0.0.1 431 " + client + " :No nickname given\r\n")

#define ERR_ERRONEUSNICKNAME(client, nickname)(": 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(client, nick)(":127.0.0.1 433 " + client + " " + nick + " :Nickname is already in use\r\n")
#define ERR_NICKNAMEINUSEC(nick)(":127.0.0.1 433 * " + nick + " :Nickname is already in use\r\n")
#define ERR_ERRONEUSUSERNAME(client, username)(": 432 " + client + " " + username + " :Erroneus username\r\n")
#define ERR_USERNAMEINUSE(client, username)(":127.0.0.1 433 * " + client + " " + username + " :Username is already in use\r\n")
#define ERR_NORECIPIENT(client, command)(": 411 " + client + " " + command + " :No recipient given\r\n")

#define ERR_USERNOTINCHANNEL(client, nick, channel)(": 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(client, channel)(": 442 " + client + " " + channel + " :Not on that channel\r\n")
#define ERR_NOTREGISTERED()(": 451 :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(client, cmd)(": 461 " + client + " " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(client)(":127.0.0.1 462 " + client + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(client)(":127.0.0.1 464 " + client + " :Password incorrect\r\n")
#define ERR_KEYSET(nick, channel)(":127.0.0.1 467 " + nick + " " + channel + " :Channel key already set\r\n")
#define ERR_CHANNELISFULL(client, channel)(": 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(client, mode)(": 472 " + client + " " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(client, channel)(": 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel)(": 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_NOCHANMODES(channel)(": 477 " + channel + " :Channel doesn't support modes\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel)(": 482 " + client + " " + channel + " :You're not channel operator\r\n")

#define RPL_CHANNELMODEIS(nick, channel, modes)(": 324 " + nick + " " + channel + " " + modes + "\r\n")