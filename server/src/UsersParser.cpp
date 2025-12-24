#include "UsersParser.h"
#include <fstream>

#include "ServerError.h"

void UsersParser::parse(std::string pathToFile)
{
    std::fstream inFile(pathToFile);

    if(!inFile.is_open())
        throw ServerError("users file is not exit",ErrorLevel::Critical);

    inFile.seekg(0,std::ios_base::end);
    auto fileSize = inFile.tellp();
    if(fileSize == 0)
        throw ServerError("users file is empty",ErrorLevel::Warning);
    inFile.seekg(0,std::ios_base::beg);

    std::string fileData;
    fileData.resize(fileSize);
    inFile.read(&fileData[0],fileSize);

    for(int i = 0; i < fileSize;i++){

        auto endOfLinePos = fileData.find('\n',i);
        if(endOfLinePos > fileSize)
            endOfLinePos = fileSize;
        auto divPos = fileData.find(':',i);
        if(divPos > fileSize)
            divPos = fileSize;

        if(divPos+1 == endOfLinePos)
            throw ServerError("users file format is invalid",ErrorLevel::Critical);

        std::string username = fileData.substr(i,divPos-i);
        std::string password = fileData.substr(divPos+1,endOfLinePos-divPos-1);

        if(isHasUser(username))
            throw ServerError("users file username is duplicated",ErrorLevel::Critical);

        m_users[username] = password;
        i=endOfLinePos; 
    }
}

bool UsersParser::isHasUser(std::string username)
{
    return m_users.find(username) != m_users.end();
}

std::string UsersParser::getPassword(std::string username)
{
    if(isHasUser(username))
        return m_users[username];
    return std::string();
}
