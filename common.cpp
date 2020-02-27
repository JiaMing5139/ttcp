//
// Created by parallels on 1/15/20.
//


#include <boost/program_options.hpp>
#include <string>
#include "common.h"
#include "net/InetAddress.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
namespace  programOption= boost::program_options;

bool praseCommand(int argc,char* argv[],Options * opt){
    programOption::options_description desc("Allow options");
    desc.add_options()
            ("help,h","Help")
            ("port,p",programOption::value<uint16_t >(&opt->port)->default_value(5001),"the name of the file which you want to process")
            ("length,l", programOption::value<int>(&opt->length)->default_value(65536), "Buffer length")
            ("number,n", programOption::value<int>(&opt->number)->default_value(16384), "Number of buffers")
            ("recv,r",programOption::value<std::string>(&opt->host),  "receive")
            ("transmit,t",programOption::value<std::string>(&opt->host), "Transmit")
            ("show,s","show all parameters") ;
    programOption::variables_map vm;
    try{
        programOption::store(programOption::parse_command_line(argc,argv,desc),vm);
        programOption::notify(vm);
    }catch(std::runtime_error erro){
      //  std::cout << ... <<std::endl;
            std::cout << erro.what() <<std::endl;
    }
    opt->transmit = vm.count("transmit");
    opt->receive = vm.count("recv");



    if(vm.count("help")){
        std::cout << desc << std::endl;
    }

    if(vm.count("show")){
        std::cout<<"port:" << opt->port << std::endl;
        std::cout<<"host:"  << opt->host << std::endl;
        std::cout<<"length:" << opt->length << std::endl;
        std::cout<<"number:" << opt->number << std::endl;
    }

    if(opt->transmit == opt->receive){
        std:: cout << "specified a mod" << std::endl;
        return  false;

    }
}

int acceptFd(const Options& opt){
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    assert(listenfd >= 0);
    InetAddress addr(opt.host.c_str(),opt.port);
    int sockOptValue = 1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&sockOptValue,sizeof(sockOptValue))){
        perror("setsockopt");
        exit(1);
    }

    if(bind(listenfd,addr.getInetAddress(),addr.getLen())){
        perror("bind");
        exit(1);
    }

    if(listen(listenfd,10)){
        perror("listen");
        exit(1);
    }

    std::cout << "waitng accepting " << std::endl;
    int nfd = accept(listenfd,0,0);
    assert(nfd >= 0);

    close(listenfd);
    return nfd;
}

void showdDataAsBtye( const void* dataBuffer,size_t len){
    const char* charBuffer = (char*) dataBuffer;
    for(int i = 0;i < len;i++){
        printf("%d \n", charBuffer[i]);
    }
}




