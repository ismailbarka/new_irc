#include "Server.hpp"

void Server::handleTopicCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
{
    // std::cout << "Topic params: " << params << std::endl;
    std::stringstream iss(params);
    std::string channelName;
    std::string newTopic;
	std::string tmp;
    std::getline(iss, channelName, ' ');
	while (std::getline(iss, tmp, ' ')){
		newTopic += tmp;
		newTopic += " ";
	}
    std::map<std::string, Channels>::iterator it = channelsV.find(channelName);
    if (it == channelsV.end())
    {
        std::string response = "403 " + ClientsMap[_pfds[i].fd].getNickname() + " :" + channelName +" No such channel\r\n";
        send(_pfds[i].fd, response.c_str(), response.length(), 0);
        // std::cout << "no channel with this name : " << channelName << "\n";
        return ;
    }
	if (newTopic.size())
	{
		if (newTopic[0] == ':')
			newTopic.erase(0, 1);
		if (newTopic.size() > 80)
		{
			std::string reponse = "Topic is too long\r\n";
			send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
			// std::cout << "Topic is too long\n";
			return ;
		}
		//check if the client is the operator of the channel
		std::vector<Client>::iterator itOp = it->second.operators.begin();
		while (itOp != it->second.operators.end())
		{
			if (itOp->getNickname() == ClientsMap[_pfds[i].fd].getNickname())
				break;
			itOp++;
		}
		if (itOp == it->second.operators.end() && it->second.mode.find('t') != std::string::npos)
		{
			std::string response = "482 " + ClientsMap[_pfds[i].fd].getNickname() + " " + channelName + " :You're not channel operator\r\n";
			send(_pfds[i].fd, response.c_str(), response.length(), 0);
			// std::cout << "the client is not the operator of the channel\n";
			return ;
		}
        it->second.topic = newTopic;
		std::string topicResponse = ":" + ClientsMap[_pfds[i].fd].getNickname() + " TOPIC " + channelName + " :" + it->second.topic + "\r\n";
		send(_pfds[i].fd, topicResponse.c_str(), topicResponse.length(), 0);
	}
	std::string topicResponse = "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + channelName + " :" + it->second.topic + "\r\n";
	// std::cout << "topicResponse: " << topicResponse << std::endl;
	send(_pfds[i].fd, topicResponse.c_str(), topicResponse.length(), 0);
}