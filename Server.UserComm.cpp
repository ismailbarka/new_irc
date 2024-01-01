/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.UserComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:37:01 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/01 15:52:33 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleUserCommand(std::string params, int i) {
	params = trimString(params);
	std::cout << "====> USER params: " << params << std::endl;
	if (ClientsMap[_pfds[i].fd].getNickname().empty())
	{
		std::string response = "ERROR you need to set a nickname first\r\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	if (ClientsMap.find(_pfds[i].fd) != ClientsMap.end() && ClientsMap[_pfds[i].fd].getIsAutonticated()) {
		std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": You are already registered with a username\r\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	std::stringstream ss(params);
	std::string userName;
	std::string zero;
	std::string asterisk;
	std::string realName;
	ss >> userName >> zero >> asterisk >> realName;
	std::cout << "userName: " << userName << std::endl;
	std::cout << "zero: " << zero << std::endl;
	std::cout << "asterisk: " << asterisk << std::endl;
	std::cout << "realName: " << realName << std::endl;
	if (zero != "0" || asterisk != "*" || realName.empty() || userName.empty())
	{
		std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": Invalid USER command\r\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	bool check = true;
	for (std::map<int, Client>::iterator it = ClientsMap.begin(); it != ClientsMap.end(); it++)
	{
		if (it->second.getUserName() == userName)
		{
			check = false;
			std::string resp = "ERROR userName already used\r\n";
			std::cout << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		}
	}
	if (check)
	{
		ClientsMap[_pfds[i].fd].setuserName(userName);
		ClientsMap[_pfds[i].fd].setIsAutonticated();
		ClientsMap[_pfds[i].fd].setRealName(realName);
		std::cout << "isAutonticated: " << std::boolalpha << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
		welcomeMessage(i);
	}
}
