#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Server.hpp"
class Channels
{
	private:
	public:
		std::vector<int> clientsFd;
		std::string mode;
		std::string password;
    	bool havePassword;
    	std::string topic;
		Channels();
		Channels(bool _havePassword);
		~Channels();
		void addClient(int _clientFd);
		std::vector<int> getClientFd();
		std::vector<int>  & getClientsFd();
		void setMode(std::string _mode);
		std::string getMode();
		void removeClient(int _clientFd);
};

#endif