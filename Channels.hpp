#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Server.hpp"
class Channels
{
private:
    std::vector<int> clientsFd;
public:
    Channels(/* args */);
    ~Channels();
    void addClient(int _clientFd)
    {
        std::vector<int>::iterator it = clientsFd.begin();
        while(it != clientsFd.end() && *it != _clientFd)
            it++;
        if(it == clientsFd.end())
            clientsFd.push_back(_clientFd);
    }
    std::vector<int> getClientFd()
    {
        return clientsFd;
    }
};

Channels::Channels(/* args */)
{
}

Channels::~Channels()
{
}


#endif