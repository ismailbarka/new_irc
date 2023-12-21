/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.ModeComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:42:48 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/21 10:44:05 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleModeCommand(std::string params,int i,std::map<std::string, Channels> & channelsV)
{
	if (params.empty())
	{
		std::string resp = "ERROR Invalid channel name: " + params + "\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	std::string channelName = params.substr(0, params.find(" "));
	std::string mode = params.substr(params.find(" ") + 1);
	mode = mode.substr(mode.find(" ") + 1);
	mode = mode.substr(0, mode.find("\r"));
	std::cout << "channelName: " << channelName << std::endl;
	std::cout << "mode: " << mode << std::endl;
	if (mode != "+i" && mode != "-i" && mode != "+t" && mode != "-t" && mode != "+k" && mode != "-k" && mode != "+o" && mode != "-o" && mode != "+l" && mode != "-l")
	{
		std::string resp = "ERROR Invalid mode: " + mode + "\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	std::map<std::string,Channels>::iterator it = channelsV.find(channelName);
	if (it != channelsV.end())
	{
		std::vector<int>::iterator it1 = it->second.getClientsFd().begin();
		while (it1 != it->second.getClientsFd().end() && *it1 != _pfds[i].fd)
			it1++;
		if (it1 != it->second.getClientsFd().end())
		{
			std::string resp = "MODE " + channelName + " " + mode + "\n";
			//set channel mode
			it->second.setMode(mode);
			std::cout << "response: " << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		} else {
			std::string resp = "ERROR YOU ARE NOT IN THIS CHANNEL [" + channelName.substr(channelName.find("#") + 1) + "]\n";
			std::cout << "response: " << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		}
	} else {
		std::string resp = "ERROR NO CHANNEL WITH THIS NAME [" + channelName.substr(channelName.find("#") + 1) + "]\n";
		resp += "ERROR YOU NEED TO CREATE/JOIN THIS CHANNEL FIRST\n";
		std::cout << "response: " << resp;
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
	}
}