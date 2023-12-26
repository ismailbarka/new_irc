#include "Server.hpp"

void Server::handleTopicCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
{
    std::cout << "Topic params: " << params << std::endl;
    std::stringstream iss(params);
    std::string channelName;
    std::string newTopic;
    iss >> channelName;
    std::map<std::string, Channels>::iterator it = channelsV.find(channelName);
    if (iss >> newTopic)
    {
		//check if the client is the operator of the channel
		std::vector<Client>::iterator itOp = it->second.operators.begin();
		while (itOp != it->second.operators.end())
		{
			if (itOp->getNickname() == ClientsMap[_pfds[i].fd].getNickname())
				break;
			itOp++;
		}
		if (itOp == it->second.operators.end())
		{
			std::string reponse = "482 " + ClientsMap[_pfds[i].fd].getNickname() +  " you're not the channel operator\n\n";
			send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
			std::cout << "the client is not the operator of the channel\n";
			return ;
		}
        if (it == channelsV.end())
        {
            std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() +  " channel not found\n\n";
            send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
            std::cout << "no channel with this name: " << channelName << "\n";
            return ;
        }
        it->second.topic = newTopic;
        handlePrivMsg(channelName + " " + "the new topic is " + it->second.topic, i, channelsV);
        return ;
    }
    if (it == channelsV.end())
    {
        std::string reponse = "403 " + ClientsMap[_pfds[i].fd].getNickname() +  " channel not found\n\n";
        send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
        std::cout << "no channel with this name : " << channelName << "\n";
        return ;
    }
	std::string topicResponse = "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + channelName + " :" + it->second.topic + "\n";
	std::cout << "topicResponse: " << topicResponse << std::endl;
	send(_pfds[i].fd, topicResponse.c_str(), topicResponse.length(), 0);
}