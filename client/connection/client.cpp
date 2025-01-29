#include "client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <chrono>
#include <fcntl.h>

Client::ClientServer::ClientServer( const int port , const char* ipAddr ) : _port(port) , _ipAddr( ipAddr )
{ 
}

bool Client::ClientServer::createSocket()
{
  if( ( _sockFd = socket(AF_INET , SOCK_STREAM , 0) ) < 0 )
  {
    std::cout<<" Error While Creating Socket: "<<std::endl; 
    return false;
  }

  std::cout<<"Successfully Created the socket , socket FD is : "<<_sockFd<<std::endl; 
   
  return true; 
}

bool Client::ClientServer::connectToHost()
{
  struct sockaddr_in sockAddr; 

  sockAddr.sin_family = AF_INET; 
  sockAddr.sin_port = htons(_port); 

  if( inet_pton( AF_INET , _ipAddr , &sockAddr.sin_addr.s_addr ) <= 0 )
  {
    std::cout<<" Error While getting ip address: "<<std::endl; 
    return false; 
  }

  if( connect( _sockFd , (struct sockaddr* )&sockAddr , sizeof(sockAddr ) ) < 0 )
  {
    std::cout<<"Error While connecting to the server: "<<std::endl; 
    return false; 
  }

  std::cout<<"Successfully connected to the Server with socket Fd: "<<_sockFd<<std::endl; 
  return true; 
}

bool Client::ClientServer::connectWithExchange()
{
  if( !createSocket() )
  {
    return false; 
  } 

  if( !connectToHost() )
  {
    return false;
  }

  return true; 
}

bool Client::ClientServer::isSocketAlive()
{
	char buffer[1];
  int bytesRead; 
  if( ( bytesRead = recv(_sockFd, buffer, sizeof(buffer), MSG_PEEK) ) == 0 )
  {
    std::cout<<"Connection is closed by server. "<<std::endl; 
    return false; 
  }
  else if (bytesRead < 0) 
  {
    std::cout<< "Error reading from socket." << std::endl;
    return false;
  }

  return true;
}

void Client::ClientServer::closeTheConnection()
{
  if( isSocketAlive() )
  {
    std::cout<<"Closing the connection: "<<std::endl; 
    close(_sockFd ); 
    _sockFd = -1; 
  }
}

bool Client::ClientServer::sendRequestToHost( const Messages::Request& payload )
{
  if( int bytesWritten = write( _sockFd , &payload , sizeof(payload) )  ; bytesWritten > 0 ) 
  {
    std::cout<<"Request is send to host, bytes written: "<<bytesWritten<<std::endl; 
    return true; 
   
  }

  std::cout<<"Error While Sending Request to Host: "<<std::endl; 
  return false; 
}

bool Client::ClientServer::requestAllDataFromExchange()
{
  Messages::Request payload( Messages::callType::STREAM_ALL_PACKETS ) ; 

  return sendRequestToHost(payload); 
}

bool Client::ClientServer::readFromHost()
{
  Messages::Response response; 

  if( int bytesRead = read( _sockFd , &response , sizeof(response) )  ; bytesRead > 0 )
  {
    // Parsing the response here 
    // converting big endian to little endian
    response.parseTheResponse(); 

    // Dumping the response
    response.dump();

    ++_seqNo; 
    
    if( response.getSeqNo() > _seqNo ) 
    {
      std::cout<<"Out of the order seq number is recieved , seq number received: "<<response.getSeqNo()<<" Sequence number expected: "<<_seqNo<<std::endl; 

      // Request the server to send the data of this seq number; 
      
      for( int i = _seqNo  ; i<response.getSeqNo() ; ++i )
      {
        _missedPacketSet.insert( i ) ;  
      }

      _seqNo = response.getSeqNo(); 
    }
    
    auto result = _seqAndPacketsMap.emplace(response.getSeqNo() , response);
    
    return true; 
  }
  
  return false; 
}

bool Client::ClientServer::readAllDataFromExchange()
{
  while( true )
  {
    if( !readFromHost() )
    {
      std::cout<<"Nothing to Read now: "<<std::endl; 

      break; 
    }
  }

  return true; 
}


bool Client::ClientServer::isAnySeqIsMissed()
{
  return !_missedPacketSet.empty();
}

bool Client::ClientServer::requestAndReadMissedSeqPacket()
{
  // Requesting missed seq number packets
  auto requestMissedPacket = [this]()
  {
    for( auto& seq : _missedPacketSet )
    {
      std::cout<<"Request Packet of seq Number: "<<seq<<std::endl; 

      Messages::Request payload( Messages::callType::RESEND_PACKET , seq ) ; 

      sendRequestToHost( payload ) ; 
    }
  }; 

  // Read Response of Missed Packets
  auto readMissedPacket = [this]()
  {
  	Messages::Response response;
    
    while( true ) 
    {
      response.clear(); 

      if( int bytesRead = read( _sockFd , &response , sizeof(response) )  ; bytesRead > 0 )
  		{
        // Parse the response
        response.parseTheResponse();

    		response.dump();

        // Erase the seq number which is recived
        _missedPacketSet.erase( response.getSeqNo() ); 

        // insert in the map of sequence vs packets 
        _seqAndPacketsMap.emplace( response.getSeqNo() , response ); 
       
        if( _missedPacketSet.empty() )
        {
          std::cout<<"Got All the packets: "<<std::endl; 
          break;
        }
    	}
      
		}

    return ; 
	}; 

  // Firstly check if socket is alive or not , if not then create the connection 
  if( !isSocketAlive() )
  {
    // Create connection with the host 
    if( connectWithExchange() )
    {
     	requestMissedPacket(); 
    }
    else
    {
      std::cout<<"Error while connecting with exchange"<<std::endl; 
      return false; 
    }

    readMissedPacket();
  }
  else
  {
    requestMissedPacket();
 
    readMissedPacket(); 
  }
  
  return true;
}

bool Client::ClientServer::writeToFile()
{
  json jsonArray = json::array(); 

  for( auto& packet : _seqAndPacketsMap ) 
  {
    jsonArray.push_back( packet.second.toJson() ); 
  }

  if( !_outputFile.is_open() )
  {
    _outputFile.open(_fileName , std::ios::out | std::ios::app ); 

    if( !_outputFile.is_open() )
    {
      std::cout<<"Failed to open the file: "<<std::endl; 
      return false; 
    }
  }

  _outputFile<<jsonArray.dump(4);

  return true; 
}

Client::ClientServer::~ClientServer()
{
  // In destructor we are writing data in the file 
  if( !writeToFile() )
  {
    std::cout<<"Error While Writing to File: "<<std::endl;
  }

  // if output file is open then flush the data 
  // And close the file
  if( _outputFile.is_open() )
  {
    _outputFile.flush(); 
    _outputFile.close(); 
  }

  // Close the connection 
  close(_sockFd ); 
}


