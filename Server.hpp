/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:05:37 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/15 17:42:38 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

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
	struct sockaddr_in server_adr;
	std::map<int, Client> ClientsMap;
public:
	Server() {
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
			throw std::exception();
		int tmp = 1;
		int nRet = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
		if (!nRet) {
			std::cout << "nRet success\n";
		} else {
			std::cout << "nRet failed\n";
		}
		int nFl = fcntl(serverSocket, F_SETFL, O_NONBLOCK);
		if (nFl < 0) {
			std::cout << "failed to set socket to non-blocking mode\n";
		} else {
			std::cout << "socket to non-blocking mode\n";
		}
		memset(&server_adr, '\0', sizeof(server_adr));
		server_adr.sin_family = AF_INET;
		server_adr.sin_port = htons(5000);
	}
	void bindSocket()
	{
		int bCheck = bind(serverSocket, (struct sockaddr *)&server_adr, sizeof(server_adr));
		if (bCheck < 0)
			throw std::exception();
	}
	void listenSocket()
	{
		if (listen(serverSocket, 128) < 0)
			throw std::exception();
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

	void handleNickCommand(std::string params, int i)
	{
		std::string nickName = params.substr(0, params.find(" "));
		std::cout << "nickName: " << nickName << std::endl;
		ClientsMap[_pfds[i].fd].setNickname(nickName);
		ClientsMap[_pfds[i].fd].setIsAutonticated();
		std::cout << "isAutonticated: " << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
	}

	void handleUserCommand(std::string params, int i)
	{
		std::string userName = params.substr(0, params.find(" "));
		std::cout << "userName: " << userName << std::endl;
		ClientsMap[_pfds[i].fd].setuserName(userName);
		ClientsMap[_pfds[i].fd].setIsAutonticated();
		std::cout << "isAutonticated: " << ClientsMap[_pfds[i].fd].getIsAutonticated() << std::endl;
	}

	void handleQuitCommand(int i, int & clients_numbers)
	{
		std::cout << "client disconnected\n";
		close(_pfds[i].fd);
		_pfds[i].fd = -1;
		_pfds[i].events = 0;
		_pfds[i].revents = 0;
		clients_numbers--;
		ClientsMap.erase(_pfds[i].fd);
	}

	void handleListCommand(int i, int clients_numbers)
	{
		std::map<int, Client>::iterator it;
		std::cout << "clients_numbers: " << clients_numbers << std::endl;
		for (it = ClientsMap.begin(); it != ClientsMap.end(); it++) {
			if (it->second.getIsAutonticated()) {
				std::string response = it->second.getNickname() + "\n";
				std::cout << "response: " << response << std::endl;
				send(_pfds[i].fd, response.c_str(), response.length(), 0);
			}
		}
	}

	void handlePrivMsg(std::string params, int i)
	{
		std::string target = params.substr(0, params.find(" "));
		std::string message = params.substr(params.find(" ") + 1);
		std::cout << "target: " << target << std::endl;
		std::cout << "message: " << message << std::endl;
		if (target[0] == '#') {
			std::map<int, Client>::iterator it;
			for (it = ClientsMap.begin(); it != ClientsMap.end(); it++) {
				if (it->second.getIsAutonticated() && it->second.getNickname() != ClientsMap[_pfds[i].fd].getNickname()) {
					std::string response = "Message from [" + ClientsMap[_pfds[i].fd].getNickname();
					response += "@" + ClientsMap[_pfds[i].fd].getNickname() + "]: " + message + "\n";
					std::cout << "response: " << response << std::endl;
					send(it->first, response.c_str(), response.length(), 0);
				}
			}
		} else {
			std::map<int, Client>::iterator it;
			for (it = ClientsMap.begin(); it != ClientsMap.end(); it++) {
				if (it->second.getIsAutonticated() && it->second.getNickname() == target) {
					std::string response = "Message from [" + ClientsMap[_pfds[i].fd].getuserName();
					response += "@" + ClientsMap[_pfds[i].fd].getNickname() + "]: " + message + "\n";
					std::cout << "response: " << response << std::endl;
					send(it->first, response.c_str(), response.length(), 0);
				}
			}
		}
	}

	void startServer()
	{
		bindSocket();
		listenSocket();
		setPollfd();
		int clients_numbers = 1;
		while (1) {
			int pollResult = poll(_pfds, clients_numbers, -1);
			if (pollResult == -1) {
				perror("poll error");
			}
			if (_pfds[0].revents == POLLIN) {
				std::cout << "new client connected\n";
				struct sockaddr_in client_adr;
				socklen_t client_adr_size = sizeof(client_adr);
				int clientSocket = accept(serverSocket, (struct sockaddr *)&client_adr, &client_adr_size);
				if (clientSocket < 0)
					throw std::exception();
				int nFl = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
				if (nFl < 0) {
					std::cout << "failed to set socket to non-blocking mode\n";
				} else {
					std::cout << "socket to non-blocking mode\n";
				}
				struct pollfd client;
				client.fd = clientSocket;
				client.events = POLLIN;
				client.revents = 0;
				setPollfd(client, clients_numbers);
				Client newClient(client);
				ClientsMap.insert(std::pair<int, Client>(clientSocket, newClient));
				clients_numbers++;
			}
			for (int i = 1; i < clients_numbers; i++) {
				if (_pfds[i].revents == POLLIN) {
					char buffer[1024];
					memset(buffer, '\0', 1024);
					int readed = recv(_pfds[i].fd, buffer, 1024, 0);
					if (readed < 0)
						throw std::exception();
					if (readed == 0) {
						std::cout << "client disconnected\n";
						close(_pfds[i].fd);
						_pfds[i].fd = -1;
						_pfds[i].events = 0;
						_pfds[i].revents = 0;
						clients_numbers--;
					} else {
						std::cout << "received: " << buffer << std::endl;
						std::string message(buffer);
						std::string command = message.substr(0, message.find(" "));
						std::string params = message.substr(message.find(" ") + 1);
						params = params.substr(0, params.find("\n"));
						std::cout << "command: " << command << std::endl;
						std::cout << "params: " << params << std::endl;
						if (command == "NICK") {
							handleNickCommand(params, i);
						} else if (command == "USER") {
							handleUserCommand(params, i);
						} else if (command == "QUIT\n") {
							handleQuitCommand(i, clients_numbers);
						} else if (command == "PRIVMSG") {
							handlePrivMsg(params, i);
						} else if (command == "LIST\n") {
							handleListCommand(i, clients_numbers);
						} else {
							std::string response = command.substr(0, command.find("\n")) + " " + "Unknown command" + "\n";
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
