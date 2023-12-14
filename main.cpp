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
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
int main()
{
    //setsocket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int tmp = 1;
    int nRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
    if (!nRet)
    {
        std::cout << "nRet sucsess\n";
    }
    else
        std::cout << "nRet failed\n";
    int nFl = fcntl(sock, F_SETFL, O_NONBLOCK);
    if(!nFl)
    {
        std::cout << "failed to set socket to non-blocking mode\n";
    }
    else
        std::cout << "socket to non-blocking mode\n";
    struct sockaddr_in server_adr;
    //setaddr
    memset(&server_adr, '\0', sizeof(server_adr));     
    server_adr.sin_family = AF_INET;
    server_adr.sin_port = htons(5000);
    //bind(bind the socket to a port)
    int bCheck = bind(sock, (struct sockaddr *)&server_adr, sizeof(server_adr));
    if(bCheck < 0)
        std::cout << "failed to bind the socket\n";
    if(listen(sock, 128) < 0)
        std::cout << "failed to start listening from the server\n";
}