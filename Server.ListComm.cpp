/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.ListComm.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:45:29 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 12:13:56 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleListCommand(int i)
{
	//list all channels
	std::string reponse = "321 Channel :Users  Name\r\n";
	send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
	std::map<std::string, Channels>::iterator it = channelsV.begin();
	while (it != channelsV.end())
	{
		std::string reponse = "322 Channel " + it->first + " " + std::to_string(it->second.clientsFd.size()) + " " + it->second.topic + "\r\n";
		//membres count
		// std::cout << reponse << std::endl;
		send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
		it++;
	}
	std::string reponse2 = "323 End of /LIST\r\n";
	send(_pfds[i].fd, reponse2.c_str(), reponse2.length(), 0);
}
