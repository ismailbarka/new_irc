/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.QuitComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:40:47 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/03 11:58:55 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleQuitCommand(int i, int & clients_numbers, std::string params)
{
	std::string response = ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" 
	+ std::string(ClientsMap[_pfds[i].fd].ipAddress) + " QUIT :" + params + "\r\n";
	std::cout << "response: " << response << std::endl;
	// send(_pfds[i].fd, response.c_str(), response.length(), 0);
	for (int j = 1; j < clients_numbers; j++)
	{
		// if (_pfds[j].fd != _pfds[i].fd && _pfds[j].fd != _pfds[i].fd)
		// {
			send(_pfds[j].fd, response.c_str(), response.length(), 0);
		// }
	}
	std::cout << "client disconnected: " << ClientsMap[_pfds[i].fd].getNickname() << std::endl;
	ClientsMap.erase(_pfds[i].fd);
	close(_pfds[i].fd);
	_pfds[i].fd = -1;
	_pfds[i].events = 0;
	_pfds[i].revents = 0;
	(void)clients_numbers;
}