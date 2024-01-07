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

void Server:: firstJoin(std::string key, std::string value, bool pass, int i, std::map<std::string, Channels> &channelsV)
{
	Channels channel;
	channel.havePassword = pass;
	channel.password = value;
	channel.addClient(_pfds[i].fd);
	channel.operators.push_back(ClientsMap[_pfds[i].fd]);
	channelsV.insert(std::pair<std::string, Channels>(key, channel));
	std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN " + key + "\n";
	// std::cout << "response: " << resp;
	resp += ":" + std::string(ClientsMap[_pfds[i].fd].ipAddress) + " MODE " + key + " +o " + ClientsMap[_pfds[i].fd].getNickname() + "\r\n";
	std::vector<int>::iterator it = channel.clientsFd.begin();
	while (it != channel.clientsFd.end())
	{
		send(*it, resp.c_str(), resp.length(), 0);
		it++;
	}
	resp = "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully Joined " + key + "\r\n";
	resp += "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + channel.topic + "\r\n";
	send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
}

void Server::joinChannel(std::string key, int i, std::map<std::string, Channels> &channelsV, std::string value)
{
	std::map<std::string,Channels>::iterator it = channelsV.find(key);
	std::vector<Client>::iterator it1 = it->second.invited.begin();
	while (it1 != it->second.invited.end() && it1->getNickname() != ClientsMap[_pfds[i].fd].getNickname())
	{
		it1++;
	}
	if (it->second.mode.find("i") != std::string::npos && it1 == it->second.invited.end())
	{
		//tell him that he can't join this channel because it's invite only
		std::string resp = "473 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :Cannot join channel (+i)\r\n";
		// std::cout << "response: " << resp;
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	if (it->second.mode.find("l") != std::string::npos && (int)(it->second.clientsFd.size()) >= std::atoi(it->second.limit.c_str()))
	{
		//tell him that he can't join this channel because it's invite only
		std::string resp = "471 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :Cannot join channel (+l)\r\n";
		// std::cout << "response: " << resp;
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	//check if the channel have a password
	if (it->second.havePassword == true && it->second.password != value)
	{
		//tell him that he can't join this channel because it's invite only
		std::string resp = "475 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :Cannot join channel (incorrect password)\r\n";
		// std::cout << "response: " << resp;
		send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
		return;
	}
	if (key[0] != '#')
		key = "#" + key;
	std::string resp2 = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
	send(_pfds[i].fd, resp2.c_str(), resp2.length(), 0);
	it->second.addClient(_pfds[i].fd);
	//inform all the clients in the channel that this client joined
	std::vector<int>::iterator it2 = it->second.clientsFd.begin();
	while (it2 != it->second.clientsFd.end())
	{
		std::vector<int>::iterator it3 = it->second.clientsFd.begin();
		while (it3 != it->second.clientsFd.end())
		{
			if (*it2 != *it3)
			{
				std::string resp = ":" + ClientsMap[*it3].getNickname() + "!~" + ClientsMap[*it3].getUserName() + "@" + ClientsMap[*it3].ipAddress + " JOIN :" + key + "\r\n";
				if (it->second.isOperator(*it3))
					resp += ":" + std::string(ClientsMap[*it3].ipAddress) + " MODE " + key + " +o " + ClientsMap[*it3].getNickname() + "\r\n";
				send(*it2, resp.c_str(), resp.length(), 0);
			}
			it3++;
		}
		// send(*it2, resp2.c_str(), resp2.length(), 0);
		it2++;
	}
	resp2 = "332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + it->second.topic + "\r\n";
	resp2 += "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully Joined " + key + "\r\n";						
	send(_pfds[i].fd, resp2.c_str(), resp2.length(), 0);
	if (it1 != it->second.invited.end())
		it->second.invited.erase(it1);
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
		// std::cout << "keys = " << keys << std::endl;
		// std::cout << "values = " << values << std::endl;
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
					firstJoin(key, value, true, i, channelsV);
				}
				else
				{
					std::vector<int>::iterator it1 = it->second.clientsFd.begin();
					while (it1 != it->second.clientsFd.end() && *it1 != _pfds[i].fd)
						it1++;
					if (it1 != it->second.clientsFd.end())
					{
						std::string resp = "443 " + ClientsMap[_pfds[i].fd].getNickname() + " You have already Joined " + key + "\r\n";
			 			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
					} else {
						if (value != it->second.password) {
							std::string resp = "475 " + ClientsMap[_pfds[i].fd].getNickname() + " Incorrect password " + "\r\n";
							send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
							// std::cout << "Incorrect password\n";
						} else {
							joinChannel(key, i, channelsV, value);
						}
					}
				}
			}
			else // there is no password for the channel
			{
				std::map<std::string,Channels>::iterator it = channelsV.find(key);
				if (it == channelsV.end()) //no channel with this name and don't have a password
				{
					firstJoin(key, value, false, i, channelsV);
				}
				else
				{
					std::vector<int>::iterator it1 = it->second.clientsFd.begin();
					while (it1 != it->second.clientsFd.end() && *it1 != _pfds[i].fd)
						it1++;
					if (it1 != it->second.clientsFd.end())
					{
						std::string resp = "443 " + ClientsMap[_pfds[i].fd].getNickname() + " You have already Joined " + key + "\r\n";
			 			send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
					} else {
						joinChannel(key, i, channelsV, value);
					}
				}
			}
		} 
	}