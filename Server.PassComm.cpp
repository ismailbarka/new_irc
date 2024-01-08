/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.PassComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 10:39:46 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 10:23:33 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handlePassCommand(std::string params, int i)
{
	if (ClientsMap[_pfds[i].fd].getCorrectPassWord()) {
		std::string response = "462 * :You have already entred the password!\r\n";
		// std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		return;
	}
	std::string password = params.substr(0, params.find("\r"));
	// std::cout << "password: " << password << std::endl;
	if (password == _password) {
		ClientsMap[_pfds[i].fd].setCorrectPassWord(true);
		ClientsMap[_pfds[i].fd].setIsAutonticated();
		welcomeMessage(i);
	} else {
		std::string response = "464 * :Password incorrect\r\n";
		// std::cout << "response: " << response << " : " << password << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
	}
}
