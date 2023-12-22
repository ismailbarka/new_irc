#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Server.hpp"
class Channels
{
private:
public:
    std::vector<int> clientsFd;
    std::string topic;
    Channels()
    {
        topic = "notopic yet\n";
    };
    ~Channels();
    void addClient(int _clientFd)
    {
        std::vector<int>::iterator it = clientsFd.begin();
        while(it != clientsFd.end() && *it != _clientFd)
            it++;
        if (it == clientsFd.end())
            clientsFd.push_back(_clientFd);
    }
    std::vector<int> getClientFd()
    {
        return clientsFd;
    }
	std::vector<int>  & getClientsFd()
	{
		return clientsFd;
	}
};

Channels::~Channels()
{
}


#endif