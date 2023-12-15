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
class Client
{
private:
    struct pollfd pFd;
    std::string nickName;
    std::string userName;
    bool isAutonticated;
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
    void setIsAutonticated()
    {
        if(nickName.length() > 0 && userName.length() > 0)
            isAutonticated = true;
    }
    Client();
    Client(struct pollfd client)
    {
        pFd = client;
        nickName = "";
        userName = "";
        isAutonticated = false;
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