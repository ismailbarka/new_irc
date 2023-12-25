#include "Client.hpp"

    void Client::setPollfd(struct pollfd _pFd)
    {
        pFd = _pFd;
    }
    void Client::setNickname(std::string _nickName)
    {
        nickName = _nickName;
    }
    void Client::setuserName(std::string _userName)
    {
        userName = _userName;
    }
    std::string Client::getNickname()
    {
        return nickName;
    }
    std::string Client::getUserName()
    {
        return userName;
    }
    bool Client::getIsAutonticated()
    {
        return isAutonticated;
    }
	void Client::setClientIp(char* ip)
	{
		strcpy(_client_ip, ip);
	}
    void Client::setIsAutonticated()
    {
        if (nickName.length() > 0 && userName.length() > 0 && correctPassword == true)
            isAutonticated = true;
    }
	int Client::getfd(){
		return pFd.fd;
	}
	void Client::setCorrectPassWord(bool _correctPassword){
		correctPassword = _correctPassword;
	}
	bool Client::getCorrectPassWord(){
		return correctPassword;
	}
    Client::Client(){};
    Client::Client(struct pollfd client)
    {
        pFd = client;
        nickName = "";
        userName = "";
		_client_host = "";
        isAutonticated = false;
		correctPassword = false;
		fillCommandList();
    }
	std::string Client::getClientHost() const {
		return _client_host;
	}
	void Client::setClientHost(std::string host) {
		_client_host = host;
	}
	void Client::fillCommandList()
	{
		commandList.push_back("USER");
		commandList.push_back("NICK");
		commandList.push_back("LIST");
		commandList.push_back("QUIT");
	}
    Client::~Client(){};

    void Client::setIpAddress(struct sockaddr_in client_addr)
    {
        char *str;
        str = inet_ntoa(client_addr.sin_addr);
        this->ipAddress = str;
    }

    void Client::setHostName()
    {
         std::system("hostname > ip.txt");
        std::ifstream ip("ip.txt");
        std::string hostname;
        std::getline(ip, hostname);
        std::remove("ip.txt");
        _client_host = hostname;
    }