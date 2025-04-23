#ifndef BOARD_CONNECTOR_H
#define BOARD_CONNECTOR_H

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <thread>

#define MAX_BUFFER_LEN 256
class BoardConnector {
private:
    std::string __port;
    int __portfd;
    int __baudrate;
    termios __tty;
    bool __connected;
    std::thread listenThread;
    void readData();
    void handleData(const std::string &data);
public:
    BoardConnector(const std::string &port);
    ~BoardConnector();
    void startConnection();
    bool isConnected() const;
};

#endif
    