#include "Server.h"


#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <vector>


bool Server::Initialize(int argc,char** argv)
{
    m_cmd.addParam("--port", "-p", "<port> , port must be less than 65536", param_type::integer);
    m_cmd.addParam("--file", "-f", "<users File>");
    m_cmd.addParam("--logs", "-l", "<logs File>");
    
    try
    {
        m_cmd.parse(argc, argv);
    }
    catch (ServerError err)
    {
        std::cout << err.what() << std::endl;

        m_cmd.printHelp();
        return true;
    }

    if (m_cmd.isHasHelp())
    {
        m_cmd.printHelp();
        return true;
    }

    try
    {
        Logger::get().Initialize(m_cmd.getString("--logs"));
        m_usersParser.parse(m_cmd.getString("--file"));

        m_com.createSocket(m_cmd.getInt("--port"));
    }

    catch (ServerError err)
    {
        Logger::get().serverError(err);
        return false;
    }

    m_mustDie = false;
    Logger::get().info("Initialization succsessfull");
    

    return true;
}

void Server::start()
{
    int client = 0;

    while (!m_mustDie)
    {
        try
        {
            client = m_com.waitClient();
            m_com.autorize(client, m_usersParser);
            executeCalculations(client);
        }
        catch (ServerError err)
        {
            Logger::get().serverError(err);
            if (err.getErrorLevel() == ErrorLevel::Critical)
                m_mustDie = true;

            close(client);
        }
    }
}

void Server::executeCalculations(int client)
{
    uint32_t numOfVectors;
    uint32_t vectorLenght;
    std::vector<int16_t> vector;

    recv(client, &numOfVectors, sizeof(numOfVectors), 0);

    for (uint32_t i = 0; i < numOfVectors; i++)
    {

        recv(client, &vectorLenght, sizeof(vectorLenght), 0);
        vector.resize(vectorLenght);
        while (1)
        {
            int numOfBytes = 0;
            if (ioctl(client, TIOCINQ, &numOfBytes) < 0)
                throw ServerError("Invalid size",ErrorLevel::Warning);

            if (numOfBytes == 0)
                continue;

            if (numOfBytes < vector.size() * sizeof(int16_t))
                throw ServerError("Invalid size: data size is less than expected",ErrorLevel::Warning);
            else if(numOfBytes > vector.size() * sizeof(int16_t))
                throw ServerError("Invalid size: data size is greater than expected",ErrorLevel::Warning);
            else
                break;
        }

        recv(client, vector.data(), vector.size() * sizeof(int16_t), 0);

        int16_t sum = 0;

        for (const auto &vecEl : vector)
            sum += vecEl;

        send(client, &sum, sizeof(sum), 0);
    }
}
