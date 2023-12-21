/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.JoinComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:38:52 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/21 10:40:38 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleJoinCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
{
	std::string channel = params.substr(0, params.find(" "));
	params = trimString(params);
	if (params.empty() || params[0] != '#' || (params[0] == '#' && params[1] && !isalnum(params[1])) || (params[0] == '#' && !params[1]))
	{
		std::string resp = "ERROR Invalid channel name: " + params + "\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	std::map<std::string,Channels>::iterator it = channelsV.find(channel);
	if (it == channelsV.end())
	{
		// params = channel;
		std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + " JOIN " + params + "\n";
		resp += "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + params + "\n";
		// resp += "353 " + ClientsMap[_pfds[i].fd].getNickname() + " = " + params + " :" + ClientsMap[_pfds[i].fd].getNickname() + "\n";
		// resp += "366 " + ClientsMap[_pfds[i].fd].getNickname() + " " + params + " :End of NAMES list\n";
		resp += "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have successfully CREATED and JOINED " + params + "\n";
		std::cout << "response: " << resp;
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		Channels newChannel;
		newChannel.addClient(_pfds[i].fd);
		newChannel.setMode("o");
		channelsV[channel] = newChannel;
	}
	else
	{
		std::vector<int>::iterator it1 = it->second.getClientsFd().begin();
		while (it1 != it->second.getClientsFd().end() && *it1 != _pfds[i].fd)
			it1++;
		if (*it1 == _pfds[i].fd)
		{
			std::string resp = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + " You have already JOINED " + params + "\n";
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
			return;
		} else {
			std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + " JOIN " + params + "\n";
			resp += "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully JOINED " + params + "\n";
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
			it->second.addClient(_pfds[i].fd);
		}
	}
}
