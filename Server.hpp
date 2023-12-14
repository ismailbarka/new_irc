#ifndef SERVER_HPP
#define SERVER_HPP

class Server
{
private:
    int socket_fd;
    int _port;
    struct sockaddr_in
public:
    Server(/* args */);
    ~Server();
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}


#endif
