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
#include <iostream>
#include <fstream>
#include <cstdlib>
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
public:
	std::string _client_host;
    std::string ipAddress;
    void setPollfd(struct pollfd _pFd);
    void setNickname(std::string _nickName);
    void setuserName(std::string _userName);
    std::string getNickname();
    std::string getUserName();
    bool getIsAutonticated();
	void setClientIp(char* ip);
    void setIsAutonticated();
	int getfd();
	void setCorrectPassWord(bool _correctPassword);
	bool getCorrectPassWord();
    Client();
    Client(struct pollfd client);
	std::string getClientHost() const;
	void setClientHost(std::string host);
	void fillCommandList();
    void setIpAddress(struct sockaddr_in client_addr);
    void setHostName();
    ~Client();
};

#endif