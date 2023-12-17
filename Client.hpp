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
	char _client_ip[INET_ADDRSTRLEN];
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
    std::string getuserName()
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
        if(nickName.length() > 0 && userName.length() > 0)
            isAutonticated = true;
    }
	int getfd(){
		return pFd.fd;
	}
    Client();
    Client(struct pollfd client)
    {
        pFd = client;
        nickName = "";
        userName = "";
        isAutonticated = false;
		fillCommandList();
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