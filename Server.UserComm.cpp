/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.UserComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:37:01 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 14:43:31 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleUserCommand(std::string params, int i) {
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
	std::string rn;
	ss >> userName >> zero >> asterisk;
	while (ss >> rn)
		realName += rn + " ";
	if (realName.empty())
		realName = ClientsMap[_pfds[i].fd].getNickname();
	// std::cout << "userName: " << userName << std::endl;
	// std::cout << "zero: " << zero << std::endl;
	// std::cout << "asterisk: " << asterisk << std::endl;
	// std::cout << "realName: " << realName << std::endl;
	if (zero != "0" || asterisk != "*" || realName.empty() || userName.empty())
	{
		std::string response = "468 * Invalid USER command\r\n";
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		response = "468 * example: USER <username> 0 * <realname>\r\n";
		// std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	ClientsMap[_pfds[i].fd].setuserName(userName);
	ClientsMap[_pfds[i].fd].setIsAutonticated();
	if (realName[0] == ':')
		realName.erase(0, 1);
	ClientsMap[_pfds[i].fd].setRealName(realName);
	// std::cout << "isAutonticated: " << std::boolalpha << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
	welcomeMessage(i);
}
