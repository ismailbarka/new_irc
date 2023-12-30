/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 11:02:31 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/26 15:57:13 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <map>
#include <arpa/inet.h>
class Client
{
	private:
		struct pollfd pFd;
		std::string nickName;
		std::string userName;
		bool isAutonticated;
		std::vector<std::string> commandList;
		bool correctPassword;
	public:
		char ipAddress[INET_ADDRSTRLEN];
		std::string _client_host;
		// std::string ipAddress;
		void setPollfd(struct pollfd _pFd);
		void setNickname(std::string _nickName);
		void setuserName(std::string _userName);
		std::string getNickname();
		std::string getUserName();
		bool getIsAutonticated();
		void setClientIp(char* ip);
		void setIsAutonticated();
		int getfd();
		void setCorrectPassWord(bool _correctPassword);
		bool getCorrectPassWord();
		Client();
		Client(struct pollfd client);
		std::string getClientHost() const ;
		void setClientHost(std::string host);
		// void fillCommandList()
		// {
		// 	commandList.push_back("USER");
		// 	commandList.push_back("NICK");
		// 	commandList.push_back("LIST");
		// 	commandList.push_back("QUIT");
		// }
		~Client();
};

#endif