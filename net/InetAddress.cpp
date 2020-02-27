//
// Created by parallels on 2019/12/24.
//

#include "InetAddress.h"
#include <string.h>
#include <memory>

InetAddress::InetAddress(const char *ip, int16_t port):_ip(ip),_port(port),len(sizeof(struct sockaddr_in)){
     this->addr = new sockaddr_in();
     memset(addr,0,sizeof(struct sockaddr_in));
     addr->sin_family = AF_INET;
     addr->sin_port = htons(_port);
     addr->sin_addr.s_addr = inet_addr(_ip);
}

struct sockaddr* InetAddress::getInetAddress() {
    return (struct sockaddr *)addr;
}
socklen_t InetAddress::getLen() {
    return len;
}

InetAddress::~InetAddress() {
    delete addr;
}
