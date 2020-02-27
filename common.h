//
// Created by parallels on 1/15/20.
//

#ifndef TTCP_COMMON_H
#define TTCP_COMMON_H
#include <string>


struct Options{
    std::string host;
    uint16_t port;
    int length;
    int number;
    bool transmit;
    bool receive;
    Options():port(0),transmit(false),receive(false){}
};

struct sessionMessage{
    int32_t len;
    int32_t num;
    sessionMessage(int32_t l,int32_t n):len(l),num(n){
    }

};

struct messageData{
    int length;
    char data[0];
};

class testConst{
public:
    testConst & operator[](int i);
private:
};

bool praseCommand(int argc,char* argv[],Options * opt);
bool transmit(const  Options & opt);
bool receive(const  Options & opt);
int acceptFd(const Options& opt);
void showdDataAsBtye( const void* dataBuffer,size_t len);
#endif //TTCP_COMMON_H
