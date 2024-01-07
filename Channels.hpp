#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Server.hpp"
class Channels
{
	private:
	public:
		std::vector<int> clientsFd;
		std::vector<Client> operators;
		std::vector<Client> invited;
		std::string mode;
		std::string password;
    	std::string topic;
		std::string limit;
    	bool havePassword;
		Channels();
		Channels(bool _havePassword);
		~Channels();
		std::string getMode();
		void setKey(std::string _key);
		void setLimit(std::string _limit);
		void addClient(int _clientFd);
		void setMode(std::string _mode);
		bool removeClient(int _clientFd);
		bool haveClient(int _clientFd);
		bool isOperator(int _clientFd);
		// std::vector<int> getClientFd();
		// std::vector<int>  & getClientsFd();
};

#endif