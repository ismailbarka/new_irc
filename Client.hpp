#ifndef Client_hpp
#define Client_hpp
#include <iostream>
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <map>
#include <arpa/inet.h>
class Client
{
private:
    struct pollfd pFd;
    std::string nickName;
    std::string userName;
    bool isAutonticated;
	std::vector<std::string> commandList;
	bool correctPassword;
	char _client_ip[INET_ADDRSTRLEN];
	std::string _client_host;
public:
    void setPollfd(struct pollfd _pFd)
    {
        pFd = _pFd;
    }
    void setNickname(std::string _nickName)
    {
        nickName = _nickName;
    }
    void setuserName(std::string _userName)
    {
        userName = _userName;
    }
    std::string getNickname()
    {
        return nickName;
    }
    std::string getUserName()
    {
        return userName;
    }
    bool getIsAutonticated()
    {
        return isAutonticated;
    }
	void setClientIp(char* ip)
	{
		strcpy(_client_ip, ip);
	}
    void setIsAutonticated()
    {
        if (nickName.length() > 0 && userName.length() > 0 && correctPassword == true)
            isAutonticated = true;
    }
	int getfd(){
		return pFd.fd;
	}
	void setCorrectPassWord(bool _correctPassword){
		correctPassword = _correctPassword;
	}
	bool getCorrectPassWord(){
		return correctPassword;
	}
    Client();
    Client(struct pollfd client)
    {
        pFd = client;
        nickName = "";
        userName = "";
		_client_host = "";
        isAutonticated = false;
		correctPassword = false;
		fillCommandList();
    }
	std::string getClientHost() const {
		return _client_host;
	}
	void setClientHost(std::string host) {
		_client_host = host;
	}
	void fillCommandList()
	{
		commandList.push_back("USER");
		commandList.push_back("NICK");
		commandList.push_back("LIST");
		commandList.push_back("QUIT");
	}
    ~Client();
};

Client::Client()
{
}

Client::~Client()
{
}


#endif