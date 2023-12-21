/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.PrivMsgComm.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:41:43 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/21 10:42:34 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handlePrivMsg(std::string params, int i, std::map<std::string,Channels> &channelsV)
{
	std::string sender = ClientsMap[_pfds[i].fd].getNickname();
	std::string target = params.substr(0, params.find(" "));
	std::string message = params.substr(params.find(" ") + 1);
	message = message.substr(message.find(":") + 1);
	std::cout << "------PRIVMSG------" << std::endl;
	std::cout << "sender: " << sender << std::endl;
	std::cout << "target: " << target << std::endl;
	std::cout << "message: " << message << std::endl;
	if (target[0] == '#') { //channel messages
		std::map<std::string,Channels>::iterator it = channelsV.find(target);
		if (it == channelsV.end())
		{
			std::string resp = "403 " + ClientsMap[_pfds[i].fd].getNickname() + " :YOU NEED TO CREATE/JOIN THIS CHANNEL FIRST: " + target + "\n";
			std::cout << "response: " << resp;
			if (send(_pfds[i].fd, resp.c_str(), resp.length(), 0) == -1)
				throw sendException();
		} else {
			//check if the client is not in the channel
			std::vector<int>::iterator it1 = it->second.getClientsFd().begin();
			while (it1 != it->second.getClientsFd().end() && *it1 != _pfds[i].fd)
				it1++;
			if (it1 == it->second.getClientsFd().end())
			{
				std::string resp = "ERROR YOU ARE NOT IN THIS CHANNEL [" + target.substr(target.find("#") + 1) + "]\n";
				std::cout << "response: " << resp;
				if (send(_pfds[i].fd, resp.c_str(), resp.length(), 0) == -1)
					throw sendException();
				return;
			}
			std::vector<int> clientFds = it->second.getClientFd();
			it1 = clientFds.begin();
			while (it1 != clientFds.end())
			{
				std::map<int, Client>::iterator itt = ClientsMap.find(*it1);
				if (itt->second.getIsAutonticated() == true && itt->second.getNickname() != sender) {
					std::string resp = ":" + sender + " PRIVMSG " + target + " :" + message + "\n";
					std::cout << "response: " << resp << std::endl;
					if (send(*it1, resp.c_str(), resp.length(), 0) == -1)
						throw sendException();
				}
				it1++;
			}
		}
	} else { //private messages
		std::map<int, Client>::iterator it;
		for (it = ClientsMap.begin(); it != ClientsMap.end(); it++) {
			if (it->second.getIsAutonticated() && it->second.getNickname() == target) {
				std::string response = ":" + ClientsMap[_pfds[i].fd].getNickname() + " PRIVMSG " + target + " :" + message + "\n";
				std::cout << "response: " << response << std::endl;
				if (send(it->first, response.c_str(), response.length(), 0) == -1)
					throw sendException();
				break ;
			}
		}
		if (it == ClientsMap.end()) {
			std::string response = "401 " + ClientsMap[_pfds[i].fd].getNickname() + " Error sending messgae, no such nick: [ " + target + " ]\n";
			std::cout << "response: " << response << std::endl;
			send(_pfds[i].fd, response.c_str(), response.length(), 0);
		}
	}
}