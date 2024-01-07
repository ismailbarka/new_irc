#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
using namespace std;

void fetchDataFromAPI() {
    const char* apiHost = "api.quotable.io";
    const char* apiEndpoint = "/random?tags=technology";
    int portno = 80; // HTTP port

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return;
    }

    server = gethostbyname(apiHost);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        return;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        return;
    }

    stringstream request;
    request << "GET " << apiEndpoint << " HTTP/1.1\r\n";
    request << "Host: " << apiHost << "\r\n";
    request << "Connection: close\r\n\r\n";

    string req = request.str();
    const char* buffer = req.c_str();

    n = send(sockfd, buffer, strlen(buffer), 0);
    if (n < 0) {
        perror("ERROR writing to socket");
        return;
    }

    char response[4096];
    stringstream fullResponse;

    while ((n = recv(sockfd, response, sizeof(response), 0)) > 0) {
        fullResponse.write(response, n);
    }

    // Displaying received data
    // cout << "Received data from API:\n" << fullResponse.str() << endl;
	std::string qu = fullResponse.str();
	qu = qu.substr(qu.find_last_of("{"));
	std::cout << qu << std::endl;

    close(sockfd);
}

int main() {
    fetchDataFromAPI();
    return 0;
}
