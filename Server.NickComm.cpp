/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.NickComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:34:52 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/21 10:36:33 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleNickCommand(std::string params, int i)
{
	params = trimString(params);
	if (params.empty())
	{
		std::cout << "Invalid NickName" << std::endl;
		std::string resp = "431 * Invalid NickName!\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	if (ClientsMap.find(_pfds[i].fd) != ClientsMap.end() && ClientsMap[_pfds[i].fd].getIsAutonticated()) {
		std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": You are already registered with a nickname\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	std::string nickName = params.substr(0, params.find("\r"));
	std::cout << "nickName: " << nickName << std::endl;
	bool check = true;
	for (std::map<int, Client>::iterator it = ClientsMap.begin(); it != ClientsMap.end(); it++)
	{
		if (it->second.getNickname() == nickName)
		{
			check = false;
			std::string resp = "433 * nickName already used\n";
			std::cout << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		}
	}
	if (check)
	{
		ClientsMap[_pfds[i].fd].setNickname(nickName);
		ClientsMap[_pfds[i].fd].setIsAutonticated();
		std::cout << "isAutonticated: " << std::boolalpha << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
		welcomeMessage(i);
	}
}
