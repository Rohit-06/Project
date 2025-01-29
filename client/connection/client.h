#ifndef CLIENT_H
#define CLIENT_H


#include<iostream>
#include<map>
#include<unordered_set>
#include"../src/message.h"
#include<fstream>

namespace Client
{
  class ClientServer
  {
    const int _port ; 
    const char* _ipAddr ; 
    int _sockFd = 0 ; 
    int32_t _seqNo = 0 ; 
    const std::string _fileName = "packets.json"; 
    std::ofstream _outputFile ; 
    std::unordered_set<int32_t> _missedPacketSet; 
    std::map<int32_t , Messages::Response> _seqAndPacketsMap ; 

    public:

    ClientServer( const int port , const char* ipAddr ) ; 

    bool createSocket(); 

    bool connectToHost() ; 

    bool sendRequestToHost(const Messages::Request& payload );

    bool readFromHost();

    bool readAllDataFromExchange(); 

    void closeTheConnection();

    bool isAnySeqIsMissed();

    bool connectWithExchange(); 

    bool requestAllDataFromExchange(); 

    bool requestAndReadMissedSeqPacket();
    
    bool isSocketAlive();  

    bool writeToFile(); 

    ~ClientServer(); 

  }; 
}

#endif
	
