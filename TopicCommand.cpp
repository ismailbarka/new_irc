#include "Server.hpp"

void Server::handleTopicCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
{
    std::cout << "testt\n";
    std::stringstream iss(params);
    std::string channelName;
    std::string newTopic;
    iss >> channelName;
    std::map<std::string, Channels>::iterator it = channelsV.find(channelName);
    if(iss >> newTopic)
    {
        if(it == channelsV.end())
        {
            std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() +  " channel not found\n\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            std::cout << "no channel with this name\n";
            return ;
        }
        it->second.topic = newTopic;
        handlePrivMsg(channelName + " " + "the new topic is " + it->second.topic, i, channelsV);
        return ;
    }
    if(it == channelsV.end())
    {
        std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() +  " channel not found\n\n";
        send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
        std::cout << "no channel with this name\n";
        return ;
    }

    handlePrivMsg(ClientsMap[_pfds[i].fd].getNickname() + " " + "the current topic is " + it->second.topic, i, channelsV);
    // send(_pfds[i].fd, it->second.topic )
}