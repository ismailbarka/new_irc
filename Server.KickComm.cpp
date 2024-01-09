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

void sendMsgToMembers(std::string msg, std::vector<int> clients)
{
	std::vector<int>::iterator it = clients.begin();
	while (it != clients.end())
	{
		send(*it, msg.c_str(), msg.length(), 0);
		it++;
	}
}

void Server::handleKickCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
	{
		std::stringstream iss(params);
		std::string channelName;
		std::string userName;
		iss >> channelName;
		iss >> userName;
		std::map<std::string, Channels>::iterator channelIt = channelsV.find(channelName);
		if(channelIt == channelsV.end())
		{
			std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() +  " channel not found\r\n";
			send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
		}
		else
		{
			int client_fd = nameTofd(userName);
			if(client_fd == -1)
			{
				std::string reponse = "476 " + ClientsMap[_pfds[i].fd].getNickname() +  " the user is not in the channel\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				return;
			}
			if(_pfds[i].fd != *channelIt->second.clientsFd.begin() && isOperator(_pfds[i].fd, channelIt->second.operators) == false)
			{
				std::string reponse = "442 " + ClientsMap[_pfds[i].fd].getNickname() + " you are not channel operator\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				return;
			}

			std::vector<int>::iterator it = std::find(channelIt->second.clientsFd.begin(), channelIt->second.clientsFd.end(), client_fd);
			if (it != channelIt->second.clientsFd.end()) {
				channelIt->second.clientsFd.erase(it);
				std::string line_;
				std::string msgg;
				bool check = false;
				while(iss >> line_)
				{
					check = true;
					msgg += " " + line_;
				}
				std::string reponse = ":" + ClientsMap[_pfds[i].fd].getNickname() + " KICK " + channelName + " " + userName + msgg + "\r\n";
				sendMsgToMembers(reponse, channelIt->second.clientsFd);
				send(client_fd, reponse.c_str(), reponse.length(), 0);
			} else {
				std::string reponse = "476 " + ClientsMap[_pfds[i].fd].getNickname() + " the user is not in the channel\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				return ;
			}
		}
	}