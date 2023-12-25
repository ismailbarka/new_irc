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
		std::cout << "testtttt\n";
		while(std::getline(issKey, key, ','))
		{
			if(std::getline(issVlaue, value, ','))
			{
				std::map<std::string,Channels>::iterator it = channelsV.find(key);
				if(it == channelsV.end()) //there is a password
				{
					if(key[0] != '#')
						key = "#" + key;
					std::string resp1 = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
					send(_pfds[i].fd, resp1.c_str(), resp1.length(), 0);
					std::string resp2 = ":" + ClientsMap[_pfds[i].fd].getNickname() + " JOIN " + key + "\r\n";
					send(_pfds[i].fd, resp2.c_str(), resp2.length(), 0);
					std::string resp3 = ClientsMap[_pfds[i].fd]._client_host + " 353 " + ClientsMap[_pfds[i].fd].getNickname() + " = " + key + " :" + ClientsMap[_pfds[i].fd].getUserName() + "\r\n";
					send(_pfds[i].fd, resp3.c_str(), resp3.length(), 0);
					std::string resp4 = ClientsMap[_pfds[i].fd]._client_host + " 366 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :End of /NAMES list\r\n";
					send(_pfds[i].fd, resp4.c_str(), resp4.length(), 0);
					Channels newChannel(true);
					newChannel.addClient(_pfds[i].fd);
					if(key[0] != '#')
						key = "#" + key;
					newChannel.password = value;
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
					}else {
						if(it->second.havePassword == true && value == it->second.password)
						{
						if(key[0] != '#')
							key = "#" + key;
						std::string resp1 = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
						send(_pfds[i].fd, resp1.c_str(), resp1.length(), 0);
						std::string resp2 = ":" + ClientsMap[_pfds[i].fd].getNickname() + " JOIN " + key + "\r\n";
						send(_pfds[i].fd, resp2.c_str(), resp2.length(), 0);
						std::string msg = ":" + ClientsMap[_pfds[i].fd]._client_host + " 332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + "topic" + "\r\n";
						handlePrivMsg(msg, i, channelsV);
						std::string resp3 = ClientsMap[_pfds[i].fd]._client_host + " 353 " + ClientsMap[_pfds[i].fd].getNickname() + " = " + key + " :" + ClientsMap[_pfds[i].fd].getUserName() + "\r\n";
						send(_pfds[i].fd, resp3.c_str(), resp3.length(), 0);
						std::string resp4 = ClientsMap[_pfds[i].fd]._client_host + " 366 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :End of /NAMES list\r\n";
						send(_pfds[i].fd, resp4.c_str(), resp4.length(), 0);
						it->second.addClient(_pfds[i].fd);
						}
						else
						{
							std::string resp = "475 " + ClientsMap[_pfds[i].fd].getNickname() + " incorrect password " + "\n";
							send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
							std::cout << "encorrect password\n";
						}
					}
				}
			}
			else
			{
				std::map<std::string,Channels>::iterator it = channelsV.find(key);
				if(it == channelsV.end())
				{
					if(key[0] != '#')
						key = "#" + key;
					std::string resp1 = ":" + ClientsMap[_pfds[i].fd].getNickname() + "!~" + ClientsMap[_pfds[i].fd].getUserName() + "@" + ClientsMap[_pfds[i].fd].ipAddress + " JOIN :" + key + "\r\n";
						send(_pfds[i].fd, resp1.c_str(), resp1.length(), 0);
						std::string resp2 = ":" + ClientsMap[_pfds[i].fd].getNickname() + " JOIN " + key + "\r\n";
						send(_pfds[i].fd, resp2.c_str(), resp2.length(), 0);
						std::string msg = ":" + ClientsMap[_pfds[i].fd]._client_host + " 332 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :" + "topic" + "\r\n";
						handlePrivMsg(msg, i, channelsV);
						std::string resp3 = ClientsMap[_pfds[i].fd]._client_host + " 353 " + ClientsMap[_pfds[i].fd].getNickname() + " = " + key + " :" + ClientsMap[_pfds[i].fd].getUserName() + "\r\n";
						send(_pfds[i].fd, resp3.c_str(), resp3.length(), 0);
						std::string resp4 = ClientsMap[_pfds[i].fd]._client_host + " 366 " + ClientsMap[_pfds[i].fd].getNickname() + " " + key + " :End of /NAMES list\r\n";
						send(_pfds[i].fd, resp4.c_str(), resp4.length(), 0);
					Channels newChannel(false);
					newChannel.addClient(_pfds[i].fd);
					newChannel.password = "0";
					channelsV[key] = newChannel;
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
					}else {
						if(it->second.havePassword == false)
						{
						std::string resp = ":" + ClientsMap[_pfds[i].fd].getNickname() + " JOIN " + key + "\n";
						resp += "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully JOINED " + key + "\n";
						send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
						it->second.addClient(_pfds[i].fd);
						}
						else
						{
							std::string resp = "339 " + ClientsMap[_pfds[i].fd].getNickname() + ": incorrect pass " + key + "\n";
							// resp += "001 " + ClientsMap[_pfds[i].fd].getNickname() + " You have sucssefully JOINED " + key + "\n";
							send(_pfds[i].fd, resp.c_str(), resp.length(), 0);
							std::cout << "encorrect password\n";
							return ;
						}
					}
				}
			}
			
		} 
	}