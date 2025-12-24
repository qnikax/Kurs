#include "Logger.h"

#include <cassert>
#include <fstream>
#include <ctime>
#include <cstring>  // Добавь этот include

Logger* Logger::m_state = nullptr;

Logger& Logger::get()
{
    if(m_state == nullptr)
        m_state = new Logger();
    return *m_state;
}

Logger* Logger::getPtr()
{
    if(m_state == nullptr)
        m_state = new Logger();
    return m_state;
}

bool Logger::Initialize(std::string logPath)
{
    m_pathToFile = logPath;

    m_logFile.open(m_pathToFile, std::ios_base::app|std::ios_base::out);

    if(!m_logFile.is_open())
        return false;

    return true;
}

void Logger::info(std::string str)
{
    std::time_t time = std::time(nullptr);
    char timeString[100];  // Просто большой фиксированный буфер
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    std::cout << timeString;
    m_logFile << timeString;

    std::cout << " <INFO> : " << str << std::endl;
    m_logFile << " <INFO> : " << str << std::endl;
}

void Logger::serverError(ServerError serverError)
{
    std::time_t time = std::time(nullptr);
    char timeString[100];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    std::cout << timeString;
    m_logFile << timeString;

    switch (serverError.getErrorLevel())
    {
    case ErrorLevel::Critical:
        std::cout << " <CRITICAL> : ";
        m_logFile << " <CRITICAL> : ";
        break;
    case ErrorLevel::Warning:
        std::cout << " <WARNING> : ";
        m_logFile << " <WARNING> : ";
        break;    
    default:
        break;
    }
    std::cout << serverError.what() << std::endl;
    m_logFile << serverError.what() << std::endl;
}
