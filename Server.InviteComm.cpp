#include "Server.hpp"

void Server::handleInviteCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
{
    std::stringstream iss(params);
    std::string channelName;
    std::string nickName;
    std::string linee;
    iss >> nickName;
    iss >> channelName;
    if(iss >> linee)
    {
        std::cout << "error arguments\n";
    }
    std::map<std::string, Channels>::iterator channelIt = channelsV.find(channelName);
    if (channelIt == channelsV.end())
    {
        std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() + " :" + channelName + " channel not found\r\n";
        send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
    }
    else
    {
        int client_fd = nameTofd(nickName);
        if (client_fd == -1)
        {
            std::string reponse = "476 " + ClientsMap[_pfds[i].fd].getNickname() +  " the user is not in the channel-Server\r\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            return;
        }
        if (_pfds[i].fd != *channelIt->second.clientsFd.begin())
        {
            std::string reponse = "482 " + ClientsMap[_pfds[i].fd].getNickname() + " :You're not channel operator\r\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            return;
        }
        std::vector<int>::iterator it = std::find(channelIt->second.clientsFd.begin(), channelIt->second.clientsFd.end(), client_fd);
        if (it != channelIt->second.clientsFd.end()) {
            std::string reponse = "443 " + ClientsMap[_pfds[i].fd].getNickname() + " the user already in the channel\r\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            return;
        }
        else
        {
            std::string reponse = "341 " + ClientsMap[_pfds[i].fd].getNickname() + " " + nickName + " " + channelName + "\r\n";
			send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
			reponse = ":" + ClientsMap[_pfds[i].fd].getNickname() + " INVITE " + nickName + " " + channelName + "\r\n";
			send(client_fd, reponse.c_str(), reponse.length(), 0);
			if (channelIt->second.mode.find('i') != std::string::npos)
				channelIt->second.invited.push_back(ClientsMap[client_fd]);
        }
    }
}