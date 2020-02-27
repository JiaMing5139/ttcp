//
// Created by parallels on 2019/12/24.
//

#ifndef TESTCLIENT_INETADDRESS_H
#define TESTCLIENT_INETADDRESS_H

#include <arpa/inet.h>

class InetAddress{
public:
    InetAddress(const char * ip, int16_t port);
    struct sockaddr* getInetAddress();
    socklen_t getLen();
    ~InetAddress();

private:
    const char * _ip;
    int16_t _port;
    socklen_t len;
    struct sockaddr_in * addr;

};
#endif //TESTCLIENT_INETADDRESS_H
