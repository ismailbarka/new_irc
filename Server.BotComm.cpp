/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.BotComm.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 10:10:23 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/07 16:44:27 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleBotCommand(std::string params, int i, struct pollfd _pfds[])
{
    std::string tr = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " :";
    if (params == "TIME" || params == "TIME\n")
    {
        std::time_t nowTime = std::time(NULL);
        char timeChar[6];
        std::strftime(timeChar, sizeof(timeChar),"%H:%M", std::localtime(&nowTime));
        std::string timestr = "" + std::string(timeChar) + "\n";
        // std::cout << timestr;
        std::string msg = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " :Current Time is: " + timestr;
        send(_pfds[i].fd, msg.c_str(), msg.length(), 0);
    }
    else if (params == "MAN" || params == "MAN\n")
    {
		std::string man = tr +  "************************************************************************************\r\n";
					man += tr + "*   Set UserName   : USER + (your username)                                        *\r\n";
					man += tr + "*   Set NickName   : NICK + (your nickname)                                        *\r\n";
					man += tr + "*   Join a channel : JOIN + #(channel name                                         *\r\n";
					man += tr + "*   KICK   Eject a client from the channe                                          *\r\n";
					man += tr + "*   INVITE   Invite a client to a channel                                          *\r\n";
					man += tr + "*   TOPIC   Change or view the channel topic                                       *\r\n";
					man += tr + "*   MODE   Change the channel's mode:                                              *\r\n";
					man += tr + "*     +i: Set/remove Invite-only channel                                           *\r\n";
					man += tr + "*     +t: Set/remove the restrictions of the TOPIC command to channel operators    *\r\n";
					man += tr + "*     +k: Set/remove the channel key (password)                                    *\r\n";
					man += tr + "*     +o: Give/take channel operator privilege                                     *\r\n";
					man += tr + "*     +l: Set/remove the user limit to channel                                     *\r\n";
					man += tr + "************************************************************************************\r\n";
        // std::cout << man;
        send(_pfds[i].fd, man.c_str(), man.length(), 0);
    }
    else
    {
        std::string str = tr + " Available commands:\r\n";
        str += tr + " /BOT MAN\r\n";
        str += tr + " /BOT TIME\r\n";
        send(_pfds[i].fd,str.c_str(), str.length(), 0);
    }
}