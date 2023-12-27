#include "Server.hpp"

void Server::handleInviteCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
{
    std::stringstream iss(params);
    std::string channelName;
    std::string nickName;
    std::string linee;
    iss >> nickName;
    iss >> channelName;
	std::cout << "---handleInviteCommand---" << std::endl;
	std::cout << "nickName: " << nickName << std::endl;
	std::cout << "channelName: " << channelName << std::endl;
    if(iss >> linee)
    {
        std::cout << "error arguments\n";
    }
    std::map<std::string, Channels>::iterator channelIt = channelsV.find(channelName);
    if (channelIt == channelsV.end())
    {
        std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() +  " channel not found\n\n";
        send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
        std::cout << "channel not found\n";
    }
    else
    {
        int client_fd = nameTofd(nickName);
        if (client_fd == -1)
        {
            std::string reponse = "476 " + ClientsMap[_pfds[i].fd].getNickname() +  " the user is not in the channel-Server\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            std::cout << "the user is not in the channel-Server\n";
            return;
        }
        if (_pfds[i].fd != *channelIt->second.clientsFd.begin())
        {
            std::string reponse = "482 " + ClientsMap[_pfds[i].fd].getNickname() + " you are not an admin\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            std::cout << "the user is not an admin\n";
            return;
        }
        std::vector<int>::iterator it = std::find(channelIt->second.clientsFd.begin(), channelIt->second.clientsFd.end(), client_fd);
        if (it != channelIt->second.clientsFd.end()) {
            std::string reponse = "443 " + ClientsMap[_pfds[i].fd].getNickname() + " the user already in the channel\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            std::cout << "the user already in the channel\n";
            return;
        }
        else
        {
            std::string reponse = "341 " + ClientsMap[_pfds[i].fd].getNickname() + " " + nickName + " " + channelName + "\n";
			std::cout << "reponse: " << reponse << std::endl;
			send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
			reponse = ":" + ClientsMap[_pfds[i].fd].getNickname() + " INVITE " + nickName + " " + channelName + "\n";
			std::cout << "reponse: " << reponse << std::endl;
			send(client_fd, reponse.c_str(), reponse.length(), 0);
            std::cout << "add success\n";
			if (channelIt->second.mode == "+i")
				channelIt->second.invited.push_back(ClientsMap[client_fd]);
			else
				channelIt->second.addClient(client_fd);
        }
    }
}