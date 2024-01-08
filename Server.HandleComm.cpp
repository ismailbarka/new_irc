/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.HandleComm.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 10:29:12 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 12:25:13 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::authCommand(std::string command, std::string params, int i) {
	if (command == "PASS" || command == "PASS\n") {
		std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": You are already registered with a password\r\n";
		// std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
	} else if (command == "PRIVMSG") {
		handlePrivMsg(params, i,channelsV);
	} else if (command == "LIST" || command == "LIST\n") {
		handleListCommand(i);
	} else if (command == "JOIN" || command == "JOIN\n") {
		handleJoinCommand(params, i, channelsV, _pfds);
	} else if (command == "INVITE" || command == "INVITE\n") {
		handleInviteCommand(params, i, channelsV, _pfds);
	} else if (command == "TOPIC" || command == "TOPIC\n") {
		handleTopicCommand(params, i, channelsV, _pfds);
	} else if (command == "KICK" || command == "KICK\n") {
		handleKickCommand(params, i, channelsV, _pfds);
	} else if (command == "MODE" || command == "MODE\n") {
		handleModeCommand(params, i, channelsV);
	} else if (command == "BOT" || command == "BOT\n") {
		handleBotCommand(params, i, _pfds);
	} else if (command == "PART" || command == "PART\n") {
		handlePartCommand(params, i, channelsV, _pfds);
	} else if (command == "MOTD") {
		welcomeMessage(i); }
	else if (command == "HELP" || command == "HELP\n") {
		handleBotCommand("HELP", i, _pfds);
	} else {
		std::string response = "421 " + ClientsMap[_pfds[i].fd].getNickname() + " :Unknown command: " + command + "\r\n";
		// std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
	}
}

void  Server::nonAuthCommand(int i) {
	std::string response = "ERROR you need to be autonticated first\r\n";
	if (ClientsMap[_pfds[i].fd].getNickname().empty())
		response += "ERROR you need to set a nickname !\n";
	if (ClientsMap[_pfds[i].fd].getUserName().empty())
		response += "ERROR you need to set a username !\n";
	if (ClientsMap[_pfds[i].fd].getCorrectPassWord() == false)
		response += "ERROR you need to enter the server password !\r\n";
	// std::cout << "response: " << response << std::endl;
	send(_pfds[i].fd, response.c_str(), response.length(), 0);
}
