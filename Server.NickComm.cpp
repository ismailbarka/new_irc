/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.NickComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:34:52 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 10:17:44 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string ft_toUpper(std::string str)
{
	std::string toReturn(str);
	for (size_t i = 0; i < str.length(); i++)
	{
		toReturn[i] = toupper(str[i]);
	}
	return toReturn;
}

void Server::handleNickCommand(std::string params, int i)
{
	if (params.empty() || params[0] == ':')
	{
		// std::cout << "Invalid NickName" << std::endl;
		std::string resp = "432 * Invalid NickName!\r\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	if (ClientsMap.find(_pfds[i].fd) != ClientsMap.end() && ClientsMap[_pfds[i].fd].getIsAutonticated()) {
		std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": You are already registered with a nickname\r\n";
		// std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	std::string nickName = params.substr(0, params.find("\r"));
	// std::cout << "nickName: " << nickName << std::endl;
	bool check = true;
	for (std::map<int, Client>::iterator it = ClientsMap.begin(); it != ClientsMap.end(); it++)
	{
		if (ft_toUpper(it->second.getNickname()) == ft_toUpper(nickName))
		{
			check = false;
			std::string resp = "433 * nickName already used\r\n";
			// std::cout << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		}
	}
	if (check)
	{
		ClientsMap[_pfds[i].fd].setNickname(nickName);
		ClientsMap[_pfds[i].fd].setIsAutonticated();
		// std::cout << "isAutonticated: " << std::boolalpha << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
		welcomeMessage(i);
	}
}
