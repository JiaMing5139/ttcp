//
// Created by parallels on 1/18/20.
//

#include "net/InetAddress.h"
#include <memory.h>
#include "common.h"
#include <iostream>
#include  <errno.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int wirte_n(int sockfd, void* buf, int length){
    int nwrite = 0;
    while(nwrite < length){


    }

}

int read_n(int sockfd, void* buf, int length)
{
    int nread = 0;
    while (nread < length)
    {
        ssize_t nr = ::recv(sockfd, static_cast<char*>(buf) + nread, length - nread,0);
        if (nr > 0)
        {
            nread += static_cast<int>(nr);
        }
        else if (nr == 0)
        {
            break;  // EOF
        }
        else if (errno != EINTR)
        {
            perror("read");
            break;
        }
    }
    return nread;
}

bool transmit(const Options & opt){
    std::cout << "transmiting" << std::endl;
    int fd  = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    std::cout<< opt.host << " "<< opt.port << std::endl;
    InetAddress recvAddress(opt.host.c_str(),opt.port);
    if(connect(fd,recvAddress.getInetAddress(),recvAddress.getLen())){
        perror("connect");
        exit(EXIT_FAILURE);
    }


    sessionMessage* sessionPkt = new sessionMessage(opt.length,opt.number);

    ssize_t n = send(fd,sessionPkt,sizeof(sessionPkt),0);
     if(n == -1){
        perror("send");
        exit(EXIT_FAILURE);
    }

     //generate data pkt
    size_t dataMessagePktLen = sizeof(messageData) + opt.length;
    messageData* dataPkt = static_cast<messageData*> (malloc(dataMessagePktLen));
    for(int i = 0; i < opt.length; i++){
        dataPkt->data[i] = (char)(i%16);
    }
     double total_sent_mb = (double)opt.length*(double)opt.number / 1024 /1024;
    dataPkt->length = opt.length;

    time_t startTime;
    time_t endTime;
    time(&startTime);
    for(int num = 0; num < opt.number; num++ ) {
        // send a packet
        ssize_t ns = send(fd, dataPkt, dataMessagePktLen, 0);
        assert(ns == dataMessagePktLen);
    //    std::cout << "send a packet:" << ns << std::endl;
        // recv a ack
        int ack = 0;
        int nr = recv(fd, &ack, sizeof(ack), 0);
        assert(nr == sizeof(ack));
        assert(ack == opt.length);
      //  std::cout << "recv a ack::" << ack << std::endl;
    }
    time(&endTime);
    std::cout <<"total sent::" << total_sent_mb <<" mb"<< std::endl;
    std::cout <<"time::cost:"<<endTime - startTime<< std::endl;
    printf("%.3f mib/s\n", total_sent_mb / (endTime - startTime) );
}

bool receive(const Options & opt){
    std::cout<<"sizeof(messageData):" << sizeof(messageData) << std::endl;
    std::cout << "receiving" << std::endl;
    int fd = acceptFd(opt);
    char * buffer  = new char[sizeof(sessionMessage)];
    ssize_t n = recv(fd,buffer,sizeof(sessionMessage),0);
    if(-1 ==n ){
        perror("receive");
        exit(EXIT_FAILURE);
    }
    //handle session pkt
    int total_num = 0;
    sessionMessage * recevSessionmessage = (sessionMessage*)buffer;
    std::cout << "Session:: len:"<<recevSessionmessage->len<<" num:"<<recevSessionmessage->num << std::endl;
    size_t dataMessagePktLen = sizeof(messageData) + recevSessionmessage->len;
    total_num = recevSessionmessage->num;
    delete[] buffer;
    std::cout << "start tranmit" << std::endl;


    //buffer data is a buffer of recv
    messageData * bufferData = static_cast<messageData*> (malloc(dataMessagePktLen));
    long double total_recv = 0;
    for(int num = 0;num < total_num;num++){
        //bufferData = static_cast<messageData*> (malloc(dataMessagePktLen));
        // recv a data pkt
        ssize_t nr = read_n(fd,bufferData,dataMessagePktLen);
        //std::cout << "recv::" << nr << " errno:" <<errno<<std::endl;

        assert(nr == dataMessagePktLen);
        int ack = bufferData->length;

        //send a ack
        int ns = send(fd, &ack, sizeof(ack), 0);
        assert( ns == sizeof(ack));
        total_recv += bufferData->length;
    }
    std::cout << "total_recv:"<< total_recv << std::endl;
    close(fd);
    free(bufferData);


}

