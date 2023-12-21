#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Server.hpp"
class Channels
{
	private:
		std::vector<int> clientsFd;
		std::string mode;
	public:
		Channels();
		~Channels();
		void addClient(int _clientFd);
		std::vector<int> getClientFd();
		std::vector<int>  & getClientsFd();
		void setMode(std::string _mode);
		std::string getMode();
};

#endif