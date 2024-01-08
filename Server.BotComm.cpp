/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.BotComm.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 10:10:23 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 13:01:51 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleBotCommand(std::string params, int i, struct pollfd _pfds[])
{
	std::stringstream ss(params);
	std::string command;
	std::string target;
	ss >> command;
	if (command == "BOT" || command == "/BOT")
		ss >> command;
	ss >> target;
    std::string tr = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " :";
    if (command == "TIME" || command == "TIME\n")
    {
        std::time_t nowTime = std::time(NULL);
        char timeChar[6];
        std::strftime(timeChar, sizeof(timeChar),"%H:%M", std::localtime(&nowTime));
        std::string timestr = "" + std::string(timeChar) + "\n";
        // std::cout << timestr;
        std::string msg = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " :Current Time is: " + timestr;
        send(_pfds[i].fd, msg.c_str(), msg.length(), 0);
    }
    else if (command == "MAN" || command == "MAN\n" || command == "HELP" || command == "HELP\n")
    {
		std::string man = tr +  "************************************************************************************\r\n";
					man += tr + "*   Set UserName   : USER + (your username)                                        *\r\n";
					man += tr + "*   Set NickName   : NICK + (your nickname)                                        *\r\n";
					man += tr + "*   Join a channel : JOIN + #(channel name                                         *\r\n";
					man += tr + "*   KICK   Eject a client from the channe                                          *\r\n";
					man += tr + "*   INVITE   Invite a client to a channel                                          *\r\n";
					man += tr + "*   TOPIC   Change or view the channel topic                                       *\r\n";
					man += tr + "*   PART #channelName  Leave a channel                                             *\r\n";
					man += tr + "*   PRIVMSG   Send a private message to a client                                   *\r\n";
					man += tr + "*   LIST   List all channels                                                       *\r\n";
					man += tr + "*   MOTD   Message of the day                                                      *\r\n";
					man += tr + "*   MODE   Change the channel's mode:                                              *\r\n";
					man += tr + "*     +i: Set/remove Invite-only channel                                           *\r\n";
					man += tr + "*     +t: Set/remove the restrictions of the TOPIC command to channel operators    *\r\n";
					man += tr + "*     +k: Set/remove the channel key (password)                                    *\r\n";
					man += tr + "*     +o: Give/take channel operator privilege                                     *\r\n";
					man += tr + "*     +l: Set/remove the user limit to channel                                     *\r\n";
					man += tr + "*   Available BOT commands:                                                        *\r\n";
					man += tr + "*      /BOT MAN or HELP                                                            *\r\n";
					man += tr + "*      /BOT TIME                                                                   *\r\n";
					man += tr + "*      /BOT LIST                                                                   *\r\n";
					man += tr + "*      /BOT WHOAMI                                                                 *\r\n";
					man += tr + "*      /BOT WHOIS + nickName                                                       *\r\n";
					man += tr + "************************************************************************************\r\n";
        // std::cout << man;
        send(_pfds[i].fd, man.c_str(), man.length(), 0);
    }
	else if (command == "LIST" || command == "LIST\n")
	{
		handleListCommand(i);
	}
	else if (command == "WHOAMI" || command == "WHOAMI\n")
	{
		std::string msg = tr + "Your nickname is: " + ClientsMap[_pfds[i].fd].getNickname() + "\r\n";
		msg += tr + "Your username is: " + ClientsMap[_pfds[i].fd].getUserName() + "\r\n";
		msg += tr + "Your hostname is: " + ClientsMap[_pfds[i].fd]._client_host + "\r\n";
		msg += tr + "Your realname is: " + ClientsMap[_pfds[i].fd].getRealName() + "\r\n";
		msg += tr + "Your ip address is: " + ClientsMap[_pfds[i].fd].ipAddress + "\r\n";
		msg += tr + "Your joined channels are: ";
		std::map<std::string, Channels>::iterator it = channelsV.begin();
		while (it != channelsV.end())
		{
			if (it->second.haveClient(_pfds[i].fd))
			{
				msg += it->first + " ";
			}
			it++;
		}
		msg += "\r\n";
		msg += tr + "Your are operator in: ";
		it = channelsV.begin();
		while (it != channelsV.end())
		{
			if (it->second.isOperator(_pfds[i].fd))
			{
				msg += it->first + " ";
			}
			it++;
		}
		msg += "\r\n";
		send(_pfds[i].fd, msg.c_str(), msg.length(), 0);
	}
	else if (command == "WHOIS" || command == "WHOIS\n")
	{
		if (target.empty())
		{
			std::string msg = tr + "Please enter a nickname\r\n";
			send(_pfds[i].fd, msg.c_str(), msg.length(), 0);
			return ;
		}
		std::map<int, Client>::iterator it = ClientsMap.begin();
		while (it != ClientsMap.end())
		{
			if (it->second.getNickname() == target)
			{
				std::string msg = tr + "Nickname: " + it->second.getNickname() + "\r\n";
				msg += tr + "Username: " + it->second.getUserName() + "\r\n";
				msg += tr + "Hostname: " + it->second._client_host + "\r\n";
				msg += tr + "Realname: " + it->second.getRealName() + "\r\n";
				msg += tr + "Ip address: " + it->second.ipAddress + "\r\n";
				msg += tr + "Joined channels: ";
				std::map<std::string, Channels>::iterator it2 = channelsV.begin();
				while (it2 != channelsV.end())
				{
					if (it2->second.haveClient(it->first))
					{
						msg += it2->first + " ";
					}
					it2++;
				}
				msg += "\r\n";
				msg += tr + "Operator in: ";
				it2 = channelsV.begin();
				while (it2 != channelsV.end())
				{
					if (it2->second.isOperator(it->first))
					{
						msg += it2->first + " ";
					}
					it2++;
				}
				msg += "\r\n";
				send(_pfds[i].fd, msg.c_str(), msg.length(), 0);
				return ;
			}
			it++;
		}
		if (it == ClientsMap.end())
		{
			std::string msg = tr + "Nickname not found\r\n";
			send(_pfds[i].fd, msg.c_str(), msg.length(), 0);
		}
	}
	else if (command == "LIST" || command == "LIST\n")
	{
		handleListCommand(i);
	}
    else
    {
        std::string str = tr + " Available commands:\r\n";
        str += tr + " /BOT MAN\r\n";
		str += tr + " /BOT HELP\r\n";
        str += tr + " /BOT TIME\r\n";
		str += tr + " /BOT LIST\r\n";
		str += tr + " /BOT WHOAMI\r\n";
		str += tr + " /BOT WHOIS\r\n";
        send(_pfds[i].fd,str.c_str(), str.length(), 0);
    }
}