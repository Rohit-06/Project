#include "../connection/client.h"
#include <memory>
#include<chrono>

int main()
{
  // Create Object of Client
  int portNo = 3000; 
  constexpr const char* ipAddr = "127.0.0.1";

  std::unique_ptr<Client::ClientServer> clientPtr = std::make_unique<Client::ClientServer>( portNo , ipAddr );

  // Create the socket 

  if( !clientPtr->connectWithExchange() )
  {
    std::cout<<"Error while connecting with exchange closing the application: "<<std::endl;
    exit(1); 
  }

  // Request From Host
  if( !clientPtr->requestAllDataFromExchange() )
  {
    std::cout<<"Error while request data from exchange: "<<std::endl;
    exit(1); 
  }

  // read from host
  
  clientPtr->readAllDataFromExchange(); 

  if( clientPtr->isSocketAlive() )
  {
    std::cout<<" Connection is not closed: "<<std::endl; 
  }

  if( clientPtr->isAnySeqIsMissed() )
  {
    // Create The Connection And Request the missed seq packets 
    clientPtr->requestAndReadMissedSeqPacket(); 
  }

}
  
