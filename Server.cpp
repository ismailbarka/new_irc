/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 15:51:50 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/01 16:54:11 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string trimString(const std::string& str) {
	size_t start = 0;
	size_t end = str.length();

	while (start < end && std::isspace(str[start]))
		++start;
	while (end > start && std::isspace(str[end - 1]))
		--end;
	return str.substr(start, end - start);
}

Server::Server()
{
}

Server::~Server()
{
	std::cout << "Server destroyed" << std::endl;
}

Server::Server(char **av)
{
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
		std::cout << GREEN << "socket on non-blocking mode" << RESET << std::endl;
	}
	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(atoi(av[1]));
}

void Server::welcomeMessage(int i)
{
	if (ClientsMap[_pfds[i].fd].getIsAutonticated())
	{
		std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :Welcome to the Internet Relay Network\n";
		response += "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You are now logged in as [ " + ClientsMap[_pfds[i].fd].getNickname() + " ]\r\n";
		// response += "002 " + ClientsMap[_pfds[i].fd].getNickname() + " :your host is " + std::string(hostname) + ", running version 1.0\n";
		std::cout << "response: " << response << std::endl;
		send(_pfds[i].fd, response.c_str(), response.length(), 0);
	}
}

void Server::bindSocket()
{
	int binded = bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (binded < 0)
		throw bindException();
}

void Server::listenSocket()
{
	int listening = listen(serverSocket, 128);
	if (listening < 0)
		throw listenException();
}

void Server::setPollfd()
{
	_pfds[0].fd = serverSocket;
	_pfds[0].events = POLLIN;
	_pfds[0].revents = 0;
}

void Server::setPollfd(struct pollfd _pfd)
{
	_pfds[0] = _pfd;
}

void Server::setPollfd(struct pollfd _pfd, int index)
{
	_pfds[index] = _pfd;
}

void Server::startServer()
{
	std::string message;
	bindSocket();
	listenSocket();
	setPollfd();
	int clients_numbers = 1;
	char hostname[256];
	while (1)
	{
		char client_ip[INET_ADDRSTRLEN];
		int pollResult = poll(_pfds, clients_numbers, -1);
		if (pollResult == -1) {
			throw pollException();
		}
		if (_pfds[0].revents & POLLIN) { // new client connected to server
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
				std::cout << "socket on non-blocking mode\n";
			}
			struct pollfd client;
			client.fd = clientSocket;
			client.events = POLLIN | POLLHUP;
			client.revents = 0;
			inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN); // ?
			std::cout << "client ip: " << client_ip << std::endl;
			//HOSTNAME
			if (gethostname(hostname, sizeof(hostname)) == 0) {
				std::cout << "Hostname: " << hostname << std::endl;
			} else {
				std::cerr << "Error getting hostname" << std::endl;
			}
			//HOSTNAME
			setPollfd(client, clients_numbers);
			Client newClient(client);
			newClient.setClientIp(client_ip);
			newClient.setClientHost(hostname);
			ClientsMap.insert(std::pair<int, Client>(clientSocket, newClient));
			clients_numbers++;
			// std::string response = "251 * MG Welcome to our IRC server, please enter nickname, username and password\n";
			//mini_irc ascii art
			std::string art  = "002 * :███╗   ███╗██╗███╗   ██╗██╗        ██╗██████╗  ██████╗ \r\n";
						art += "002 * :████╗ ████║██║████╗  ██║██║        ██║██╔══██╗██╔════╝ \r\n";
						art += "002 * :██╔████╔██║██║██╔██╗ ██║██║███████╗██║██████╔╝██║      \r\n";
						art += "002 * :██║╚██╔╝██║██║██║╚██╗██║██║╚══════╝██║██╔══██╗██║      \r\n";
						art += "002 * :██║ ╚═╝ ██║██║██║ ╚████║██║        ██║██║  ██║╚██████╗ \r\n";
						art += "002 * :╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝        ╚═╝╚═╝  ╚═╝ ╚═════╝ \r\n";
						art += "002 * :[~[ Welcome to the FT_IRC server! Enjoy your chat. ]~] \r\n";
			std::cout << art << std::endl;
			send(clientSocket, art.c_str(), art.length(), 0);
			//check if is autonticated
		}
		//Grap diconnected clients
		for (int i = 1; i < clients_numbers; i++) {
			if (_pfds[i].revents & POLLIN) {
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				int readed = recv(_pfds[i].fd, buffer, 1024, 0);
				if (readed < 0) {
					perror("recv Error");
					continue;
				} else if (readed == 0) {
					if (_pfds[i].revents & POLLHUP)
					{
						std::cout << "POLLHUP" << std::endl;
						buffer[5] = '\0';
						std::cout << "client disconnected: " << ClientsMap[_pfds[i].fd].getNickname() << std::endl;
						std::cout << "buffer: [" << buffer << "]" << std::endl;
						ClientsMap.erase(_pfds[i].fd);
						close(_pfds[i].fd);
						_pfds[i].fd = -1;
						_pfds[i].events = 0;
						_pfds[i].revents = 0;
						// clients_numbers--;
					}
				} else if (readed > 0) {
					buffer[readed] = '\0';
					std::cout << GREEN << "\n==> received: [" << buffer << "]" << RESET << std::endl;
					message += buffer;
					std::cout << BLUE << "1 =======> message: [" << message << "]" << RESET << std::endl;
					size_t checknewline = message.find('\n');
					if(checknewline != std::string::npos)
					{
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
						std::cout << BLUE << "2 =======> message: [" << message << "]" << RESET << std::endl;
						std::cout << RED << "params: [" << params << "]" << RESET << std::endl;
						params = params.substr(0, params.find("\n"));
						params = params.substr(0, params.find("\r"));
						std::cout << "command: [" << command << "]" << std::endl;
						std::cout << "params: [" << params << "]" << std::endl;
						if ((command == "PASS" || command == "PASS\n") && !ClientsMap[_pfds[i].fd].getIsAutonticated()) {
							std::string password = params.substr(0, params.find("\r"));
							std::cout << "password: " << password << std::endl;
							if (password == _password) {
								ClientsMap[_pfds[i].fd].setCorrectPassWord(true);
								ClientsMap[_pfds[i].fd].setIsAutonticated();
								welcomeMessage(i);
							} else {
								std::string response = "464 " + ClientsMap[_pfds[i].fd].getNickname() + " :Password incorrect\r\n";
								std::cout << "response: " << response << std::endl;
								send(_pfds[i].fd, response.c_str(), response.length(), 0);
							}
						} else if (command == "PONG") {
							std::string response = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " :PING\r\n";
							std::cout << "response: " << response << std::endl;
							send(_pfds[i].fd, response.c_str(), response.length(), 0);
						} else if (command == "NICK") {
							handleNickCommand(params, i);
						} else if (command == "USER") {
							handleUserCommand(params, i);
						} else if (command == "QUIT\n" || command == "QUIT") {
							handleQuitCommand(i, clients_numbers);
						} else if (ClientsMap[_pfds[i].fd].getIsAutonticated()) {
							if (command == "PASS" || command == "PASS\n") {
								std::string response = "ERROR " + ClientsMap[_pfds[i].fd].getNickname() + ": You are already registered with a password\r\n";
								std::cout << "response: " << response << std::endl;
								send(_pfds[i].fd, response.c_str(), response.length(), 0);
							} else if (command == "PRIVMSG") {
								handlePrivMsg(params, i,channelsV);
							} else if (command == "LIST" || command == "LIST\n") {
								handleListCommand(i, clients_numbers);
							} else if (command == "JOIN" || command == "JOIN\n") {
								handleJoinCommand(params, i, channelsV, _pfds);
							} else if (command == "INVITE" || command == "INVITE\n") {
								handleInviteCommand(params, i, channelsV, _pfds);
							} else if (command == "TOPIC" || command == "TOPIC\n") {
								handleTopicCommand(params, i, channelsV, _pfds);
							} else if (command == "KICK" || command == "KICK\n") {
								handleKickCommand(params, i, channelsV, _pfds);
							} else if (command == "MODE" || command == "MODE\n") {
								handleModeCommand(params, i, channelsV);
							} else if (command == "BOT" || command == "BOT\n") {
								handleBotCommand(params, i, _pfds);
							} else if (command == "PART" || command == "PART\n") {
								handlePartCommand(params, i, channelsV, _pfds);
							} else {
								std::string response = "421 " + ClientsMap[_pfds[i].fd].getNickname() + " :Unknown command: " + command + "\r\n";
								std::cout << "response: " << response << std::endl;
								send(_pfds[i].fd, response.c_str(), response.length(), 0);
							}
						} else {
							std::string response = "ERROR you need to be autonticated first\r\n";
							if (ClientsMap[_pfds[i].fd].getNickname().empty())
								response += "ERROR you need to set a nickname !\n";
							if (ClientsMap[_pfds[i].fd].getUserName().empty())
								response += "ERROR you need to set a username !\n";
							if (ClientsMap[_pfds[i].fd].getCorrectPassWord() == false)
								response += "ERROR you need to enter the server password !\r\n";
							std::cout << "response: " << response << std::endl;
							send(_pfds[i].fd, response.c_str(), response.length(), 0);
						}
						message = "";
					}
				}
			}
		}
	}
}