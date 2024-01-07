#include "Server.hpp"

void Server::fetchDataFromAPI() {
    const char* apiHost = "api.quotable.io";
    const char* apiEndpoint = "/random?tags=sport";
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

    std::stringstream request;
    request << "GET " << apiEndpoint << " HTTP/1.1\r\n";
    request << "Host: " << apiHost << "\r\n";
    request << "Connection: close\r\n\r\n";

    std::string req = request.str();
    const char* buffer = req.c_str();

    n = send(sockfd, buffer, strlen(buffer), 0);
    if (n < 0) {
        perror("ERROR writing to socket");
        return;
    }

    char response[4096];
    std::stringstream fullResponse;

    while ((n = recv(sockfd, response, sizeof(response), 0)) > 0) {
        fullResponse.write(response, n);
    }

    // Displaying received data
    // cout << "Received data from API:\n" << fullResponse.str() << endl;
	std::string jsonString = fullResponse.str();
	jsonString = jsonString.substr(jsonString.find_last_of("{"));
	std::cout << jsonString << std::endl;
	std::cout << "================" << std::endl;
	// std::stringstream ss(jsonString);
	// std:string line;
	// while (std::getline(ss, line, ','))
	// {
	// 	std::cout << "line => " << line << std::endl;
	// }
	size_t start = jsonString.find("\"_id\":");
    size_t end = jsonString.find(",\"content\":");
    std::string id = jsonString.substr(start + 6, end - start - 7);

    start = jsonString.find("\"content\":");
    end = jsonString.find(",\"author\":");
    std::string content = jsonString.substr(start + 11, end - start - 12);

    start = jsonString.find("\"author\":");
    end = jsonString.find(",\"tags\":");
    std::string author = jsonString.substr(start + 10, end - start - 11);

    start = jsonString.find("\"tags\":");
    end = jsonString.find(",\"authorSlug\":");
    std::string tags = jsonString.substr(start + 7, end - start - 8);

    start = jsonString.find("\"authorSlug\":");
    end = jsonString.find(",\"length\":");
    std::string authorSlug = jsonString.substr(start + 13, end - start - 14);

    start = jsonString.find("\"length\":");
    end = jsonString.find(",\"dateAdded\":");
    std::string length = jsonString.substr(start + 9, end - start - 10);

    start = jsonString.find("\"dateAdded\":");
    end = jsonString.find(",\"dateModified\":");
    std::string dateAdded = jsonString.substr(start + 12, end - start - 13);

    start = jsonString.find("\"dateModified\":");
    std::string dateModified = jsonString.substr(start + 15, jsonString.length() - start - 16);

    std::cout << "=> ID: " << id << std::endl;
    std::cout << "=> Content: " << content << std::endl;
    std::cout << "=> Author: " << author << std::endl;
    std::cout << "=> Tags: " << tags << std::endl;
    std::cout << "=> Author Slug: " << authorSlug << std::endl;
    std::cout << "=> Length: " << length << std::endl;
    std::cout << "=> Date Added: " << dateAdded << std::endl;
    std::cout << "=> Date Modified: " << dateModified << std::endl;
    close(sockfd);
}

