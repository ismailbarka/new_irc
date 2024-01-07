#include "Server.hpp"

bool isOperator(int _fd, std::vector<Client> operators)
{
	std::vector<Client>::iterator it = operators.begin();
	while(it != operators.end())
	{
		if(it->getfd() == _fd)
			return true;
		it++;
	}
	return false;
}

void Server::handleKickCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
	{
		// std::cout << params << std::endl;
		std::stringstream iss(params);
		std::string channelName;
		std::string userName;
		iss >> channelName;
		iss >> userName;
		// std::cout << "||" << channelName << "||" << std::endl;
		std::map<std::string, Channels>::iterator channelIt = channelsV.find(channelName);
		if(channelIt == channelsV.end())
		{
			std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() +  " channel not found\r\n";
			send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
			// std::cout << "channel not found\n";
		}
		else
		{
			int client_fd = nameTofd(userName);
			if(client_fd == -1)
			{
				std::string reponse = "476 " + ClientsMap[_pfds[i].fd].getNickname() +  " the user is not in the channel\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				// std::cout << "the user is not in the channel\n";
				return;
			}
			// std::cout << "client_fd = " << client_fd << std::endl;
			// std::cout << "*channelIt->second.clientsFd.begin() = " << *channelIt->second.clientsFd.begin() << std::endl;
			if(_pfds[i].fd != *channelIt->second.clientsFd.begin() && isOperator(_pfds[i].fd, channelIt->second.operators) == false)
			{
				std::string reponse = "442 " + ClientsMap[_pfds[i].fd].getNickname() + " you are not channel operator\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				// std::cout << "the user is not an admin\n";
				return;
			}

			std::vector<int>::iterator it = std::find(channelIt->second.clientsFd.begin(), channelIt->second.clientsFd.end(), client_fd);
			if (it != channelIt->second.clientsFd.end()) {
				channelIt->second.clientsFd.erase(it);
				std::string reponse = ":" + ClientsMap[_pfds[i].fd].getNickname() + " KICK " + channelName + " " + userName + "\r\n";// need to edit this message
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				send(client_fd, reponse.c_str(), reponse.length(), 0);
				// std::cout << "kicked!\n";
			} else {
				std::string reponse = "476 " + ClientsMap[_pfds[i].fd].getNickname() + " the user is not in the channel\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				// std::cout << "Element not found in channel client" << std::endl;
				return ;

			}
		}
		std::string line_;
		while(iss >> line_)
		{
			// std::cout << "line = " << line_ << std::endl;
		}
	}