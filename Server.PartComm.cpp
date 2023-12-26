/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.PartComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 11:54:49 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/26 12:20:07 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handlePartCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
{
	std::stringstream ss;
	ss << params;
	std::string channel;
	std::string msg;
	ss >> channel;
	ss >> msg;
	std::map<std::string,Channels>::iterator it = channelsV.find(channel);
	if (it != channelsV.end())
	{
		//check if the client is not in the channel
		std::vector<int>::iterator it1 = it->second.getClientsFd().begin();
		while (it1 != it->second.getClientsFd().end() && *it1 != _pfds[i].fd)
			it1++;
		if (it1 == it->second.getClientsFd().end())
		{
			std::string resp = "ERROR YOU ARE NOT IN THIS CHANNEL [" + channel.substr(channel.find("#") + 1) + "]\n";
			std::cout << "response: " << resp;
			if (send(_pfds[i].fd, resp.c_str(), resp.length(), 0) == -1)
				throw sendException();
		} else {
			it->second.removeClient(_pfds[i].fd);
			std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + " PART " + channel + " :" + msg + "\n";
			std::cout << "response: " << resp << std::endl;
			std::vector<int> clientFds = it->second.getClientFd();
			std::vector<int>::iterator it1 = clientFds.begin();
			while (it1 != clientFds.end())
			{
				std::map<int, Client>::iterator itt = ClientsMap.find(*it1);
				if (itt->second.getIsAutonticated() == true && itt->second.getNickname() != ClientsMap[_pfds[i].fd].getNickname()) {
					std::cout << "resp: " << ClientsMap[_pfds[i].fd].getNickname() << " m: " << msg << " c: " << channel << std::endl;
					if (send(*it1, resp.c_str(), resp.length(), 0) == -1)
						throw sendException();
				}
				it1++;
			}
		}
	} else {
		std::string resp = "ERROR NO CHANNEL WITH THIS NAME [" + channel.substr(channel.find("#") + 1) + "]\n";
		resp += "ERROR YOU NEED TO CREATE/JOIN THIS CHANNEL FIRST\n";
		std::cout << "response: " << resp;
		if (send(_pfds[i].fd, resp.c_str(), resp.length(), 0) == -1)
			throw sendException();
	}
}
