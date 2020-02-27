#include <iostream>
#include "common.h"
#include <string>
#include <netdb.h>
int main(int argc,char *argv[]) {

    Options opt;
    praseCommand(argc,argv,&opt);
    if(opt.transmit){
        transmit(opt);
    }else if(opt.receive){
        receive(opt);
    }

}