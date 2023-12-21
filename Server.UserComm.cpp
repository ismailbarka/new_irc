/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.UserComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:37:01 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/21 10:37:56 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleUserCommand(std::string params, int i) {
	params = trimString(params);
	if (ClientsMap[_pfds[i].fd].getNickname().empty())
	{
		std::string response = "ERROR you need to set a nickname first\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	if (params.empty())
	{
		std::cout << "Invalid UserName" << std::endl;
		std::string resp = "ERROR Invalid UserName!\n";
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	if (ClientsMap.find(_pfds[i].fd) != ClientsMap.end() && ClientsMap[_pfds[i].fd].getIsAutonticated()) {
		std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": You are already registered with a username\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	std::string userName = params.substr(0, params.find(" "));
	userName = userName.substr(userName.find(" ") + 1);
	userName = userName.substr(0, userName.find(" "));
	userName = userName.substr(userName.find("\n") + 1);
	std::cout << "userName: " << userName << std::endl;
	bool check = true;
	for (std::map<int, Client>::iterator it = ClientsMap.begin(); it != ClientsMap.end(); it++)
	{
		if (it->second.getUserName() == userName)
		{
			check = false;
			std::string resp = "ERROR userName already used\n";
			std::cout << resp;
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		}
	}
	if (check)
	{
		ClientsMap[_pfds[i].fd].setuserName(userName);
		ClientsMap[_pfds[i].fd].setIsAutonticated();
		std::cout << "isAutonticated: " << std::boolalpha << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
		welcomeMessage(i);
	}
}
