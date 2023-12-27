/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.ModeComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:42:48 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/27 13:55:42 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool validMode(std::string mode)
{
	if (mode.size() < 2)
		return false;
	if (mode[0] != '+' && mode[0] != '-')
		return false;
	int i = 1;
	while (mode[i] != '\0')
	{
		if (mode[i] != 'i' && mode[i] != 't' && mode[i] != 'k' && mode[i] != 'o' && mode[i] != 'l')
			return false;
		i++;
	}
	return true;
}

void Server::handleModeCommand(std::string params,int i,std::map<std::string, Channels> & channelsV)
{
	if (params.empty())
	{
		std::string resp = "ERROR Invalid channel name: " + params + "\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	std::stringstream ss(params);
	std::string channelName;
	std::string mode;
	std::string target;
	ss >> channelName;
	ss >> mode;
	ss >> target;
	std::cout << "channelName: " << channelName << std::endl;
	std::cout << "mode: " << mode << std::endl;
	std::cout << "target: " << target << std::endl;
	if (!validMode(mode))
	{
		std::string resp = "ERROR Invalid mode: " + mode + "\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	std::map<std::string,Channels>::iterator it = channelsV.find(channelName);
	if (it != channelsV.end())
	{
		std::vector<int>::iterator it1 = it->second.clientsFd.begin();
		while (it1 != it->second.clientsFd.end())
		{
			if (*it1 == _pfds[i].fd)
				break;
			it1++;
		}
		if (it1 != it->second.clientsFd.end())
		{
			std::vector<Client>::iterator it2 = it->second.operators.begin();
			while (it2 != it->second.operators.end())
			{
				if (it2->getNickname() == ClientsMap[_pfds[i].fd].getNickname())
					break;
				it2++;
			}
			if (it2 == it->second.operators.end())
			{
				std::string resp = "482 " + ClientsMap[_pfds[i].fd].getNickname() + " " + channelName + " :You're not channel operator\n";
				std::cout << "response: " << resp;
				send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
				return;
			}
			//set channel mode
			it->second.setMode(mode);
			
			if (mode.find("k") != std::string::npos)
			{
				std::string key = target.substr(0, target.find("\r"));
				if (mode[0] == '+') {
					it->second.setKey(key);
					it->second.havePassword = true;
				} else {
					it->second.setKey("");
					it->second.havePassword = false;
				}
			}
			if (mode.find("l") != std::string::npos)
			{
				std::string limit = target.substr(0, target.find("\r"));
				if (mode[0] == '+') {
					it->second.setLimit(limit);
				} else {
					it->second.setLimit("");
				}
			}
			if (mode.find("o") != std::string::npos)
			{
				if (target.empty())
				{
					std::string resp = "461 " + ClientsMap[_pfds[i].fd].getNickname() + " MODE :Not enough parameters\n";
					std::cout << "response: " << resp;
					send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
					return;
				}
				std::string nickname = target.substr(0, target.find("\r"));
				std::vector<int>::iterator it3 = it->second.clientsFd.begin();
				while (it3 != it->second.clientsFd.end())
				{
					if (ClientsMap[*it3].getNickname() == nickname)
						break;
					it3++;
				}
				if (it3 == it->second.clientsFd.end())
				{
					std::string resp = "401 " + ClientsMap[_pfds[i].fd].getNickname() + " " + nickname + " :No such nick/channel\n";
					std::cout << "response: " << resp;
					send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
					return;
				}
				if (mode[0] == '+') {
					it->second.operators.push_back(ClientsMap[*it3]);
				} else {
					std::vector<Client>::iterator it4 = it->second.operators.begin();
					while (it4 != it->second.operators.end())
					{
						if (it4->getNickname() == nickname)
							break;
						it4++;
					}
					if (it4 != it->second.operators.end())
						it->second.operators.erase(it4);
				}
			}
			if (mode.find("i") != std::string::npos)
			{
				if (mode[0] == '+') {
					it->second.setMode(mode);
				} else {
					it->second.setMode(mode);
				}
			}
			if (mode.find("t") != std::string::npos)
			{
				if (mode[0] == '+') {
					it->second.setMode(mode);
				} else {
					it->second.setMode(mode);
				}
			}
			std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + " MODE " + channelName + " " + mode + " " + target + "\n";
			std::cout << "response: " << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		} else {
			std::string resp = "ERROR YOU ARE NOT IN THIS CHANNEL [" + channelName.substr(channelName.find("#") + 1) + "]\n";
			std::cout << "response: " << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		}
	} else {
		std::string resp = "403 " + ClientsMap[_pfds[i].fd].getNickname() + " :" + channelName +" No such channel\n";
		// resp += "ERROR YOU NEED TO CREATE/JOIN THIS CHANNEL FIRST\n";
		std::cout << "response: " << resp;
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
	}
}