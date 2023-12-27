#include "Server.hpp"
#include "Client.hpp"

bool valideParams(std::string keys, std::string values)
{
    int count_camas_keys = 0;
    int count_camas_values = 0;
    if(keys[0] == ',' || values[0] == ',' || keys[keys.length() -1] == ','|| values[values.length() - 1] == ',')
    {
        return false;
    }
    int i = 0;
    while(keys[i])
    {
        if(keys[i] == ',')
            count_camas_keys++;
        if(i > 0 && keys[i] == ',' && keys[i - 1] == ',')
            return false;
        i++;
    }
    i = 0;
    while(values[i])
    {
        if(values[i] == ',')
            count_camas_values++;
        if(i > 0 && values[i] == ',' && values[i - 1] == ',')
            return false;
        i++;
    }
    if(count_camas_keys < count_camas_values)
        return false;
    return true;
}

void Server::handleJoinCommand(std::string params, int i, std::map<std::string, Channels> &channelsV,struct pollfd _pfds[])
	{
		std::stringstream iss(params);
		std::string keys;
		std::string values;
		std::string key;
		std::string value;
		std::getline(iss, keys, ' ');
		std::getline(iss, values);
		std::stringstream issKey(keys);
		std::stringstream issVlaue(values);
		std::cout << "keys = " << keys << std::endl;
		std::cout << "values = " << values << std::endl;
		if (valideParams(keys, values) == false)
		{
			std::cout << "error arguments\n";
			return;
		}
		while (std::getline(issKey, key, ','))
		{
			if (std::getline(issVlaue, value, ','))
			{
				std::map<std::string,Channels>::iterator it = channelsV.find(key);
				if (it == channelsV.end()) //there is no channel with this name and have password
				{
					if(key[0] != '#')
						key = "#" + key;
					std::string resp2 = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
					resp2 += "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + "Topic1" + "\r\n";
					send(_pfds[i].fd, resp2.c_str(), resp2.length(), 0);
					Channels newChannel(true);
					newChannel.addClient(_pfds[i].fd);
					if(key[0] != '#')
						key = "#" + key;
					newChannel.password = value;
					newChannel.operators.push_back(ClientsMap[_pfds[i].fd]);
					channelsV[key] = newChannel;
				}
				else
				{
					std::vector<int>::iterator it1 = it->second.clientsFd.begin();
					while (it1 != it->second.clientsFd.end() && *it1 != _pfds[i].fd)
						it1++;
					if (it1 != it->second.clientsFd.end())
					{
						std::string resp = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have already JOINED " + key + "\n";
			 			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
					} else {
						if (it->second.havePassword == true) {
						  	if (value == it->second.password)
						  	{
								std::vector<Client>::iterator it1 = it->second.invited.begin();
								while (it1 != it->second.invited.end() && it1->getNickname() != ClientsMap[_pfds[i].fd].getNickname())
								{
									it1++;
								}
								if (it->second.mode[0] == '+' && it->second.mode.find("i") != std::string::npos && it1 == it->second.invited.end())
								{
									//tell him that he can't join this channel because it's invite only
									std::string resp = "473 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :Cannot join channel (+i)\n";
									std::cout << "response: " << resp;
									send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
									return;
								}
								if (key[0] != '#')
									key = "#" + key;
								std::string resp2 = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
								resp2 += "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + "Topic2" + "\r\n";
								send(_pfds[i].fd, resp2.c_str(), resp2.length(), 0);
								it->second.addClient(_pfds[i].fd);
							} else {
								std::string resp = "475 " + ClientsMap[_pfds[i].fd].getNickname() + " incorrect password " + "\n";
								send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
								std::cout << "encorrect password\n";
						  	}
						}
						else
						{
							std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
							resp += "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + "Topic3" + "\r\n";
							send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
							it->second.addClient(_pfds[i].fd);
						}
					}
				}
			}
			else // there is no password for the channel
			{
				std::map<std::string,Channels>::iterator it = channelsV.find(key);
				if (it == channelsV.end()) //there is no channel with this name and don't have a password
				{
					if (key[0] != '#')
						key = "#" + key;
					std::string resp1 = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
					resp1 += "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + "Topic4" + "\r\n";
					send(_pfds[i].fd, resp1.c_str(), resp1.length(), 0);
					Channels newChannel(false);
					newChannel.addClient(_pfds[i].fd);
					newChannel.password = "0";
					newChannel.operators.push_back(ClientsMap[_pfds[i].fd]);
					channelsV[key] = newChannel;
					std::cout << "Client ip address: " << ClientsMap[_pfds[i].fd].ipAddress << "\n";
				}
				else
				{
					std::vector<int>::iterator it1 = it->second.clientsFd.begin();
					while (it1 != it->second.clientsFd.end() && *it1 != _pfds[i].fd)
					{
						it1++;
					}
					if (it1 != it->second.clientsFd.end())
					{
						std::string resp = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have already JOINED " + key + "\n";
			 			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		 				return;
					} else {
						if (it->second.havePassword == false) {
							std::vector<Client>::iterator it1 = it->second.invited.begin();
							while (it1 != it->second.invited.end() && it1->getNickname() != ClientsMap[_pfds[i].fd].getNickname())
							{
								it1++;
							}
							if (it->second.mode[0] == '+' && it->second.mode.find("i") != std::string::npos && it1 == it->second.invited.end())
							{
								//tell the client that he can't join this channel because it's invite only
								std::string resp = "473 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :Cannot join channel (+i)\n";
								std::cout << "response: " << resp;
								send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
								return;
							}
							std::cout << "channel mode: " << it->second.mode << std::endl;
							std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + " JOIN " + key + "\n";
							resp += "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully JOINED " + key + "\n";
							resp += "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + "Topic5" + "\n";
							send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
							it->second.addClient(_pfds[i].fd);
							if (it1 != it->second.invited.end())
								it->second.invited.erase(it1);
						} else {
							std::string resp = "475 " + ClientsMap[_pfds[i].fd].getNickname() + ": incorrect pass " + key + "\n";
							send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
							std::cout << "encorrect password\n";
							return ;
						}
					}
				}
			}
		} 
	}