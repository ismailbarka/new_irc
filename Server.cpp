/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 15:51:50 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 14:43:35 by tmoumni          ###   ########.fr       */
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
	close(serverSocket);
	std::cout << "Server destroyed" << std::endl;
}

Server::Server(char **av)
{
	if (!validNumber(av[1]) || std::atoi(av[1]) <= 0 || std::atoi(av[1]) > 65535)
		throw std::string("Invalid port number");
	std::stringstream ss(av[2]);
	std::string password;
	std::getline(ss, password, '\n');
	if (password.size() < 5 || password.empty() || password.size() > 10)
		throw std::string("Invalid password, password should be between 5 and 10 characters");
	_password = av[2];
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		throw socketException();
	int tmp = 1;
	int nRet = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	if (!nRet) {
		
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
		response += "375 " + ClientsMap[_pfds[i].fd].getNickname() + " :- You are now logged in as [ " + ClientsMap[_pfds[i].fd].getNickname() + " ]\r\n";
		response += "375 " + ClientsMap[_pfds[i].fd].getNickname() + " :- Your host is " + ClientsMap[_pfds[i].fd].getClientHost() + ", running version 1.0\r\n";
		response += "375 " + ClientsMap[_pfds[i].fd].getNickname() + " :- Your IP is " + std::string(ClientsMap[_pfds[i].fd].ipAddress) + "\r\n";
		response += "375 " + ClientsMap[_pfds[i].fd].getNickname() + " :- This server was created in 2023-12-15\r\n";
		response += "375 " + ClientsMap[_pfds[i].fd].getNickname() + " :- There are " + std::to_string(ClientsMap.size()) + " user(s) and " + std::to_string(channelsV.size()) + " channel(s) on this server.\r\n";
		response += "376 " + ClientsMap[_pfds[i].fd].getNickname() + " :End of /MOTD.\r\n";
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

void Server::welcomeNewClient(int & clients_numbers)
{
	char hostname[256];
	char client_ip[INET_ADDRSTRLEN];

	std::cout << YELLOW << "New client connected" RESET << std::endl;
	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	int clientSocket = accept(serverSocket, (struct sockaddr *)&client_addr, &client_addr_size);
	if (clientSocket < 0)
		throw acceptException();
	int nFl = fcntl(clientSocket, F_SETFL, O_NONBLOCK); 
	if (nFl < 0) {
		std::cout << "failed to set socket to non-blocking mode\n";
		throw fcntlException();
	}
	struct pollfd client;
	client.fd = clientSocket;
	client.events = POLLIN | POLLHUP;
	client.revents = 0;
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	std::cout << "Client IP: " << BLUE << client_ip << RESET << std::endl;

	if (gethostname(hostname, sizeof(hostname)) == 0) {
		std::cout << "Hostname: " << BLUE << hostname << RESET << std::endl;
	} else {
		std::cerr << "Error getting hostname" << std::endl;
	}

	setPollfd(client, clients_numbers);
	Client newClient(client);
	newClient.setClientIp(client_ip);
	newClient.setClientHost(hostname);
	ClientsMap.insert(std::pair<int, Client>(clientSocket, newClient));
	clients_numbers++;

	std::string art  = "NOTICE * :███╗   ███╗██╗███╗   ██╗██╗        ██╗██████╗  ██████╗ \r\n";
				art += "NOTICE * :████╗ ████║██║████╗  ██║██║        ██║██╔══██╗██╔════╝ \r\n";
				art += "NOTICE * :██╔████╔██║██║██╔██╗ ██║██║███████╗██║██████╔╝██║      \r\n";
				art += "NOTICE * :██║╚██╔╝██║██║██║╚██╗██║██║╚══════╝██║██╔══██╗██║      \r\n";
				art += "NOTICE * :██║ ╚═╝ ██║██║██║ ╚████║██║        ██║██║  ██║╚██████╗ \r\n";
				art += "NOTICE * :╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝        ╚═╝╚═╝  ╚═╝ ╚═════╝ \r\n";
				art += "NOTICE * :[~[ Welcome to the FT_IRC server! Enjoy your chat. ]~] \r\n";
	std::cout << GREEN << art << RESET << std::endl;
	send(clientSocket, art.c_str(), art.length(), 0);
}

void Server::startServer()
{
	std::string line;
	bindSocket();
	listenSocket();
	setPollfd();
	int clients_numbers = 1;
	while (1)
	{

		int pollResult = poll(_pfds, clients_numbers, -1);

		if (pollResult == -1) {
			throw pollException();
		}
		if (_pfds[0].revents & POLLIN) { 
			welcomeNewClient(clients_numbers);
		}
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
						handleQuitCommand(i, clients_numbers, "Client Quit");
				} else if (readed > 0) {
					buffer[readed] = '\0';
					line += buffer;
					if (line.find('\n') != std::string::npos)
					{
						std::stringstream ss(line);
						while (std::getline(ss, line, '\n'))
						{
							line = line.substr(0, line.find("\r"));
							size_t pos = line.find(" ");
							std::string command = line.substr(0, pos);
							if (command.empty())
								continue;
							std::string params;
							if (pos != std::string::npos)
								params = line.substr(pos + 1);
							params = trimString(params);
							if ((command == "PASS" || command == "PASS\n") && !ClientsMap[_pfds[i].fd].getIsAutonticated()) {
								handlePassCommand(params, i);
							} else if (command == "NICK") {
								handleNickCommand(params, i);
							} else if (command == "USER") {
								handleUserCommand(params, i);
							} else if (command == "QUIT" || command == "QUIT\n") {
								handleQuitCommand(i, clients_numbers, params);
							} else if (ClientsMap[_pfds[i].fd].getIsAutonticated()) {
								authCommand(command, params, i);
							} else {
								nonAuthCommand(i);
							}
							line.clear();
						}
					} else {
						std::cout << RED << "line=> " << buffer << RESET << std::endl;
					}
				}
			}
		}
	}
}