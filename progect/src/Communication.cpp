#include "Communication.h"

#include "ServerError.h"

#include <netinet/in.h>
#include <random>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>

using namespace CryptoPP;

std::string MD(std::string sah)
{
    Weak::MD5 hash;
    std::string digest;

    auto stringSink  = new StringSink(digest);
    auto hexEncoder = new HexEncoder(stringSink);
    auto hashFilter = new HashFilter(hash,hexEncoder);

    StringSource(sah, true, hashFilter);
    
    return digest;
}

void Communication::createSocket(int port)
{
    m_socket = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in sockAddr = {};

    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(port);
    sockAddr.sin_family = AF_INET;

    auto b = bind(m_socket,(sockaddr*)&sockAddr,sizeof(sockAddr));
    if(b == -1)
        throw ServerError("socket creation failure",ErrorLevel::Critical);
    
    listen(m_socket, SOMAXCONN);
}

int Communication::waitClient()
{
    sockaddr_in sock;
    socklen_t len = sizeof (sockaddr_in);
    auto clientSocket = accept(m_socket,(sockaddr*)&sock,&len);

    return clientSocket;
}

void Communication::autorize(int clientSocket,UsersParser users){

    std::random_device rnd;
    std::mt19937_64 rnd_generator(rnd());
    uint64_t randNum = rnd_generator();
    std::string salt;
    StringSource((const byte*)&randNum,sizeof(randNum),true,new HexEncoder(new StringSink(salt)));
    char messageBuffer[256];

    auto packetSize = recv(clientSocket,messageBuffer,sizeof(messageBuffer),0);

    auto login = std::string(messageBuffer,packetSize);
    if(!users.isHasUser(login))
        throw ServerError("User doesn\'t exists",ErrorLevel::Warning);

    packetSize = send(clientSocket,salt.data(),salt.length(),0);

    auto password = users.getPassword(login);
    
    std::string sah = salt + password;
    std::string digest;
    digest = MD(sah);

    packetSize = recv(clientSocket,messageBuffer,sizeof(messageBuffer),0);

    if(memcmp(digest.data(),messageBuffer,digest.length())){
        send(clientSocket,"ERR",3,0);  
        throw ServerError("Invalid password",ErrorLevel::Warning);
    }
    else{  
        send(clientSocket,"OK",2,0);   
    }
}