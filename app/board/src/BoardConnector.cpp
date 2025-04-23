#include "../inc/BoardConnector.h"
#include "../../controllers/inc/MediaPlayController.h"

#include <stdio.h>

BoardConnector::BoardConnector(const std::string &port) : __port(port) {
    __connected = false;
    __baudrate = 9600;
}

BoardConnector::~BoardConnector() {
    if(__connected) {
        close(__portfd);
    }
    if(listenThread.joinable()) {
        __connected = false;
        listenThread.join();
    }
}

void BoardConnector::startConnection() {
    __portfd = open(__port.c_str(), O_RDWR | O_NOCTTY);
    std::cerr << strerror(errno) << "\n";
    if (__portfd < 0) return;

    tcgetattr(__portfd, &__tty);

    speed_t speed = (__baudrate == 9600) ? B9600 : B115200;
    cfsetispeed(&__tty, speed);
    cfsetospeed(&__tty, speed);

    __tty.c_cflag = CS8 | CREAD | CLOCAL;
    __tty.c_lflag = 0;
    __tty.c_iflag = 0;
    __tty.c_oflag = 0;
    __tty.c_cc[VMIN] = 0;
    __tty.c_cc[VTIME] = 1;

    tcsetattr(__portfd, TCSANOW, &__tty);
    if(listenThread.joinable()) {
        std::cerr << "Board already connected\n";
        return;
    }
    __connected = true;
    std::cerr << "Board connected\n";
    listenThread = std::thread([this]() { readData(); });
    listenThread.detach();
}

void BoardConnector::readData() {
    std::string unprocessedData("");
    while(__connected) {
        char buffer[MAX_BUFFER_LEN];
        int num_bytes = read(__portfd, buffer, MAX_BUFFER_LEN - 1);
        if(num_bytes < 0) {
            __connected = false;
            break;
        }
        if(num_bytes > 0) {
            buffer[num_bytes] = 0;
            for(int i=0;i<num_bytes-1;i++) {
                fprintf(stderr, "Data received: %x\n", buffer[i]);
            }
            unprocessedData.append(buffer);
            while(true) {
                size_t stop_char_index = unprocessedData.find_first_of('.');
                if(stop_char_index == std::string::npos) {
                    break;
                }
                std::string data = unprocessedData.substr(0, stop_char_index);
                handleData(data);
                if(stop_char_index == unprocessedData.length() - 1) {
                    unprocessedData.clear();
                    break;
                }
                unprocessedData = unprocessedData.substr(stop_char_index + 1);
            }
        }
        
    }
}

void BoardConnector::handleData(const std::string& data) {
    std::cerr << "Data handled: " << data <<  "\n";
    MediaPlayController* controller = MediaPlayController::getInstance();
    if(data == "T") {
        if(controller->getPlayState().isPlaying()) controller->pause();
        else controller->play();
        return;
    }
    if(data == "P") {
        controller->previous();
        return;
    }
    if(data == "N") {
        controller->next();
        return;
    }
    
    int volume_percent;
    if(sscanf(data.c_str(), "%u", &volume_percent) == -1) {
        controller->setVolumeByPercent(volume_percent);
        return;
    }
}

bool BoardConnector::isConnected() const {
    return __connected;
}