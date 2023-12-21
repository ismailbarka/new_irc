/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.ListComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:45:29 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/21 10:46:18 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleListCommand(int i, int clients_numbers)
{
	std::map<int, Client>::iterator it;
	std::cout << "clients_numbers: " << clients_numbers - 1 << std::endl;
	for (it = ClientsMap.begin(); it != ClientsMap.end(); it++) {
		if (it->second.getIsAutonticated()) {
			std::string res = "[" + std::to_string(it->second.getfd());
			res += "] [" + it->second.getNickname() + "] [ " + it->second.getUserName() + " ]\n";
			std::string response = "393 "+ ClientsMap[_pfds[i].fd].getNickname() + res + "\n";
			std::cout << "response: " << response;
			send(_pfds[i].fd, response.c_str(), response.length(), 0);
		}
	}
}
