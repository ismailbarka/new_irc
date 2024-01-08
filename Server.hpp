/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:05:37 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 12:14:50 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channels.hpp"

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
#include <exception>
#include <sstream>
#include <stdio.h>
#include <arpa/inet.h>
#include <algorithm>
#include <ctime>
#include <string.h>
#include "time.h"

# define GREEN "\033[32m"
# define RED "\033[31m"
# define BLUE "\033[34m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"


class Client;
class Channels;
class Server
{
private:
public:
	int serverSocket;
	struct pollfd _pfds[1024];
	std::string nickName;
	std::string userName;
	// bool isAutonticated;
	std::vector<std::string> commandList;
	struct sockaddr_in server_addr;
	std::map<std::string,Channels> channelsV;
	std::map<int, Client> ClientsMap;
	std::string _password;
	Server();
	//create own exception
	class ServerException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Server Exception"); }
	};
	class bindException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Bind Exception"); }
	};
	class listenException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Listen Exception"); }
	};
	class acceptException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Accept Exception"); }
	};
	class recvException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Recv Exception"); }
	};
	class sendException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Send Exception"); }
	};
	class pollException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Poll Exception"); }
	};
	class socketException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Socket Exception"); }
	};
	class fcntlException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Fcntl Exception"); }
	};
	class setsockoptException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("Setsockopt Exception"); }
	};
	class setPollfdException : public std::exception {
		public:
			virtual const char* what() const throw() { return ("SetPollfd Exception"); }
	};
	
	Server(char **av);
	void bindSocket();
	void listenSocket();
	void setPollfd();
	void setPollfd(struct pollfd _pfd);
	void setPollfd(struct pollfd _pfd, int index);
	void welcomeMessage(int i);
	void handleNickCommand(std::string params, int i);
	void handleBotCommand(std::string params, int i, struct pollfd _pfds[]);
	void handleUserCommand(std::string params, int i);
	void handleJoinCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[]);
	void handleListCommand(int i);
	void handleQuitCommand(int i, int & clients_numbers, std::string params);
	void handlePrivMsg(std::string params, int i, std::map<std::string,Channels> &channelsV);
	void handleModeCommand(std::string params,int i,std::map<std::string, Channels> & channelsV);
	void handleInviteCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[]);
	void handleKickCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[]);
	void handleTopicCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[]);
	void handlePartCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[]);
	void handlePassCommand(std::string params, int i);
	void startServer();
	void firstJoin(std::string key, std::string value, bool pass, int i, std::map<std::string, Channels> &channelsV);
	void joinChannel(std::string key, int i, std::map<std::string, Channels> &channelsV, std::string value);
	void authCommand(std::string command, std::string params, int i);
	void nonAuthCommand(int i);
	void welcomeNewClient(int & clients_numbers);
	~Server();
	int nameTofd(std::string nickName)
	{
		std::map<int, Client>::iterator it = ClientsMap.begin();
		while(it != ClientsMap.end())
		{
			if(it->second.getNickname() == nickName)
				return it->first;
			it++;
		}
		return -1;
	}
};
std::string trimString(const std::string& str);
bool validNumber(std::string number);

#endif
