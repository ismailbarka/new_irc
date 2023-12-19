/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:05:37 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/19 16:29:44 by tmoumni          ###   ########.fr       */
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


class Client;
class Server
{
private:
	int serverSocket;
	struct pollfd _pfds[1024];
	std::string nickName;
	std::string userName;
	// bool isAutonticated;
	std::vector<std::string> commandList;
	struct sockaddr_in server_addr;
	std::map<int, Client> ClientsMap;
	std::string _password;
public:
	//create own exception
	class ServerException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Server Exception"); }
	};
	class bindException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Bind Exception"); }
	};
	class listenException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Listen Exception"); }
	};
	class acceptException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Accept Exception"); }
	};
	class recvException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Recv Exception"); }
	};
	class sendException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Send Exception"); }
	};
	class pollException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Poll Exception"); }
	};
	class socketException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Socket Exception"); }
	};
	class fcntlException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Fcntl Exception"); }
	};
	class setsockoptException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("Setsockopt Exception"); }
	};
	class setPollfdException : public std::exception
	{
		public:
			virtual const char* what() const throw() { return ("SetPollfd Exception"); }
	};
	
	Server(char **av) {
		_password = av[2];
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
			throw socketException();
		int tmp = 1;
		int nRet = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
		if (!nRet) {
			std::cout << "nRet success\n";
		} else {
			std::cout << "nRet failed\n";
			throw setsockoptException();
		}
		int nFl = fcntl(serverSocket, F_SETFL, O_NONBLOCK);
		if (nFl < 0) {
			std::cout << "failed to set socket to non-blocking mode\n";
			throw fcntlException();
		} else {
			std::cout << "socket to non-blocking mode\n";
		}
		memset(&server_addr, '\0', sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htons(INADDR_ANY);
		server_addr.sin_port = htons(atoi(av[1]));
	}

	void bindSocket()
	{
		int bCheck = bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if (bCheck < 0)
			throw bindException();
	}

	void listenSocket()
	{
		if (listen(serverSocket, 128) < 0)
			throw listenException();
	}

	void setPollfd()
	{
		_pfds[0].fd = serverSocket;
		_pfds[0].events = POLLIN;
		_pfds[0].revents = 0;
	}

	void setPollfd(struct pollfd _pfd)
	{
		_pfds[0] = _pfd;
	}

	void setPollfd(struct pollfd _pfd, int index)
	{
		_pfds[index] = _pfd;
	}

	std::string trimString(const std::string& str) {
		size_t start = 0;
		size_t end = str.length();

		while (start < end && std::isspace(str[start]))
			++start;
		while (end > start && std::isspace(str[end - 1]))
			--end;
		return str.substr(start, end - start);
	}

	void handleNickCommand(std::string params, int i)
	{
		params = trimString(params);
		if (params.empty())
		{
			std::cout << "Invalid NickName" << std::endl;
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
				std::string resp = "ERROR nickName already used\n";
				std::cout << resp;
				send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
			}
		}
		if (check)
		{
			ClientsMap[_pfds[i].fd].setNickname(nickName);
			ClientsMap[_pfds[i].fd].setIsAutonticated();
			std::cout << "isAutonticated: " << std::boolalpha << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
			if (ClientsMap[_pfds[i].fd].getIsAutonticated())
			{
				std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :Welcome to the Internet Relay Network " + ClientsMap[_pfds[i].fd].getNickname() + "\n";
				std::cout << "response: " << response << std::endl;
				send(_pfds[i].fd, response.c_str(), response.length(), 0);
			}
		}
	}

	void handleUserCommand(std::string params, int i)
	{
		params = trimString(params);
		if (params.empty())
		{
			std::cout << "Invalid UserName" << std::endl;
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
			if (ClientsMap[_pfds[i].fd].getIsAutonticated())
			{
				std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :Welcome to the Internet Relay Network " + ClientsMap[_pfds[i].fd].getNickname() + "\n";
				std::cout << "response: " << response << std::endl;
				send(_pfds[i].fd, response.c_str(), response.length(), 0);
			}
		}
	}

	void handleJoinCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
	{
		params = trimString(params);
		if (params.empty() || params[0] != '#' || (params[0] == '#' && params[1] && !isalnum(params[1])))
		{
			std::string resp = "ERROR Invalid channel name: " + params + "\n";
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
			return;
		}
		std::map<std::string,Channels>::iterator it = channelsV.find(params);
		if (it == channelsV.end())
		{
			std::string resp = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully CREATED and JOINED " + params + "\n";
			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
			Channels newChannel;
			newChannel.addClient(_pfds[i].fd);
			channelsV[params] = newChannel;
		}
		else
		{
			std::vector<int>::iterator it1 = it->second.getClientsFd().begin();
			while (it1 != it->second.getClientsFd().end() && *it1 != _pfds[i].fd)
				it1++;
			if (it1 != it->second.getClientsFd().end())
			{
				std::string resp = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have already JOINED " + params + "\n";
				send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
				return;
			} else {
				std::string resp = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully JOINED " + params + "\n";
				send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
				it->second.addClient(_pfds[i].fd);
			}
		}
	}

	void handleListCommand(int i, int clients_numbers)
	{
		std::map<int, Client>::iterator it;
		std::cout << "clients_numbers: " << clients_numbers - 1 << std::endl;
		for (it = ClientsMap.begin(); it != ClientsMap.end(); it++) {
			if (it->second.getIsAutonticated()) {
				std::string res = "[" + std::to_string(it->second.getfd());
				res += "] [" + it->second.getNickname() + "] [ " + it->second.getUserName() + " ]\n";
				std::string response = "251 * :" + res + "\n";
				std::cout << "response: " << response;
				send(_pfds[i].fd, response.c_str(), response.length(), 0);
			}
		}
	}
	

	void handleQuitCommand(int i, int & clients_numbers)
	{
		std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :Bye Bye " + ClientsMap[_pfds[i].fd].getNickname() + "\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
		std::cout << "client disconnected: " << ClientsMap[_pfds[i].fd].getNickname() << std::endl;
		ClientsMap.erase(_pfds[i].fd);
		close(_pfds[i].fd);
		_pfds[i].fd = -1;
		_pfds[i].events = 0;
		_pfds[i].revents = 0;
		(void)clients_numbers;
	}

	void handlePrivMsg(std::string params, int i, std::map<std::string,Channels> &channelsV)
	{
		std::string sender = ClientsMap[_pfds[i].fd].getNickname();
		std::string target = params.substr(0, params.find(" "));
		std::string message = params.substr(params.find(" ") + 1);
		message = message.substr(message.find(":") + 1);
		std::cout << "sender: " << sender << std::endl;
		std::cout << "target: " << target << std::endl;
		std::cout << "message: " << message << std::endl;
		if (target[0] == '#') {
			std::map<std::string,Channels>::iterator it = channelsV.find(target);
			if (it == channelsV.end())
			{
				std::cout << "no channel with this name\n";
				return ;
			} else {
				std::vector<int> clientFds = it->second.getClientFd();
				std::vector<int>::iterator it1 = clientFds.begin();
				while (it1 != clientFds.end())
				{
					std::map<int, Client>::iterator itt = ClientsMap.find(*it1);
					if (itt->second.getIsAutonticated() == true && itt->second.getNickname() != sender) {
						std::string resp = ":" + sender + " PRIVMSG " + target + " " + message + "\n";
						if (send(*it1, resp.c_str(), strlen(resp.c_str()), 0) == -1)
							throw sendException();
					}
					it1++;
				}
			}
		} else {
			std::map<int, Client>::iterator it;
			for (it = ClientsMap.begin(); it != ClientsMap.end(); it++) {
				if (it->second.getIsAutonticated() && it->second.getNickname() == target) {
					std::string response = ":" + ClientsMap[_pfds[i].fd].getNickname() + " PRIVMSG " + target + " :" + message + "\n";
					std::cout << "response: " << response << std::endl;
					
					if (send(it->first, response.c_str(), response.length(), 0) == -1)
						throw sendException();
					break;
				}
			}
			if (it == ClientsMap.end()) {
				std::string response = "Error sending messgae, no such nick: [ " + target + " ]\n";
				std::cout << "response: " << response << std::endl;
				send(_pfds[i].fd, response.c_str(), response.length(), 0);
			}
		}
	}

	void startServer()
	{
		bindSocket();
		listenSocket();
		setPollfd();
		std::map<std::string,Channels> channelsV;
		int clients_numbers = 1;
		while (1)
		{
			char client_ip[INET_ADDRSTRLEN];
			int pollResult = poll(_pfds, clients_numbers, -1);
			if (pollResult == -1) {
				throw pollException();
			}
			if (_pfds[0].revents & POLLIN) {
				std::cout << "new client connected\n";
				struct sockaddr_in client_addr;
				socklen_t client_addr_size = sizeof(client_addr);
				int clientSocket = accept(serverSocket, (struct sockaddr *)&client_addr, &client_addr_size);
				if (clientSocket < 0)
					throw acceptException();
				int nFl = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
				if (nFl < 0) {
					std::cout << "failed to set socket to non-blocking mode\n";
					throw fcntlException();
				} else {
					std::cout << "socket to non-blocking mode\n";
				}
				struct pollfd client;
				client.fd = clientSocket;
				client.events = POLLIN | POLLHUP;
				client.revents = 0;
				inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN); // ?
				std::cout << "client ip: " << client_ip << std::endl;
				setPollfd(client, clients_numbers);
				Client newClient(client);
				newClient.setClientIp(client_ip);
				ClientsMap.insert(std::pair<int, Client>(clientSocket, newClient));
				clients_numbers++;
				// std::string response = "251 * MG Welcome to our IRC server, please enter nickname, username and password\n";
				std::string response = ":e2r8p3.1337.ma 251 * :Welcome to the FT_IRC server! Enjoy your chat.\n";
				send(clientSocket, response.c_str(), response.length(), 0);
				//check if is autonticated
			}
			//Grap diconnected clients
			for (int i = 1; i < clients_numbers; i++) {
				if (_pfds[i].revents & POLLIN) {
					char buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					int readed = recv(_pfds[i].fd, buffer, 1024, 0);
					buffer[readed] = '\0';
					if (readed < 0) {
						throw recvException();
					} else if (readed == 0) {
						if (_pfds[i].revents & POLLHUP)
						{
							std::cout << "POLLHUP" << std::endl;
							buffer[5] = '\0';
							std::string message(buffer);
							std::cout << "message: " << message << std::endl;
							std::cout << "client disconnected: " << ClientsMap[_pfds[i].fd].getNickname() << std::endl;
							std::cout << "buffer: [" << buffer << "]" << std::endl;
							ClientsMap.erase(_pfds[i].fd);
							close(_pfds[i].fd);
							_pfds[i].fd = -1;
							_pfds[i].events = 0;
							_pfds[i].revents = 0;
							// clients_numbers--;
						}
					} else if (readed >= 0) {
						buffer[readed] = '\0';
						std::cout << "\n==> received: [" << buffer << "]" << std::endl;
						std::string message(buffer);
						//Command
						size_t pos = message.find(" ");
						std::string command = message.substr(0, pos);
						command = command.substr(0, command.find("\n"));
						command = command.substr(0, command.find("\r"));
						//Params
						std::string params;
						if (pos != std::string::npos)
						 	params = message.substr(pos + 1);
						else
							params = "";
						params = params.substr(0, params.find("\n"));
						params = params.substr(0, params.find("\r"));
						std::cout << "command: [" << command << "]" << std::endl;
						std::cout << "params: [" << params << "]" << std::endl;
						// split params userName 0 * NickName
						if ((command == "PASS" || command == "PASS\n") && !ClientsMap[_pfds[i].fd].getIsAutonticated()) {
							std::string password = params.substr(0, params.find("\r"));
							std::cout << "password: " << password << std::endl;
							if (password == _password) {
								ClientsMap[_pfds[i].fd].setCorrectPassWord(true);
								ClientsMap[_pfds[i].fd].setIsAutonticated();
								if (ClientsMap[_pfds[i].fd].getIsAutonticated())
								{
									std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :Welcome to the Internet Relay Network " + ClientsMap[_pfds[i].fd].getNickname() + "\n";
									std::cout << "response: " << response << std::endl;
									send(_pfds[i].fd, response.c_str(), response.length(), 0);
								}
							} else {
								std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :Password incorrect\n";
								std::cout << "response: " << response << std::endl;
								send(_pfds[i].fd, response.c_str(), response.length(), 0);
							}
						} else if (command == "PONG") {
							std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :PING\n";
							std::cout << "response: " << response << std::endl;
							send(_pfds[i].fd, response.c_str(), response.length(), 0);
						}
						else if (command == "NICK") {
							handleNickCommand(params, i);
						} else if (command == "USER") {
							handleUserCommand(params, i);
						} else if (command == "QUIT\n" || command == "QUIT") {
							handleQuitCommand(i, clients_numbers);
						} else if (command == "GET") {
								std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, Welcome to my web server!</h1></body></html>\n";
								std::cout << "response: " << response << std::endl;
								send(_pfds[i].fd, response.c_str(), response.length(), 0);
						}
						else if (ClientsMap[_pfds[i].fd].getIsAutonticated())
						{
							if (command == "PASS" || command == "PASS\n") {
								std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": You are already registered with a password\n";
								std::cout << "response: " << response << std::endl;
								send(_pfds[i].fd, response.c_str(), response.length(), 0);
							} else if (command == "PRIVMSG") {
								handlePrivMsg(params, i,channelsV);
							} else if (command == "LIST" || command == "LIST\n") {
								handleListCommand(i, clients_numbers);
							} else if (command == "JOIN" || command == "JOIN\n") {
								handleJoinCommand(params, i, channelsV, _pfds);
							} else {
								std::string response = "ERROR Unknown command: " + command + "\n";
								std::cout << "response: " << response << std::endl;
								send(_pfds[i].fd, response.c_str(), response.length(), 0);
							}
						} else {
							std::string response = "ERROR you need to be autonticated first\n";
							if (ClientsMap[_pfds[i].fd].getNickname().empty())
								response += "ERROR you need to set a nickname !\n";
							if (ClientsMap[_pfds[i].fd].getUserName().empty())
								response += "ERROR you need to set a username !\n";
							if (ClientsMap[_pfds[i].fd].getCorrectPassWord() == false)
								response += "ERROR you need to enter the server password !\n";
							std::cout << "response: " << response << std::endl;
							send(_pfds[i].fd, response.c_str(), response.length(), 0);
						}
					}
				}
			}
		}
	}
};

#endif
