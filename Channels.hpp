#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Server.hpp"
class Channels
{
private:
public:
    std::vector<int> clientsFd;
    std::string password;
    bool havePassword;
    std::string topic;
    Channels()
    {
        topic = "no topic yet\n";
    };
    Channels(bool _havePassword)
    {
        havePassword = _havePassword;
        topic = "no topic yet\n";
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