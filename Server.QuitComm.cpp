/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.QuitComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:40:47 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/07 17:35:38 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleQuitCommand(int i, int & clients_numbers, std::string params)
{
	if (ClientsMap[_pfds[i].fd].getIsAutonticated())
	{
		std::string response = "ERROR :Closing Link: " + ClientsMap[_pfds[i].fd].getNickname() + " (Client Quit)\r\n";
		
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		
		for (std::map<std::string,Channels>::iterator it = channelsV.begin(); it != channelsV.end(); ++it)
		{
			if (it->second.haveClient(_pfds[i].fd))
				handlePartCommand(it->first, i, channelsV, _pfds);
		}
		response = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" 
		+ std::string(ClientsMap[_pfds[i].fd].ipAddress) + " QUIT " + params + "\r\n";
		
		for (int j = 1; j < clients_numbers; j++)
		{
			if (_pfds[j].fd != -1) {
				if (send(_pfds[j].fd, response.c_str(), response.length(), 0) == -1)
					throw sendException();
			}
		}
	}
	
	ClientsMap.erase(_pfds[i].fd);
	close(_pfds[i].fd);
	_pfds[i].fd = -1;
	_pfds[i].events = 0;
	_pfds[i].revents = 0;
	(void)clients_numbers;
}