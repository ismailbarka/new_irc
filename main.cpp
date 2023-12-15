#include "Client.hpp"

int main() {
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Set socket options to reuse address
    int tmp = 1;
    int nRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
    if (!nRet) {
        std::cout << "nRet success\n";
    } else {
        std::cout << "nRet failed\n";
    }

    // Set socket to non-blocking mode
    int nFl = fcntl(sock, F_SETFL, O_NONBLOCK);
    if (nFl < 0) {
        std::cout << "failed to set socket to non-blocking mode\n";
    } else {
        std::cout << "socket to non-blocking mode\n";
    }

    // Set up server address structure
    struct sockaddr_in server_adr;
    memset(&server_adr, '\0', sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_port = htons(5000);

    // Bind the socket to a port
    int bCheck = bind(sock, (struct sockaddr *)&server_adr, sizeof(server_adr));
    if (bCheck < 0)
        std::cout << "failed to bind the socket\n";

    // Start listening on the socket
    if (listen(sock, 128) < 0)
        std::cout << "failed to start listening from the server\n";

    // Set up pollfd array for polling
    struct pollfd _fds[1024];
    _fds[0].fd = sock;
    _fds[0].events = POLLIN;
    _fds[0].revents = 0;

    // Number of connected clients
    int clients_numbers = 1;

    // Map to store client information
    std::map<int, Client> ClientsMap;

    // Main server loop
    while (1) {
        // Poll for events on the sockets
        int pollResult = poll(_fds, clients_numbers, -1);
        if (pollResult == -1) {
            perror("poll error");
            // Handle the error as needed
        }

        // Check if there is a new client connection
        if (_fds[0].revents == POLLIN) {
            std::cout << "new client connected\n";

            // Accept the new client
            struct pollfd newClientPoll;
            newClientPoll.fd = accept(sock, NULL, NULL);
            newClientPoll.events = POLLIN;
            newClientPoll.revents = 0;

            // Add the new client to the _fds array
            _fds[clients_numbers] = newClientPoll;
            clients_numbers++;
        }

        // Check for events and data on existing client sockets
        for (int i = 1; i < clients_numbers; i++) {
            if (_fds[i].revents == POLLIN) {
                char buffer[1024];
                int readed = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
                if (readed <= 0) {
                    if (readed == 0) {
                        std::cout << "Connection closed by client " << i << std::endl;
                        // Handle client disconnection, remove from data structures if necessary
                    } else {
                        perror("recv error");
                    }
                } else {
                    // Check if the client is new or already in the map
                    std::map<int, Client>::iterator it = ClientsMap.find(_fds[i].fd);
                    if(it == ClientsMap.end()) {
                        // New client authentication
                        std::string line = buffer;
                        if(line == "PASS\n") {
                            std::cout << "new client added\n";
                            write(_fds[i].fd, "welcome to the server\n", 22);
                            Client newClien(_fds[i]);
                            ClientsMap[_fds[i].fd] = newClien;
                        } else {
                            std::cout << "wrong pass\n";
                            write(_fds[i].fd, "wrong pass retry\n", 17);
                        }
                        memset(buffer, 0, 1024);
                    } else {
                        // Existing client handling
                        std::stringstream iss(buffer);
                        std::string line;
                        std::getline(iss,line, ' ');
                        if(line == "USER") {
                            // Set user name for the client
                            std::getline(iss,line);
                            it->second.setuserName(line);
                            it->second.setIsAutonticated();
                            std::cout  << "is auto : " << it->second.getIsAutonticated() << std::endl;
                        } else if(line == "NICK") {
                            // Set nickname for the client
                            std::getline(iss,line);
                            it->second.setNickname(line);
                            std::cout << line << std::endl;
                            it->second.setIsAutonticated();
                            std::cout  << "is auto : " << it->second.getIsAutonticated() << std::endl;
                        }
                    }
                }
            }
        }
    }
}
