#include "Server.hpp"

bool isOperator(int fd, std::vector<Client> operators)
{
    std::vector<Client>::iterator it = operators.begin();
    while (it != operators.end())
    {
        if(fd == it->getfd())
            return true;
        it++;
    }
    return false;
}

void Server::handleKickCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
	{
		std::string msg;
		std::cout << params << std::endl;
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
			std::cout << "client_fd = " << client_fd << std::endl;
			std::cout << "*channelIt->second.clientsFd.begin() = " << *channelIt->second.clientsFd.begin() << std::endl;
			if(isOperator(_pfds[i].fd, channelIt->second.operators) == false)
			{
				std::string reponse = "442 " + ClientsMap[_pfds[i].fd].getNickname() + " you are not an admin\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				return;
			}
			else if(client_fd == *channelIt->second.clientsFd.begin())
			{
				std::string reponse = "442 " + ClientsMap[_pfds[i].fd].getNickname() + " you cant kick the owner\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				return;
			}
			std::vector<int>::iterator it = std::find(channelIt->second.clientsFd.begin(), channelIt->second.clientsFd.end(), client_fd);
			if (it != channelIt->second.clientsFd.end()) {
                if(_pfds[i].fd == client_fd)
                    send(_pfds[i].fd, "Error : you cant kick your self\r\n", 33, 0);
                else if(client_fd == *channelIt->second.clientsFd.begin())
                    send(_pfds[i].fd, "Error: you cant kick the owner\r\n", 32, 0);
                else
                {
                    channelIt->second.clientsFd.erase(it);
					bool check = false;
					msg = ":" + ClientsMap[_pfds[i].fd].getNickname() + " KICK " + channelName + " " + userName + "\r\n";
                    send(_pfds[i].fd, msg.c_str(), msg.size(), 0);
					// msg = "Error : " + userName + " have been kicked from " + channelName + "\nREASON : ";
					std::string line_;
					while(iss >> line_)
					{
						check = true;
						msg += " ";
						msg+= line_;
					}

					if(check == false)
					{
						msg += " ";
						msg+= "no reason";
					}
					sendMagTomembers(msg, channelIt->second.clientsFd);
                }
			} else {
				std::string reponse = "476 " + ClientsMap[_pfds[i].fd].getNickname() + " the user is not in the channel\r\n";
				send(_pfds[i].fd, reponse.c_str(), reponse.length(), 0);
				return ;
			}
		}

		
	}