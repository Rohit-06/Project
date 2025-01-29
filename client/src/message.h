#ifndef MESSAGE_H
#define MESSAGE_H

#pragma pack(push , 1)

#include<iostream> 
#include<netinet/in.h>
#include<cstring>
#include<nlohmann/json.hpp>

using json = nlohmann::json; 

namespace Messages
{
  enum callType
  {
    STREAM_ALL_PACKETS = 1,
    RESEND_PACKET = 2
  };

  struct Request
  {
    unsigned char callType ; 
    unsigned char resentSeq ; 

    Request( unsigned char callType , unsigned char resentSeq = 0 ) : callType( callType ) , resentSeq( resentSeq )
    {
    }
  }; 

  struct Response
  {
    char symbol[4]; 
    char orderMode; 
    int32_t orderQuantity; 
    int32_t orderPrice;
    int32_t seqNo;

    void parseTheResponse()
    {
      orderQuantity = ntohl( orderQuantity ); 
      orderPrice = ntohl( orderPrice ) ; 
      seqNo = ntohl( seqNo ) ;  
    }

    void clear()
    {
      memset( symbol , '\0' , sizeof( symbol ) );  
      orderMode = '\0' ;
      orderQuantity = 0 ; 
      orderPrice = 0 ; 
      seqNo = 0 ; 
    }

    json toJson()
    {
      return {
        {"Symbol",std::string(symbol , 4)}, 
        {"OrdreMode",std::string(1 , orderMode)},
        {"OrderQuantity",orderQuantity},
        {"OrderPrice",orderPrice},
        {"SeqNo",seqNo}
      };
    }

    int32_t getSeqNo()
    {
      return seqNo; 
    }

    void dump()
    {
      std::cout<<"===== Order Book Details: ======="<<std::endl;
      std::cout<<" Symbol is: "<<std::string(symbol , 4)
        <<"\n Order Mode is: "<<orderMode
        <<"\n Order Quantity is: "<<orderQuantity
        <<"\n Order Price is: "<<orderPrice
        <<"\n Seq Number is: "<<seqNo<<std::endl; 
      std::cout<<"=================================="<<std::endl; 
    }
  }; 
}

#pragma pack(pop)

#endif
